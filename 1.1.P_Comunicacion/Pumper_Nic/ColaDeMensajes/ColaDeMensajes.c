#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <time.h>
#include <wait.h>
#include <errno.h>

#define KEY ((key_t)(1243))
#define PRIORIDAD_VIP 1
#define PRIORIDAD_COMUN 2
#define TIPO_HAMBURGUESA 3
#define TIPO_VEGANO 4
#define TIPO_PAPAS 5

#define CANT_CLIENTES 20

typedef struct {
	long tipo;
	int es_vip;         // 0 = normal , 1 = VIP
	int tipo_pedido;       
	int id_client;  // Identificador Unico para cada cliente
}mensaje;

int queueID;
int msgSize = sizeof(mensaje) - sizeof(long);

void EmpleadoHambuguesa() {
    while(1){
        mensaje pedido;
        msgrcv(queueID, &pedido,msgSize,TIPO_HAMBURGUESA, 0);
        printf("Cocinando hamburguesa (3) para cliente con ID: %i\n",pedido.id_client);
        sleep(1);
        printf("Hamburguesa lista para entregar.\n");
        pedido.tipo = pedido.id_client;
        msgsnd(queueID, &pedido, msgSize, 0);
    }
}

void EmpleadoVegano() {
	while (1) {
		mensaje pedido;
		msgrcv(queueID, &pedido, msgSize,TIPO_VEGANO, 0);
		printf("Cocinando menu vegano (4) para cliente con ID: %i\n",pedido.id_client);
		sleep(1);
		printf("Menu vegano listo para entrega.\n");
		pedido.tipo = pedido.id_client;
		msgsnd(queueID, &pedido, msgSize, 0);
	}
}


void EmpleadoPapa() {
	while (1) {
		mensaje pedido;
		msgrcv(queueID, &pedido, msgSize, TIPO_PAPAS, 0);
		printf("Cocinando papas fritas (5) para cliente con ID: %i\n",pedido.id_client);
		sleep(1);
		printf("Papas fritas listas para entregar\n");
		pedido.tipo = pedido.id_client;
		msgsnd(queueID, &pedido, msgSize, 0);
	}
}


void recibirPedido() {
	while (1) {
		mensaje pedido;
        msgrcv(queueID, &pedido,msgSize, -PRIORIDAD_COMUN, 0);
        char* tipoCliente;
		if(pedido.es_vip == 1)
			tipoCliente = "VIP";
		else
			tipoCliente = "Normal";
		printf("Atendiendo cliente %s, con ID: %i, tipo pedido: %i.\n", tipoCliente,pedido.id_client, pedido.tipo_pedido);
        sleep(1);
        pedido.tipo = pedido.tipo_pedido;
        msgsnd(queueID, &pedido,msgSize, 0);
    }
}

void cliente(int id) {
    srand(time(NULL)+getpid());
    mensaje pedido;
	sleep(rand()%10);
    pedido.es_vip = rand() % 2;
    char* tipoCliente;
	if(pedido.es_vip == 1)
		tipoCliente = "VIP";
	else
		tipoCliente = "Normal";
    pedido.tipo_pedido = (rand() % 3) + 3;
    pedido.id_client = id;
    pedido.tipo = pedido.es_vip;
    printf("Llega cliente, %s, id: %i y pide: %i.\n",tipoCliente,id,pedido.tipo_pedido);
     int irse = rand() % 10 + 1;
        if (irse == 1) {
        printf("Cliente %d se va de la cola \n",id);
        exit(1);
    }
   /* while (msgsnd(queueID, &pedido,msgSize, IPC_NOWAIT ) == -1) {
	    if (errno == EAGAIN) {  // Cola llena
                printf("Cliente %d se va de la cola \n",id);
                sleep(1);  // Esperar 1 segundo antes de reintentar
                exit(0);
	    } else {
                perror("Error al enviar el mensaje");
                exit(0);
        }
	}*/

    msgsnd(queueID, &pedido,msgSize, 0);
    msgrcv(queueID, &pedido, msgSize, id, 0);
    printf("Se va cliente %s, id: %i.\n",tipoCliente,pedido.id_client);
}
int main(int argc, char **argv) {
	// Eliminamos la cola de mensajes si ya existe y la volvemos a crear
	queueID = msgget(KEY, IPC_CREAT | 0666);
	msgctl(queueID, IPC_RMID, NULL);
	queueID = msgget(KEY, IPC_CREAT | 0666);

    pid_t pid_hamburguesa = fork();
    if(pid_hamburguesa == 0){
        EmpleadoHambuguesa();
        exit(0);
    }

    pid_t pid_vegano = fork();
    if(pid_vegano == 0){
        EmpleadoVegano();
        exit(0);
    }

    pid_t pid_papas1 = fork();
    if(pid_papas1 == 0){
        EmpleadoPapa();
        exit(0);
    }

    pid_t pid_papas2 = fork();
    if(pid_papas2 == 0){
        EmpleadoPapa();
        exit(0);
    }

    pid_t pid_recibir = fork();
    if(pid_recibir == 0){
        recibirPedido();
        exit(0);
    }

    for(int i = 0; i < CANT_CLIENTES; i++){
        pid_t pid_cliente = fork();
        if(pid_cliente == 0){
            cliente(i+1);
            exit(0);
        }
    }


	// Wait for all clients
	for (int i = 0; i < CANT_CLIENTES + 5; i++) {
		wait(NULL);
	}

	printf("Local vacio \n");

	// Destroy message queue
	msgctl(queueID, IPC_RMID, NULL);

	return 0;
}
