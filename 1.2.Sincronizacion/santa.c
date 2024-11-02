#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <semaphore.h>
#include <time.h>
#include <unistd.h> 

#define CANT_RENOS 9
#define CANT_ELFOS 3

sem_t sem_santa,sem_renos, sem_elfos, sem_nueveRenos, sem_tresElfos; 

pthread_mutex_t mutexRenos, mutexElfos;

void *santa(void *arg) { 
    while (1) { 
        sem_wait(&sem_santa);
        printf("Santa DESPIERTA \n");
        if(sem_trywait(&sem_nueveRenos) == 0){
            printf("Santa atiende a los renos\n");
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

void *reno(void *arg) { 
    while (1) { 
        pthread_mutex_lock(&mutexRenos);
        for (int i = 0; i < 9; i++){
            sem_wait(&sem_renos);
            printf("Llego reno %i\n", i + 1);
            fflush(stdout);
        }
        sem_post(&sem_santa);
        sem_post(&sem_nueveRenos); 
        pthread_mutex_unlock(&mutexRenos);
    }
} 

void *elfo(void *arg) { 
    while (1) { 
        pthread_mutex_lock(&mutexElfos);
        int r = rand() % 3 + 4;
        printf("Elfo %i Trabajando sin problemas\n",r);
        for (int j = 0; j < 3; j++){
            sem_wait(&sem_elfos);
            printf("Elfo %i tiene problema\n", j + 1);
             fflush(stdout);
        }
        printf("Tres elfos piden ayuda a santa\n");
        sem_post(&sem_santa);
        sem_post(&sem_tresElfos); 
        pthread_mutex_unlock(&mutexElfos);
    }
}


int main() { 
    pthread_t threadRenos[CANT_RENOS], threadElfos[CANT_ELFOS], threadSanta;
    
    pthread_mutex_init(&mutexRenos, NULL); 
    pthread_mutex_init(&mutexElfos, NULL);
    
    sem_init(&sem_santa, 0, 0); 
    sem_init(&sem_nueveRenos, 0, 0); 
    sem_init(&sem_tresElfos, 0, 0); 
    sem_init(&sem_renos, 0, 9); 
    sem_init(&sem_elfos, 0, 3); 
   
    
    for(int i = 0; i < CANT_RENOS; i++){
        pthread_create(&threadRenos[i], NULL, reno, NULL);
    }
    for(int i = 0; i < CANT_ELFOS; i++){
        pthread_create(&threadElfos[i], NULL, elfo, NULL);
    }
    pthread_create(&threadSanta, NULL, santa, NULL);


    for(int i = 0; i < CANT_RENOS; i++){
        pthread_join(threadRenos[i], NULL);
    }
    for(int i = 0; i < CANT_ELFOS; i++){
        pthread_join(threadElfos[i], NULL);
    }
    pthread_join(threadSanta, NULL);

    pthread_mutex_destroy(&mutexElfos);
    pthread_mutex_destroy(&mutexRenos);
    
    sem_destroy(&sem_santa); 
    sem_destroy(&sem_nueveRenos); 
    sem_destroy(&sem_tresElfos); 
    sem_destroy(&sem_renos); 
    sem_destroy(&sem_elfos); 

    return 0;
} 