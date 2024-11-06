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
#define MENULISTO 1000

#define CANT_CLIENTES 20

typedef struct {
	long tipo;
	int es_vip;         // 0 = normal , 1 = VIP
	long mtype;       // 0 = Hamburguesa, 1 = MenuVegano, 2 = Papas fritas
	int id_client;  // Identificador Unico para cada cliente
} mensaje;
int queueID;
int msgSize = sizeof(mensaje) - sizeof(long);

void EmpleadoHambuguesa() {
	while (1) {
		mensaje pedido;
		msgrcv(queueID, &pedido, msgSize, TIPO_HAMBURGUESA, 0);
		printf("Cocinando hamburguesa (0) para cliente con ID: %i\n",pedido.id_client-100);
		printf("Hamburguesa lista para entregar.\n");
		pedido.tipo = MENULISTO;
		msgsnd(queueID, &pedido, msgSize, 0);
	}
}

void EmpleadoVegano() {
	while (1) {
		mensaje pedido;
		msgrcv(queueID, &pedido, msgSize,TIPO_VEGANO, 0);
		printf("Cocinando menu vegano (1) para cliente con ID: %i\n",pedido.id_client-100);
		printf("Menu vegano listo para entrega.\n");
		pedido.tipo = MENULISTO;
		msgsnd(queueID, &pedido, msgSize, 0);
	}
}

void EmpleadoPapa() {
	while (1) {
		mensaje pedido;
		msgrcv(queueID, &pedido, msgSize, TIPO_PAPAS, 0);
		printf("Cocinando papas fritas (2) para cliente con ID: %i\n",pedido.id_client-100);
		printf("Papas fritas listas para entregar\n");
		pedido.tipo = MENULISTO;
		msgsnd(queueID, &pedido, msgSize, 0);
	}
}

void recibirPedido() {
	while (1) {
		mensaje pedido;
		// Prioridad para VIPs usando tipo negativo
		// Toma el primer mensaje <= a 2, es decir los VIP.
		msgrcv(queueID, &pedido,msgSize, -PRIORIDAD_COMUN, 0);
		//Swap para prioridades
		long mtypeAux = pedido.mtype;
		long tipoAux = pedido.tipo;
		pedido.mtype = tipoAux;
		pedido.tipo = mtypeAux;

		char* tipoCliente;
		if(pedido.es_vip == 1)
			tipoCliente = "VIP";
		else
			tipoCliente = "Normal";

		printf("Atendiendo cliente %s, con ID: %i, tipo pedido: %ld.\n", tipoCliente,pedido.id_client-100,  pedido.mtype);

		pedido.tipo = pedido.mtype + 3; 
		msgsnd(queueID, &pedido,msgSize, 0);

		msgrcv(queueID, &pedido, msgSize, MENULISTO, 0); //1000 corresponde a pedido finalizado y listo para entregar
		printf("Pedido entregado a cliente %d.\n", pedido.id_client-100);
		pedido.tipo = pedido.id_client;
		msgsnd(queueID, &pedido,msgSize, 0);
	}
}

void cliente(int id) {

	srand(getpid());
	mensaje pedido;
	sleep(rand()%10);
	pedido.es_vip = rand() % 2;
	char* tipoCliente;
	if(pedido.es_vip == 1)
		tipoCliente = "VIP";
	else
		tipoCliente = "Normal";

	pedido.tipo = rand() % 3;
	pedido.id_client = id+100;
	pedido.mtype = pedido.es_vip ? PRIORIDAD_VIP : PRIORIDAD_COMUN;  // VIPs get lower type value
	printf("Llega cliente, %s, id: %i y pide: %ld.\n",tipoCliente,id,pedido.mtype);
	//Swap para prioridades
	long mtypeAux = pedido.mtype;
	long tipoAux = pedido.tipo;
	pedido.mtype = tipoAux;
	pedido.tipo = mtypeAux;
    int irse = rand() % 10 + 1;
        if (irse == 1) {
        printf("Cliente %d se va de la cola-------------------------------------------------\n",id);
        exit(1);
    }
    int k=1;
	if (msgsnd(queueID, &pedido,msgSize, IPC_NOWAIT ) == -1) {
	    if (errno == EAGAIN) {  // Cola llena
                printf("Cola llena, reintentando...\n");
                sleep(1);  // Esperar 1 segundo antes de reintentar
                exit(0);
	    } else {
                perror("Error al enviar el mensaje");
                exit(0);
        }
	}
    else {
    
    }

	msgrcv(queueID, &pedido, msgSize, id+100, 0);
	pedido.tipo = 1000000;
	printf("Se va cliente %s, id: %i.\n",tipoCliente,pedido.id_client-100);

	exit(0);
}

int main(int argc, char **argv) {
	// Eliminamos la cola de mensajes si ya existe y la volvemos a crear
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
	for (int i = 0; i < CANT_CLIENTES+5; i++) {
		wait(NULL);
	}

	printf("Local vacio \n");

	// Destroy message queue
	msgctl(queueID, IPC_RMID, NULL);

	return 0;
}
