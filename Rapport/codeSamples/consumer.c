 void consumer(int class, int semid, shared_memory* shm) {
     struct sembuf sb;

     while (1) {
         sb.sem_num = 0;
         sb.sem_op = -1;
         sb.sem_flg = 0;
         semop(semid, &sb, 1);

         // Consumer logic...

         sb.sem_op = 1;
         semop(semid, &sb, 1);

         sleep(1); // Introduce delay to simulate message consumption
     }
 }
