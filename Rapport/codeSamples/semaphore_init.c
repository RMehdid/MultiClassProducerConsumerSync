 semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0644);
 if (semid == -1) {
     perror("semget");
     exit(1);
 }

 // Initialize semaphore value
 if (semctl(semid, 0, SETVAL, 1) == -1) {
     perror("semctl");
     exit(1);
 }
