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
sem_t inicio;

void* operario_armar_ruedas() {
	while(1){
		sem_wait(&inicio);
		sem_wait(&ruedas_armadas);
		printf("Operario 1: Armando rueda \n");
		fflush(stdout);
	   	sleep(1);
		sem_post(&chasis_listo);
	}
}

void* operario_armar_chasis() {
	while(1){
		sem_wait(&chasis_listo);
		sem_wait(&chasis_listo);
		printf("Operario 2: Armando chasis\n");
		fflush(stdout);
		sleep(1);
		sem_post(&motor_listo);
	}
}

void* operario_agregar_motor() {
	while(1){
		sem_wait(&motor_listo);
		printf("Operario 3: Agregando motor\n");
		fflush(stdout);
		sleep(1);
		sem_post(&pintura_hecha);
	}
}

void* operario_pintar_rojo() {
	while(1){
		sem_wait(&pintura_hecha);
		printf("Operario 4: Pintando moto de rojo\n");
     	fflush(stdout);
		sleep(1);
		sem_post(&equipamiento_listo);
		sem_post(&ruedas_armadas);
		sem_post(&ruedas_armadas);
	}
}

void* operario_pintar_verde() {
	while(1){
		sem_wait(&pintura_hecha);
		printf("Operario 5: Pintando moto de verde\n");
    	fflush(stdout);
        sleep(1);
		sem_post(&equipamiento_listo);
		sem_post(&ruedas_armadas);
		sem_post(&ruedas_armadas);
	}
}


void* operario_agregar_equipamiento() {
	while(1){
		sem_wait(&equipamiento_listo);
		sem_wait(&equipamiento_listo);		
	    printf("Operario 6: Agregando equipamiento extra\n");
    	fflush(stdout);
    	sleep(1);
		sem_post(&inicio);
		sem_post(&inicio);
		sem_post(&inicio);
		sem_post(&inicio);
	}
}
int main() {
    pthread_t operarios[6]; 
   
    sem_init(&ruedas_armadas, 0, 2);
    sem_init(&chasis_listo, 0, 0);
    sem_init(&motor_listo, 0, 0);
    sem_init(&pintura_hecha, 0, 0);
    sem_init(&equipamiento_listo, 0, 0); 
    sem_init(&inicio,0,4);
    
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
    sem_destroy(&inicio);

    return 0;
}