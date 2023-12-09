 for (int i = 1; i <= 4; i++) {
     if (fork() == 0) {
         producer(i <= 2 ? 1 : 2, semid, shm);
         exit(0);
     }
   }

 for (int i = 1; i <= 2; i++) {
     if (fork() == 0) {
         consumer(i, semid, shm);
         exit(0);
     }
 }
