#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <wait.h>

#define CANT_CLIENTES 10

typedef struct{
    int es_vip;   //0 = comun , 1 = vip
    int tipo; //0 = hamburguesa, 1 = Vegano, 2 = Papas
} pedido;

//Declaraciones de pipes
int pipe_clienteCOMUN[2];
int pipe_clienteVIP[2];

int pipe_cola[2];

int pipe_orden_hamburguesa[2];
int pipe_orden_vegano[2];
int pipe_orden_papas[2];

int pipe_entrega_hamburguesa[2];
int pipe_entrega_vegano[2];
int pipe_entrega_papas[2];

void EmpleadoHambuguesa(){
    //cerrar pipes
    close(pipe_clienteCOMUN[0]);
    close(pipe_clienteCOMUN[1]);
    close(pipe_clienteVIP[0]);
    close(pipe_clienteVIP[1]);
    close(pipe_entrega_hamburguesa[0]);
    close(pipe_entrega_vegano[0]);
    close(pipe_entrega_vegano[1]);
    close(pipe_entrega_papas[0]);
    close(pipe_entrega_papas[1]);
    close(pipe_orden_hamburguesa[1]);
    close(pipe_orden_vegano[1]);
    close(pipe_orden_vegano[0]);
    close(pipe_orden_papas[0]);
    close(pipe_orden_papas[1]);
    close(pipe_cola[0]);
    close(pipe_cola[1]);

    int respuestaH = 1;
    int p;

    while(1){
        read(pipe_orden_hamburguesa[0],&p,sizeof(int));
        printf("Cocinando hamburguesa\n");
        write(pipe_entrega_hamburguesa[1],&respuestaH,sizeof(int));
        printf("Hamburguesa lista para entregar.\n");
    }


    exit(0);
}

void EmpleadoVegano(){
    //cerrar pipes
    close(pipe_clienteCOMUN[0]);
    close(pipe_clienteCOMUN[1]);
    close(pipe_clienteVIP[0]);
    close(pipe_clienteVIP[1]);
    close(pipe_entrega_hamburguesa[0]);
    close(pipe_entrega_hamburguesa[1]);
    close(pipe_entrega_vegano[0]);
    close(pipe_entrega_papas[0]);
    close(pipe_entrega_papas[1]);
    close(pipe_orden_hamburguesa[0]);
    close(pipe_orden_hamburguesa[1]);
    close(pipe_orden_vegano[1]);
    close(pipe_orden_papas[0]);
    close(pipe_orden_papas[1]);
    close(pipe_cola[0]);
    close(pipe_cola[1]);

    int respuestaV = 1;
    int p;

    while(1){
        read(pipe_orden_vegano[0],&p,sizeof(int));
        printf("Cocinando menu vegano\n");
      
        write(pipe_entrega_vegano[1],&respuestaV,sizeof(int));
        printf("Menu vegano listo para entrega.\n");
    }
    exit(0);
}

void EmpleadoPapa(){
    //cerrar pipes
    close(pipe_clienteCOMUN[0]);
    close(pipe_clienteCOMUN[1]);
    close(pipe_clienteVIP[0]);
    close(pipe_clienteVIP[1]);
    close(pipe_entrega_hamburguesa[0]);
    close(pipe_entrega_hamburguesa[1]);
    close(pipe_entrega_vegano[0]);
    close(pipe_entrega_vegano[1]);
    close(pipe_entrega_papas[0]);
    close(pipe_orden_hamburguesa[0]);
    close(pipe_orden_hamburguesa[1]);
    close(pipe_orden_vegano[1]);
    close(pipe_orden_vegano[0]);
    close(pipe_orden_papas[1]);
    close(pipe_cola[0]);
    close(pipe_cola[1]);

    int respuestaP = 1;
    int p;

    while(1){
        read(pipe_orden_papas[0],&p,sizeof(int));
        printf("Cocinando papas fritas\n");
     
        write(pipe_entrega_papas[1],&respuestaP,sizeof(int));
        printf("Papas fritas listas para entregar\n");
    }

    exit(0);
}

