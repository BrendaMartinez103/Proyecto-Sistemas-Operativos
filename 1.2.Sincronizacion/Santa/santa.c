#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <semaphore.h>
#include <time.h>
#include <unistd.h> 

#define CANT_RENOS 9
#define CANT_ELFOS 9
#define CANT_ELFO_AYUDA 3

sem_t sem_santa;// Despierta a Santa Claus cuando se necesita su atención
sem_t sem_renos;//Cuenta cuántos renos han llegado
sem_t sem_elfos;//Cuenta cuántos elfos necesitan ayuda
sem_t sem_nueveRenos;//Sincroniza la salida de los renos tras ser atendidos
sem_t sem_tresElfos;//Sincroniza la salida de los elfos tras recibir ayuda
sem_t sem_grupoElfos;//Controla cuántos elfos pueden intentar formar un grupo para pedir ayuda

pthread_mutex_t mutexRenos, mutexElfos;

void *santa() { 
    while (1) { 
        sem_wait(&sem_santa);
        printf("Santa DESPIERTA \n");
        fflush(stdout);
        pthread_mutex_lock(&mutexRenos);
        if (sem_trywait(&sem_renos) == -1) { // llegaron los 9 renos
            printf("Santa atiende a los renos\n");
            fflush(stdout);

            for (int i = 0; i < CANT_RENOS; i++) {
                sem_post(&sem_renos); 
                sem_post(&sem_nueveRenos);
              
            }
            sleep(1);
            printf("Santa terminó de enganchar a los renos\n");
            fflush(stdout);
        }
        else 
              sem_post(&sem_renos); //Si no todos los renos llegaron, se devuelve el del trywait
        pthread_mutex_unlock(&mutexRenos);

        pthread_mutex_lock(&mutexElfos);
        if (sem_trywait(&sem_elfos) == -1) { // 3 elfos necesitan ayuda
            printf("Santa ayuda a los elfos\n");
            fflush(stdout);

            for (int j = 0; j < CANT_ELFO_AYUDA; j++) {
                sem_post(&sem_tresElfos);
                sem_post(&sem_elfos); 
            }
            sleep(1);
            printf("Santa terminó de ayudar a los elfos\n");
            fflush(stdout);
        }
        else
         sem_post(&sem_elfos); //si no son 3 elfos se devuelve el del trywait
        pthread_mutex_unlock(&mutexElfos);

        printf("Santa se duerme\n");
    } 
    pthread_exit(NULL);
}
void *reno(void* p) { 
 
    while (1) { 
        int id = *(int*)p;
        srand(time(NULL));
        sleep(1);
       
        pthread_mutex_lock(&mutexRenos);
        sem_wait(&sem_renos);//Cada vez que un reno llega se decreta el semaforo
        
        if (sem_trywait(&sem_renos)==0){ 
            sem_post(&sem_renos); //devuelve el del semtrywait
            printf("Llego reno %i \n",id);
            fflush(stdout);
        }
        else{//si semrenos=0 retorna -1 , soy el ultimo reno y despierto a santa 
            printf("Llego reno %i y es el ultimo de los 9.\n",id);
            sem_post(&sem_santa); 
        }
        pthread_mutex_unlock(&mutexRenos);
        sem_wait(&sem_nueveRenos);//espera hasta que los 9 renos hayan sido enganchados al trineo
    }
    pthread_exit(NULL);
} 

void *elfo(void* p) { 
        while (1) { 
            int id = *(int*)p;
            srand(time(NULL));
            sleep(1);
            printf("Elfo %i Trabajando....\n",id);
            fflush(stdout);
            sem_wait(&sem_grupoElfos);
            pthread_mutex_lock(&mutexElfos);
            sem_wait(&sem_elfos);
           
            if (sem_trywait(&sem_elfos)==0){ 
                sem_post(&sem_elfos); //devuelve el del semtrywait
                 printf("Elfo %i tiene problema\n",id);
                 fflush(stdout);
            }
            else{//si sem_elfos=0 retorna -1 , somos 3 elfos y despierto a santa 
                printf("Elfo %i tiene problema,Tres elfos piden ayuda a santa\n",id);
                fflush(stdout);
                sem_post(&sem_santa); 
            }
               
            pthread_mutex_unlock(&mutexElfos);
            
            sem_wait(&sem_tresElfos);//El elfo espera hasta que tres elfos hayan sido ayudados por santa
            
            pthread_mutex_lock(&mutexElfos);
            sem_wait(&sem_elfos);
            if (sem_trywait(&sem_elfos)==0){ 
                 printf("Elfo %i recibio la ayuda de santa\n",id);
                 fflush(stdout);
                sem_post(&sem_elfos); //devuelve el del semtrywait
               
                 
            }
            else{
                printf("Elfo %i recibio la ayuda de santa ULTIMO\n",id);
                fflush(stdout);
                for (int i=0;i<CANT_ELFO_AYUDA;i++){
                    sem_post(&sem_elfos);
                    sem_post(&sem_grupoElfos); 
                }
            }
            pthread_mutex_unlock(&mutexElfos);
               
        }
   	pthread_exit(NULL);
}



int main() { 
    pthread_t threadRenos[CANT_RENOS], threadElfos[CANT_ELFOS], threadSanta;
    srand(time(NULL));
    pthread_mutex_init(&mutexRenos, NULL); 
    pthread_mutex_init(&mutexElfos, NULL);
    
    sem_init(&sem_santa, 0, 0); 
    sem_init(&sem_nueveRenos, 0, 0); 
    sem_init(&sem_tresElfos, 0, 0); 
    sem_init(&sem_renos, 0, CANT_RENOS); 
    sem_init(&sem_elfos, 0, CANT_ELFO_AYUDA); 
    sem_init(&sem_grupoElfos, 0, CANT_ELFO_AYUDA); 
    
    int* ids_renos[CANT_RENOS];
    int* ids_elfos[CANT_ELFOS];
  
    for(int i = 0; i < CANT_RENOS; i++){
         ids_renos[i] = malloc(sizeof(int));
        *ids_renos[i] = i + 1;
        pthread_create(&threadRenos[i], NULL, reno, ids_renos[i]);
    }
    for(int i = 0; i < CANT_ELFOS; i++){
         ids_elfos[i] = malloc(sizeof(int));
        *ids_elfos[i] = i + 1;
        pthread_create(&threadElfos[i], NULL, elfo,  ids_elfos[i]);
    }
    
    pthread_create(&threadSanta, NULL, santa, NULL);


    for(int i = 0; i < CANT_RENOS; i++){
        pthread_join(threadRenos[i], NULL);
        free(ids_renos[i]);
    }
    for(int i = 0; i < CANT_ELFOS; i++){
        pthread_join(threadElfos[i], NULL);
         free(ids_elfos[i]);
    }
    
    pthread_join(threadSanta, NULL);

    pthread_mutex_destroy(&mutexElfos);
    pthread_mutex_destroy(&mutexRenos);
    
    sem_destroy(&sem_santa); 
    sem_destroy(&sem_grupoElfos);
    sem_destroy(&sem_nueveRenos); 
    sem_destroy(&sem_tresElfos); 
    sem_destroy(&sem_renos); 
    sem_destroy(&sem_elfos); 

    return 0;
} 