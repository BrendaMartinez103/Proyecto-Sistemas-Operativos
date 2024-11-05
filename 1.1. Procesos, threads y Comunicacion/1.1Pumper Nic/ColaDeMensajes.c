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

#define CANT_CLIENTES 10

typedef struct {
    long tipo;
    int es_vip;         // 0 = no VIP, 1 = VIP
    int tipo_pedido;       // 0 = Hamburguesa, 1 = MenuVegano, 2 = Papas fritas
    int id_client;  // Identificador único para cada cliente
} mensaje;
  int queueID;
int msgSize = sizeof(mensaje) - sizeof(long);
void EmpleadoHambuguesa() {
    mensaje pedido;
    while (1) {
        msgrcv(queueID, &pedido,msgSize, TIPO_HAMBURGUESA, 0);
        printf("Cocinando hamburguesa\n");
        pedido.tipo = pedido.id_client;
        printf("Hamburguesa lista para entregar.\n");
        msgsnd(queueID, &pedido, msgSize, 0);
    }
}

void EmpleadoVegano() {

    mensaje pedido;
    while (1) {
        msgrcv(queueID, &pedido,msgSize,TIPO_VEGANO, 0);
        printf("Cocinando menu vegano\n");
        pedido.tipo = pedido.id_client;
        printf("Menu vegano listo para entrega.\n");
        msgsnd(queueID, &pedido, msgSize, 0);
    }
}

void EmpleadoPapa() {

    mensaje pedido;
    while (1) {
        msgrcv(queueID, &pedido, msgSize, TIPO_PAPAS, 0);
        printf("Cocinando papas fritas\n");
        pedido.tipo = pedido.id_client;
        printf("Papas fritas listas para entregar\n");
        msgsnd(queueID, &pedido,msgSize, 0);
    }
}

void recibirPedido() {

    mensaje pedido;

    while (1) {
        // Prioridad para VIPs usando tipo negativo
        msgrcv(queueID, &pedido,msgSize, -PRIORIDAD_COMUN, 0); 
        printf("Atendiendo cliente %i, con ID: %i, tipo pedido: %i.\n", pedido.es_vip,pedido.id_client,  pedido.tipo_pedido);
        pedido.tipo = pedido.tipo_pedido + TIPO_HAMBURGUESA; // Los tipos van desde HAMBURGUESA hasta PAPAS
        msgsnd(queueID, &pedido,msgSize, 0);
    }
}

void cliente(int id) {

     srand(getpid());
     mensaje pedido;
    while(1){
        sleep(rand()%10);
        pedido.es_vip = rand() % 2;
        pedido.tipo_pedido = rand() % 3;
        pedido.id_client = id;
        pedido.tipo = pedido.es_vip ? PRIORIDAD_VIP : PRIORIDAD_COMUN;  // VIPs get lower type value
        printf("Llega cliente, %i, id: %i y pide: %i.\n",pedido.es_vip,id,pedido.tipo_pedido);

        msgsnd(queueID, &pedido,msgSize, 0);
        
        // Wait for order to be ready, matching by ID

        msgrcv(queueID, &pedido, msgSize, id, 0);
        printf("Se va cliente %i, id: %i.\n",pedido.es_vip,pedido.id_client);
      
    }
    printf("Un cliente se va porque hay mucha fila.\n");
    exit(0);
}

int main(int argc, char **argv) {
    // Destroy any existing message queue and create a new one
    queueID = msgget(KEY, IPC_CREAT | 0666);
    msgctl(queueID, IPC_RMID, NULL);
    queueID = msgget(KEY, IPC_CREAT | 0666);
    
    // Create employees
    if (fork() == 0)
        EmpleadoHambuguesa();
    else if (fork() == 0)
        EmpleadoVegano();
    else if (fork() == 0)
        EmpleadoPapa();
    else if (fork() == 0)
        EmpleadoPapa();
    else if (fork() == 0)
        recibirPedido();

    // Create clients
    for (int i = 0; i < CANT_CLIENTES; i++) {
        if (fork() == 0)
            cliente(i+1);
    }

    // Wait for all clients
    for (int i = 0; i < CANT_CLIENTES+5; i++){
        wait(NULL);
}

    // Destroy message queue
    msgctl(queueID, IPC_RMID, NULL);
    
    return 0;
}
