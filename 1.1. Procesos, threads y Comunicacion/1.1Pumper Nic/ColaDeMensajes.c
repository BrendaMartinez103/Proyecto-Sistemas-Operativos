#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#define MAX_PEDIDOS 100
#define TIPO_HAMBURGUESA 1
#define TIPO_VEGANO 2
#define TIPO_PAPAS 3
#define PRIORIDAD_VIP 1
#define PRIORIDAD_NORMAL 2

// Estructura para los mensajes
struct mensaje_pedido {
    long mtype;           // Tipo de mensaje (usado para prioridad)
    int id_pedido;       // Identificador único del pedido
    int tipo_combo;      // Tipo de comida
    int es_vip;          // Flag para clientes VIP
}mensaje;

// Funciones auxiliares
void error(const char* msg) {
    perror(msg);
    exit(1);
}

// Función para el empleado de hamburguesas
void empleado_hamburguesas(int cola_pedidos, int cola_listos) {
    struct mensaje_pedido pedido;
    
    printf("Empleado de hamburguesas iniciado\n");
    
    while (1) {
        // Recibir pedidos de tipo hamburguesa
        if (msgrcv(cola_pedidos, &pedido, sizeof(struct mensaje_pedido) - sizeof(long),
                   TIPO_HAMBURGUESA, 0) == -1) {
            error("Error recibiendo pedido de hamburguesa");
        }
        
        printf("Preparando hamburguesa #%d\n", pedido.id_pedido);
        sleep(2); // Simular tiempo de preparación
        
        // Enviar pedido listo
        pedido.mtype = 1;
        
        if (msgsnd(cola_listos, &pedido, sizeof(struct mensaje_pedido) - sizeof(long), 0) == -1) {
            error("Error enviando hamburguesa lista");
        }
        
        printf("Hamburguesa #%d lista\n", pedido.id_pedido);
    }
}

// Función para el empleado de menú vegano
void empleado_vegano(int cola_pedidos, int cola_listos) {
    struct mensaje_pedido pedido;
    
    printf("Empleado de menú vegano iniciado\n");
    
    while (1) {
        if (msgrcv(cola_pedidos, &pedido, sizeof(struct mensaje_pedido) - sizeof(long),
                   TIPO_VEGANO, 0) == -1) {
            error("Error recibiendo pedido vegano");
        }
        
        printf("Preparando menú vegano #%d\n", pedido.id_pedido);
        sleep(3); // Simular tiempo de preparación
        
        pedido.mtype = 1;
        
        if (msgsnd(cola_listos, &pedido, sizeof(struct mensaje_pedido) - sizeof(long), 0) == -1) {
            error("Error enviando menú vegano listo");
        }
        
        printf("Menú vegano #%d listo\n", pedido.id_pedido);
    }
}

// Función para el empleado de papas
void empleado_papas(int cola_pedidos, int cola_listos) {
    struct mensaje_pedido pedido;
    
    printf("Empleado de papas iniciado\n");
    
    while (1) {
        if (msgrcv(cola_pedidos, &pedido, sizeof(struct mensaje_pedido) - sizeof(long),
                   TIPO_PAPAS, 0) == -1) {
            error("Error recibiendo pedido de papas");
        }
        
        printf("Preparando papas #%d\n", pedido.id_pedido);
        sleep(1); // Simular tiempo de preparación
        
        pedido.mtype = 1;
        
        if (msgsnd(cola_listos, &pedido, sizeof(struct mensaje_pedido) - sizeof(long), 0) == -1) {
            error("Error enviando papas listas");
        }
        
        printf("Papas #%d listas\n", pedido.id_pedido);
    }
}

// Función para el despachador
void despachador(int cola_listos) {
    struct mensaje_pedido pedido_listo;
    
    printf("Despachador iniciado\n");
    
    while (1) {
        if (msgrcv(cola_listos, &pedido_listo, sizeof(struct mensaje_pedido) - sizeof(long),
                   0, 0) == -1) {
            error("Error recibiendo pedido listo");
        }
        
        printf("Pedido #%d entregado al cliente\n", pedido_listo.id_pedido);
    }
}

int main() {
    key_t key_pedidos = ftok(".", 'A');
    key_t key_listos = ftok(".", 'B');
    
    // Crear colas de mensajes
    int cola_pedidos = msgget(key_pedidos, IPC_CREAT | 0666);
    int cola_listos = msgget(key_listos, IPC_CREAT | 0666);
    
    if (cola_pedidos == -1 || cola_listos == -1) {
        error("Error creando colas de mensajes");
    }
    
    // Crear procesos para los empleados
    if (fork() == 0) {
        empleado_hamburguesas(cola_pedidos, cola_listos);
        exit(0);
    }
    
    if (fork() == 0) {
        empleado_vegano(cola_pedidos, cola_listos);
        exit(0);
    }
    
    if (fork() == 0) {
        empleado_papas(cola_pedidos, cola_listos);
        exit(0);
    }
    
    if (fork() == 0) {
        empleado_papas(cola_pedidos, cola_listos); // Segundo empleado de papas
        exit(0);
    }
    
    if (fork() == 0) {
        despachador(cola_listos);
        exit(0);
    }
    
    // Proceso principal: simular llegada de clientes
    struct mensaje_pedido nuevo_pedido;
    int id_pedido = 1;
    
    printf("Sistema iniciado. Presione Ctrl+C para terminar.\n");
    
    while (1) {
        nuevo_pedido.id_pedido = id_pedido++;
        nuevo_pedido.es_vip = (rand() % 5 == 0); // 20% de probabilidad de ser VIP
        
        // Seleccionar tipo de pedido aleatoriamente
        int tipo = rand() % 3 + 1;
        nuevo_pedido.tipo_combo = tipo;
        nuevo_pedido.mtype = nuevo_pedido.es_vip ? PRIORIDAD_VIP : PRIORIDAD_NORMAL;
        
        if (msgsnd(cola_pedidos, &nuevo_pedido, sizeof(struct mensaje_pedido) - sizeof(long), 0) == -1) {
            error("Error enviando nuevo pedido");
        }
        
        printf("Nuevo pedido #%d generado (%s)\n", 
               nuevo_pedido.id_pedido,
               nuevo_pedido.es_vip ? "VIP" : "Normal");
        
        sleep(rand() % 3 + 1); // Esperar entre 1 y 3 segundos entre pedidos
    }
    
    return 0;
}