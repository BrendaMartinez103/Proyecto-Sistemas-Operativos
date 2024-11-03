#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>

#define MAX_PEDIDOS 100
#define TIPO_HAMBURGUESA 1
#define TIPO_VEGANO 2
#define TIPO_PAPAS 3
#define PRIORIDAD_VIP 1
#define PRIORIDAD_NORMAL 2

// Estructura para los pedidos
struct mensaje_pedido {
    int id_pedido;       // Identificador único del pedido
    int tipo_combo;      // Tipo de comida
    int es_vip;          // Flag para clientes VIP
};

// Función para manejo de errores
void error(const char* msg) {
    perror(msg);
    exit(1);
}

// Función para el empleado de hamburguesas
void empleado_hamburguesas(int read_pipe[2], int write_pipe[2]) {
    struct mensaje_pedido pedido;
    close(read_pipe[1]);  // Cerrar el extremo de escritura del pipe de lectura
    close(write_pipe[0]); // Cerrar el extremo de lectura del pipe de escritura

    printf("Empleado de hamburguesas iniciado\n");
    
    while (1) {
        // Leer pedidos de hamburguesa
        if (read(read_pipe[0], &pedido, sizeof(struct mensaje_pedido)) > 0) {
            if (pedido.tipo_combo == TIPO_HAMBURGUESA) {
                printf("Preparando hamburguesa #%d\n", pedido.id_pedido);
                sleep(2); // Simular tiempo de preparación

                // Enviar pedido listo
                if (write(write_pipe[1], &pedido, sizeof(struct mensaje_pedido)) == -1) {
                    error("Error enviando hamburguesa lista");
                }
                
                printf("Hamburguesa #%d lista\n", pedido.id_pedido);
            }
        }
    }
}

// Función para el empleado de menú vegano
void empleado_vegano(int read_pipe[2], int write_pipe[2]) {
    struct mensaje_pedido pedido;
    close(read_pipe[1]);
    close(write_pipe[0]);

    printf("Empleado de menú vegano iniciado\n");
    
    while (1) {
        if (read(read_pipe[0], &pedido, sizeof(struct mensaje_pedido)) > 0) {
            if (pedido.tipo_combo == TIPO_VEGANO) {
                printf("Preparando menú vegano #%d\n", pedido.id_pedido);
                sleep(3); // Simular tiempo de preparación

                if (write(write_pipe[1], &pedido, sizeof(struct mensaje_pedido)) == -1) {
                    error("Error enviando menú vegano listo");
                }
                
                printf("Menú vegano #%d listo\n", pedido.id_pedido);
            }
        }
    }
}

// Función para el empleado de papas
void empleado_papas(int read_pipe[2], int write_pipe[2]) {
    struct mensaje_pedido pedido;
    close(read_pipe[1]);
    close(write_pipe[0]);

    printf("Empleado de papas iniciado\n");
    
    while (1) {
        if (read(read_pipe[0], &pedido, sizeof(struct mensaje_pedido)) > 0) {
            if (pedido.tipo_combo == TIPO_PAPAS) {
                printf("Preparando papas #%d\n", pedido.id_pedido);
                sleep(1); // Simular tiempo de preparación

                if (write(write_pipe[1], &pedido, sizeof(struct mensaje_pedido)) == -1) {
                    error("Error enviando papas listas");
                }
                
                printf("Papas #%d listas\n", pedido.id_pedido);
            }
        }
    }
}

// Función para el despachador
void despachador(int read_pipe[2]) {
    struct mensaje_pedido pedido;
    close(read_pipe[1]);

    printf("Despachador iniciado\n");
    
    while (1) {
        if (read(read_pipe[0], &pedido, sizeof(struct mensaje_pedido)) > 0) {
            printf("Pedido #%d entregado al cliente\n", pedido.id_pedido);
        }
    }
}

int main() {
    int pipe_pedidos[2];   // Pipe para enviar pedidos a los empleados
    int pipe_listos[2];    // Pipe para enviar pedidos listos al despachador

    // Crear pipes
    if (pipe(pipe_pedidos) == -1 || pipe(pipe_listos) == -1) {
        error("Error creando pipes");
    }

    // Crear procesos para los empleados
    if (fork() == 0) {
        empleado_hamburguesas(pipe_pedidos, pipe_listos);
        exit(0);
    }
    
    if (fork() == 0) {
        empleado_vegano(pipe_pedidos, pipe_listos);
        exit(0);
    }
    
    if (fork() == 0) {
        empleado_papas(pipe_pedidos, pipe_listos);
        exit(0);
    }
    
    if (fork() == 0) {
        empleado_papas(pipe_pedidos, pipe_listos); // Segundo empleado de papas
        exit(0);
    }
    
    if (fork() == 0) {
        despachador(pipe_listos);
        exit(0);
    }
    
    // Proceso principal: simular llegada de clientes
    close(pipe_pedidos[0]); // Cerrar el extremo de lectura del pipe de pedidos
    close(pipe_listos[1]);  // Cerrar el extremo de escritura del pipe de listos

    struct mensaje_pedido nuevo_pedido;
    int id_pedido = 1;
    
    printf("Sistema iniciado.\n");
    
    while (1) {
        nuevo_pedido.id_pedido = id_pedido++;
        nuevo_pedido.es_vip = (rand() % 5 == 0); // 20% de probabilidad de ser VIP

        // Seleccionar tipo de pedido aleatoriamente
        nuevo_pedido.tipo_combo = rand() % 3 + 1;

        // Enviar pedido al pipe de empleados
        if (write(pipe_pedidos[1], &nuevo_pedido, sizeof(struct mensaje_pedido)) == -1) {
            error("Error enviando nuevo pedido");
        }

        printf("Nuevo pedido #%d generado (%s)\n", 
               nuevo_pedido.id_pedido,
               nuevo_pedido.es_vip ? "VIP" : "Normal");
        
        sleep(rand() % 3 + 1); // Esperar entre 1 y 3 segundos entre pedidos
    }

    return 0;
}
