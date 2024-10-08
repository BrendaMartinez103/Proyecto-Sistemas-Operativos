#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Declaración de semáforos
sem_t ruedas_armadas;
sem_t chasis_listo;
sem_t motor_listo;
sem_t pintura_hecha;
sem_t equipamiento_listo;
pthread_mutex_t mutex_pintura; // Para evitar que ambos pintores pinten la misma moto

void* operario_armar_ruedas(void* arg) {
    while (1) {
        printf("Operario 1: Armando rueda 1\n");
        sleep(1); // Simula el tiempo de armar la rueda
        printf("Operario 1: Armando rueda 2\n");
        sleep(1); // Simula el tiempo de armar la rueda

        printf("Operario 1: Dos ruedas armadas\n");
        sem_post(&ruedas_armadas); // Indica que las ruedas están listas
        sem_wait(&equipamiento_listo); // Espera que la moto completa para empezar la siguiente
    }
}

void* operario_armar_chasis(void* arg) {
    while (1) {
        sem_wait(&ruedas_armadas); // Espera a que las ruedas estén listas
        printf("Operario 2: Armando chasis\n");
        sleep(2); // Simula el tiempo de armar el chasis
        sem_post(&chasis_listo); // Indica que el chasis está listo
    }
}

void* operario_agregar_motor(void* arg) {
    while (1) {
        sem_wait(&chasis_listo); // Espera a que el chasis esté listo
        printf("Operario 3: Agregando motor\n");
        sleep(2); // Simula el tiempo de agregar el motor
        sem_post(&motor_listo); // Indica que el motor está listo
    }
}

void* operario_pintar_rojo(void* arg) {
    while (1) {
        sem_wait(&motor_listo); // Espera a que el motor esté listo
        pthread_mutex_lock(&mutex_pintura); // Mutex para que solo un pintor pinte
        printf("Operario 4: Pintando moto de rojo\n");
        sleep(2); // Simula el tiempo de pintar la moto de rojo
        pthread_mutex_unlock(&mutex_pintura); // Libera el mutex de pintura
        sem_post(&pintura_hecha); // Indica que la pintura está hecha
    }
}

void* operario_pintar_verde(void* arg) {
    while (1) {
        sem_wait(&motor_listo); // Espera a que el motor esté listo
        pthread_mutex_lock(&mutex_pintura); // Mutex para que solo un pintor pinte
        printf("Operario 5: Pintando moto de verde\n");
        sleep(2); // Simula el tiempo de pintar la moto de verde
        pthread_mutex_unlock(&mutex_pintura); // Libera el mutex de pintura
        sem_post(&pintura_hecha); // Indica que la pintura está hecha
    }
}

void* operario_agregar_equipamiento(void* arg) {
    while (1) {
        sem_wait(&pintura_hecha); // Espera a que la moto esté pintada
        int agregar = rand() % 2; // Decisión aleatoria para agregar equipamiento
        if (agregar) {
            printf("Operario 6: Agregando equipamiento extra\n");
            sleep(2); // Simula el tiempo de agregar el equipamiento extra
        } else {
            printf("Operario 6: No se agrega equipamiento extra\n");
        }
        sem_post(&equipamiento_listo); // Indica que la moto está completamente lista
    }
}

int main() {
    pthread_t operarios[6]; // Hilos para los seis operarios

    // Inicializar semáforos
    sem_init(&ruedas_armadas, 0, 0);
    sem_init(&chasis_listo, 0, 0);
    sem_init(&motor_listo, 0, 0);
    sem_init(&pintura_hecha, 0, 0);
    sem_init(&equipamiento_listo, 0, 1); // Inicia en 1 para que comience el proceso

    // Inicializar mutex
    pthread_mutex_init(&mutex_pintura, NULL);

    // Crear hilos para los operarios
    pthread_create(&operarios[0], NULL, operario_armar_ruedas, NULL);
    pthread_create(&operarios[1], NULL, operario_armar_chasis, NULL);
    pthread_create(&operarios[2], NULL, operario_agregar_motor, NULL);
    pthread_create(&operarios[3], NULL, operario_pintar_rojo, NULL);
    pthread_create(&operarios[4], NULL, operario_pintar_verde, NULL);
    pthread_create(&operarios[5], NULL, operario_agregar_equipamiento, NULL);

    // Esperar a que los hilos terminen (no ocurre en este ejemplo, ya que cicla indefinidamente)
    for (int i = 0; i < 6; i++) {
        pthread_join(operarios[i], NULL);
    }

    // Destruir semáforos y mutex
    sem_destroy(&ruedas_armadas);
    sem_destroy(&chasis_listo);
    sem_destroy(&motor_listo);
    sem_destroy(&pintura_hecha);
    sem_destroy(&equipamiento_listo);
    pthread_mutex_destroy(&mutex_pintura);

    return 0;
}