void recibirPedido(){
    //cerrar pipes
    close(pipe_clienteCOMUN[1]);
    close(pipe_clienteVIP[1]);
    close(pipe_entrega_hamburguesa[0]);
    close(pipe_entrega_hamburguesa[1]);
    close(pipe_entrega_vegano[0]);
    close(pipe_entrega_vegano[1]);
    close(pipe_entrega_papas[0]);
    close(pipe_entrega_papas[1]);
    close(pipe_orden_hamburguesa[0]);
    close(pipe_orden_papas[0]);
    close(pipe_orden_vegano[0]);
    close(pipe_cola[1]);

    int hambur,papa,veg = 1;
    pedido Pedido;
    int respuesta;

    int flags = fcntl(pipe_clienteVIP[0], F_GETFL, 0);//configuran el extremo de lectura del pipe para que sea no bloqueante,
    fcntl(pipe_clienteVIP[0], F_SETFL, flags | O_NONBLOCK);

    while(1){
        read(pipe_cola[0],&respuesta,sizeof(int));
        if(read(pipe_clienteVIP[0],&Pedido,sizeof(pedido))!=-1){
            printf("Atiendo cliente VIP \n");
             switch (Pedido.tipo) {
                case 0:
                    write(pipe_orden_hamburguesa[1],&hambur,sizeof(int));
                     break;
                case 1:
                    write(pipe_orden_vegano[1],&veg,sizeof(int));
                     break;
                case 2:
                    write(pipe_orden_papas[1],&papa,sizeof(int));
                     break;
                 }
        }
        else{
            read(pipe_clienteCOMUN[0],&Pedido,sizeof(pedido));
            printf("Atiendo cliente no VIP \n");
            switch (Pedido.tipo) {
                case 0:
                write(pipe_orden_hamburguesa[1],&hambur,sizeof(int));
                 break;
                case 1:
                write(pipe_orden_vegano[1],&veg,sizeof(int));
                 break;
                case 2:
                write(pipe_orden_papas[1],&papa,sizeof(int));
                 break;
            }
            
        }
    }
    exit(0);
}

void cliente(int id){
   
    close(pipe_clienteCOMUN[0]);
    close(pipe_clienteVIP[0]);
    close(pipe_cola[0]);
     
    close(pipe_entrega_hamburguesa[1]);
    close(pipe_entrega_papas[1]);
    close(pipe_entrega_vegano[1]);
    
    close(pipe_orden_hamburguesa[0]);
    close(pipe_orden_papas[0]);
    close(pipe_orden_vegano[0]);
    close(pipe_orden_hamburguesa[1]);
    close(pipe_orden_papas[1]);
    close(pipe_orden_vegano[1]);
    
    srand(time(NULL) + getpid());
    int despacho;//para almacenar la confirmación de que el pedido fue recibido.
    int r = 1; //se usa para indicar que el cliente está en la cola 
    int irse=2; //para que usuarios que se quieran ir.
    
    while(1){
        sleep(1);
        int irse = rand() % 10 + 1;
        if (irse == 1) {
        printf("Cliente %i llega y se va porque hay mucha cola\n", id);
        return;
        }
        int vip= rand()%2;
        int p= rand()%3;
        pedido ped= {vip,p};  
        printf("Llega cliente %i, id: %i y pide: %i.\n",vip,id,ped.tipo);
        if(ped.es_vip==0)
            write(pipe_clienteCOMUN[1],&ped,sizeof(ped));
        else
            write(pipe_clienteVIP[1],&ped,sizeof(ped));
        
        write(pipe_cola[1],&r,sizeof(p));
        if(ped.tipo == 0)
            read(pipe_entrega_hamburguesa[0],&despacho,sizeof(int));
        else if(ped.tipo == 1)
            read(pipe_entrega_vegano[0],&despacho,sizeof(int));
        else
            read(pipe_entrega_papas[0],&despacho,sizeof(int));
        printf("Cliente %i, id: %i recibio su pedido Y SE VA.\n",vip,id);
    }

    exit(0);
}

int main(int argc, char **argv){
//crea pipes
    if(pipe(pipe_clienteCOMUN)==-1) return 1;
    if(pipe(pipe_entrega_hamburguesa)==-1) return 1;
    if(pipe(pipe_entrega_papas)==-1) return 1;
    if(pipe(pipe_entrega_vegano)==-1) return 1;
    if(pipe(pipe_orden_hamburguesa)==-1) return 1;
    if(pipe(pipe_orden_papas)==-1) return 1;
    if(pipe(pipe_orden_vegano)==-1) return 1;
    if(pipe(pipe_clienteVIP)==-1) return 1;
    if(pipe(pipe_cola)==-1) return 1;
    
   //procesos hijos de los empleado
    if(fork()==0)
	    EmpleadoHambuguesa();
	else if(fork()==0)
	    EmpleadoVegano();
	else if(fork()==0)
	    EmpleadoPapa();
    else if(fork()==0)
	    EmpleadoPapa();
	else if(fork()==0)
	    recibirPedido();
  //procesos hijos de los clientes
    for(int i = 0; i<CANT_CLIENTES; i++){
	if(fork()==0)
	    cliente(i+1);
    }
//esperar a que cada proceso cliente termine 
    for(int i = 0; i<CANT_CLIENTES; i++)
	    wait(NULL);
    
//esperar a que cada proceso empleado termine    
    for(int i = 0; i<5; i++)
	    wait(NULL);
    
    return 0;
}