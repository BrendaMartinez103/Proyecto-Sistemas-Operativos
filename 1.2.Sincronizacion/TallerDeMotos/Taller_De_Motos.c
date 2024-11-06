#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

sem_t ruedas_armadas;
sem_t chasis_listo;
sem_t motor_listo;
sem_t pintura_hecha;
sem_t equipamiento_listo;
pthread_mutex_t mutex_pintura; 

void* operario_armar_ruedas() {
    while (1) {
        sem_wait(&equipamiento_listo); 
        printf("Operario 1: Armando rueda 1\n");
        sleep(1); 
        printf("Operario 1: Armando rueda 2\n");
        sleep(1); 
        printf("Operario 1: Dos ruedas armadas\n");
        sem_post(&ruedas_armadas); 
       
    }
}

void* operario_armar_chasis() {
    while (1) {
        sem_wait(&ruedas_armadas); 
        printf("Operario 2: Armando chasis\n");
        sleep(1); 
        sem_post(&chasis_listo); 
    }
}

void* operario_agregar_motor() {
    while (1) {
        sem_wait(&chasis_listo); 
        printf("Operario 3: Agregando motor\n");
        sleep(1); 
        sem_post(&motor_listo); 
    }
}

void* operario_pintar_rojo() {
    while (1) {
        sem_wait(&motor_listo); 
        pthread_mutex_lock(&mutex_pintura); 
        printf("Operario 4: Pintando moto de rojo\n");
        sleep(1); 
        pthread_mutex_unlock(&mutex_pintura); 
        sem_post(&pintura_hecha); // Indica que la pintura está hecha
    }
}

void* operario_pintar_verde() {
    while (1) {
        sem_wait(&motor_listo); 
        pthread_mutex_lock(&mutex_pintura); 
        printf("Operario 5: Pintando moto de verde\n");
        sleep(1); 
        pthread_mutex_unlock(&mutex_pintura); 
        sem_post(&pintura_hecha); 
    }
}

void* operario_agregar_equipamiento() {
    while (1) {
        sem_wait(&pintura_hecha); 
        int agregar = rand() % 2; 
        if (agregar) {
            printf("Operario 6: Agregando equipamiento extra\n");
            sleep(1);
        } else {
            printf("Operario 6: No se agrega equipamiento extra\n");
        }
        sem_post(&equipamiento_listo); 
    }
}

int main() {
    pthread_t operarios[6]; 

   
    sem_init(&ruedas_armadas, 0, 0);
    sem_init(&chasis_listo, 0, 0);
    sem_init(&motor_listo, 0, 0);
    sem_init(&pintura_hecha, 0, 0);
    sem_init(&equipamiento_listo, 0, 1); 

    pthread_mutex_init(&mutex_pintura, NULL);
    
    pthread_create(&operarios[0], NULL, operario_armar_ruedas, NULL);
    pthread_create(&operarios[1], NULL, operario_armar_chasis, NULL);
    pthread_create(&operarios[2], NULL, operario_agregar_motor, NULL);
    pthread_create(&operarios[3], NULL, operario_pintar_rojo, NULL);
    pthread_create(&operarios[4], NULL, operario_pintar_verde, NULL);
    pthread_create(&operarios[5], NULL, operario_agregar_equipamiento, NULL);

    for (int i = 0; i < 6; i++) {
        pthread_join(operarios[i], NULL);
    }

    sem_destroy(&ruedas_armadas);
    sem_destroy(&chasis_listo);
    sem_destroy(&motor_listo);
    sem_destroy(&pintura_hecha);
    sem_destroy(&equipamiento_listo);
    pthread_mutex_destroy(&mutex_pintura);

    return 0;
}
