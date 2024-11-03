#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <semaphore.h>
#include <time.h>
#include <unistd.h> 



sem_t sem_santa,sem_renos, sem_elfos, sem_nueveRenos, sem_tresElfos; 


void *santa(void *arg) { 
    while (1) { 
        sem_wait(&sem_santa);
        printf("Santa DESPIERTA \n");
        fflush(stdout);
        if(sem_trywait(&sem_nueveRenos) == 0){
            printf("Santa atiende a los renos\n");
            fflush(stdout);
             for (int i = 0; i < 9; i++){
                sem_post(&sem_renos); 
             }
            printf("Santa terminó de enganchar a los renos y se duerme \n");
        }
        else {
            if(sem_trywait(&sem_tresElfos) == 0){ 
                for (int j = 0; j < 3; j++){
                    sem_post(&sem_elfos); 
                    printf("Santa ayuda al elfo %i \n",j+1);
                    fflush(stdout);
                }
               printf("Santa terminó de ayudar y se duerme \n");
             
            }
        }
    } 
} 

void *reno() { 
    while (1) { 
       
        for (int i = 0; i < 9; i++){
            sem_wait(&sem_renos);
            printf("Llego reno %i\n", i + 1);
            fflush(stdout);
        }
        sem_post(&sem_santa);
        sem_post(&sem_nueveRenos); 
    
    }
} 

void *elfo() { 
    while (1) { 
 
        int r = rand() % 3 + 4;
        printf("Elfo %i Trabajando sin problemas\n",r);
        fflush(stdout);
        for (int j = 0; j < 3; j++){
            sem_wait(&sem_elfos);
            printf("Elfo %i tiene problema\n", j + 1);
            fflush(stdout);
        }
        printf("Tres elfos piden ayuda a santa\n");
        sem_post(&sem_santa);
        sem_post(&sem_tresElfos); 
       
    }
}


int main() { 
    pthread_t threads[3];
    
    sem_init(&sem_santa, 0, 0); 
    sem_init(&sem_nueveRenos, 0, 0); 
    sem_init(&sem_tresElfos, 0, 0); 
    sem_init(&sem_renos, 0, 9); 
    sem_init(&sem_elfos, 0, 3); 
   
    
    pthread_create(&threads[0], NULL, reno, NULL);
    pthread_create(&threads[1], NULL, elfo, NULL);
    pthread_create(&threads[2], NULL, santa, NULL);

    for (int i = 0; i < 3; i++)
    {
        pthread_join(threads[i], NULL);
    }
     
    sem_destroy(&sem_santa); 
    sem_destroy(&sem_nueveRenos); 
    sem_destroy(&sem_tresElfos); 
    sem_destroy(&sem_renos); 
    sem_destroy(&sem_elfos); 

    return 0;
} 