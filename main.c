#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define BUFFER_SIZE 256 // Define BUFFER_SIZE

typedef struct {
    char buffer[BUFFER_SIZE];
    int class; // 1 for CL1, 2 for CL2
} shared_memory;

void producer(int class, int semid, shared_memory* shm) {
    struct sembuf sb;

    FILE* file = fopen("messages.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    while (1) {
        // Wait before producing (P operation on the semaphore)
        sb.sem_num = 0;
        sb.sem_op = -1;  // Decrement the semaphore
        sb.sem_flg = 0;
        semop(semid, &sb, 1);

        // Check the class in the buffer
        if (shm->class == 0 || shm->class == class) {
            // Produce a message from the file
            if (fgets(shm->buffer, BUFFER_SIZE, file) != NULL) {
                shm->class = class;
                printf("Producer of class %d produced a message: %s", class, shm->buffer);
            } else {
                // If the end of the file is reached, break the loop
                break;
            }
        }

        // Signal after production (V operation on the semaphore)
        sb.sem_op = 1;  // Increment the semaphore
        semop(semid, &sb, 1);

        sleep(1); // To slow down production
    }

    fclose(file);
}

void consumer(int class, int semid, shared_memory* shm) {
    struct sembuf sb;

    while (1) {
        // Wait before consuming (P operation on the semaphore)
        sb.sem_num = 0;
        sb.sem_op = -1;  // Decrement the semaphore
        sb.sem_flg = 0;
        semop(semid, &sb, 1);

        // Check the class in the buffer
        if (shm->class == class) {
            printf("Consumer of class %d consumed: %s\n", class, shm->buffer);
            shm->class = 0; // Reset the class after consumption
        }

        // Signal after consumption (V operation on the semaphore)
        sb.sem_op = 1;  // Increment the semaphore
        semop(semid, &sb, 1);

        sleep(1); // To slow down consumption
    }
}

int main() {
    int shmid, semid;
    shared_memory *shm;

    // Create shared memory
    shmid = shmget(IPC_PRIVATE, sizeof(shared_memory), IPC_CREAT | 0644);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach to shared memory
    shm = shmat(shmid, NULL, 0);
    if (shm == (void *) -1) {
        perror("shmat");
        exit(1);
    }
    shm->class = 0; // Initialize the class

    // Create semaphore
    semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0644);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    // Initialize semaphore
    if (semctl(semid, 0, SETVAL, 1) == -1) {
        perror("semctl");
        exit(1);
    }

    // Create producer and consumer processes
    for (int i = 1; i <= 4; i++) {
        if (fork() == 0) {
            // Processes P1 and P2 belong to class 1, P3 and P4 to class 2
            producer(i <= 2 ? 1 : 2, semid, shm);
            exit(0);
        }
    }

    for (int i = 1; i <= 2; i++) {
        if (fork() == 0) {
            consumer(i, semid, shm); // Consumers C1 and C2
            exit(0);
        }
    }

    // Wait for the end of child processes
    for (int i = 0; i < 6; i++) {
        wait(NULL);
    }

    // Cleanup
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID, NULL);

    return 0;
}
