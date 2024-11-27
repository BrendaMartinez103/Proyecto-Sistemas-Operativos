#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <time.h>
#include <wait.h>

#define KEY ((key_t)(1243))
#define PRIORIDAD_VIP 1
#define PRIORIDAD_COMUN 2
#define TIPO_HAMBURGUESA 3
#define TIPO_VEGANO 4
#define TIPO_PAPAS 5


#define CANT_CLIENTES 20
#define PRIMER_CLIENTE 100

typedef struct {
    long tipo;
    int es_vip;       // 0 = normal , 1 = VIP
    int tipo_pedido;  // 0 = hamburguesa, 1 = vegano, 2 = papas
    int id_client;    // ID del cliente
} mensaje;

int queueID;
int msgSize = sizeof(mensaje) - sizeof(long);

void EmpleadoHambuguesa() {
    mensaje pedido;
    while (1) {
        msgrcv(queueID, &pedido, msgSize, TIPO_HAMBURGUESA, 0);
        printf("Cocinando hamburguesa (0) para cliente con ID: %i\n", pedido.id_client);
        sleep(1);
        printf("Hamburguesa lista para entregar.\n");
        pedido.tipo = pedido.id_client;
        msgsnd(queueID, &pedido, msgSize, 0);
    }
}

void EmpleadoVegano() {
    mensaje pedido;
    while (1) {
        msgrcv(queueID, &pedido, msgSize, TIPO_VEGANO, 0);
        printf("Cocinando menu vegano (1) para cliente con ID: %i\n", pedido.id_client);
        sleep(1);
        printf("Menu vegano listo para entregar.\n");
        pedido.tipo = pedido.id_client;
        msgsnd(queueID, &pedido, msgSize, 0);
    }
}

void EmpleadoPapa() {
    mensaje pedido;
    while (1) {
        msgrcv(queueID, &pedido, msgSize, TIPO_PAPAS, 0);
        printf("Cocinando papas fritas (2) para cliente con ID: %i\n", pedido.id_client);
        sleep(1);
        printf("Papas fritas listas para entregar\n");
        pedido.tipo = pedido.id_client;
        msgsnd(queueID, &pedido, msgSize, 0);
    }
}

void recibirPedido() {
    mensaje pedido;
    while (1) {
        // Prioridad para VIPs usando tipo negativo
        msgrcv(queueID, &pedido, msgSize, -PRIORIDAD_COMUN, 0);
        printf("Atendiendo cliente %s, con ID: %i, tipo pedido: %i.\n",
               (pedido.es_vip ? "VIP" : "Normal"), pedido.id_client, pedido.tipo_pedido);
        sleep(1);  
        pedido.tipo = pedido.tipo_pedido + TIPO_HAMBURGUESA;
        msgsnd(queueID, &pedido, msgSize, 0);
    }
}

void cliente(int id) {
    srand(getpid());
    mensaje pedido;
    while (1) {
        sleep(rand() % 5);
        
        pedido.es_vip = rand() % 2;
        pedido.tipo_pedido = rand() % 3;
        pedido.id_client = id;
        pedido.tipo = PRIORIDAD_COMUN - pedido.es_vip;

        printf("Llega cliente %s, id: %i y pide: %i.\n",
               (pedido.es_vip ? "VIP" : "Normal"), id, pedido.tipo_pedido);
    	int irse = rand() % 10 + 1;
        if (irse == 1) { 
            printf("Cliente %d se va de la cola.\n", id);
        }
        msgsnd(queueID, &pedido, msgSize, 0);//enviar pedido
        msgrcv(queueID, &pedido, msgSize, id, 0);  // Espera su pedido
        printf("Se va cliente %s, id: %i con su pedido\n",
               (pedido.es_vip ? "VIP" : "Normal"), pedido.id_client);
    }
}

int main() {
    // Eliminamos la cola de mensajes si ya existe y la volvemos a crear
    queueID = msgget(KEY, IPC_CREAT | 0666);
    msgctl(queueID, IPC_RMID, NULL);
    queueID = msgget(KEY, IPC_CREAT | 0666);
    
    // Creación de empleados
    if (fork() == 0) {
        EmpleadoHambuguesa();
        exit(0);
    }
    if (fork() == 0) {
        EmpleadoVegano();
        exit(0);
    }
    if (fork() == 0) {
        EmpleadoPapa();
        exit(0);
    }
    if (fork() == 0) {
        EmpleadoPapa();
        exit(0);
    }
    if (fork() == 0) {
        recibirPedido();
        exit(0);
    }

    // Creación de clientes
    for (int i = 0; i < CANT_CLIENTES; i++) {
        if (fork() == 0) {
            cliente(PRIMER_CLIENTE + i);
        }
    }

    // Esperar a los clientes
    for (int i = 0; i < CANT_CLIENTES; i++) {
        wait(NULL);
    }

    // Esperar a los empleados
    for (int i = 0; i < 5; i++) {
        wait(NULL);
    }

    // Eliminar la cola
    msgctl(queueID, IPC_RMID, NULL);

    return 0;
}
