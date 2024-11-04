#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <wait.h>

#define CANT_CLIENTES 20

typedef struct{
    int VIP;   //0 = no VIP, 1 = VIP
    int tipo; //0 = Hamburguesa, 1 = MenuVegano, 2 = Papas fritas
} pedido;

int buffer;

//Declaraciones de pipes
int pipe_entrega_hamburguesa[2];
int pipe_entrega_vegano[2];
int pipe_entrega_papas[2];

int pipe_orden_hamburguesa[2];
int pipe_orden_vegano[2];
int pipe_orden_papas[2];

int pipe_clienteVIP[2];
int pipe_clienteCOMUN[2];
int pipe_cola[2];

void EmpleadoHambuguesa(){
    //cerrar pipes
    close(pipe_entrega_hamburguesa[0]);
    close(pipe_entrega_vegano[0]);
    close(pipe_entrega_papas[0]);
    close(pipe_entrega_vegano[1]);
    close(pipe_entrega_papas[1]);
    close(pipe_orden_hamburguesa[1]);
    close(pipe_orden_vegano[1]);
    close(pipe_orden_papas[1]);
    close(pipe_orden_vegano[0]);
    close(pipe_orden_papas[0]);

    close(pipe_clienteVIP[0]);
    close(pipe_clienteCOMUN[1]);
    close(pipe_clienteVIP[0]);
    close(pipe_clienteCOMUN[1]);
    close(pipe_cola[0]);
    close(pipe_cola[1]);
    
    while(1){
      
                read(pipe_orden_hamburguesa[0],&buffer,sizeof(buffer));
                printf("Preparando hamburguesa\n");
                write(pipe_entrega_hamburguesa[1],&buffer,sizeof(buffer));
                printf("Hamburguesa lista\n");
    }
        
    close(pipe_orden_hamburguesa[0]);
    close(pipe_entrega_hamburguesa[1]);
 

    exit(0);
}
void EmpleadoVegano(){
    //cerrar pipes
    close(pipe_entrega_hamburguesa[0]);
    close(pipe_entrega_hamburguesa[1]);
    close(pipe_entrega_vegano[0]);
    close(pipe_entrega_papas[0]);
    close(pipe_entrega_papas[1]);
    
 
    close(pipe_orden_hamburguesa[1]);
    close(pipe_orden_vegano[1]);
    close(pipe_orden_papas[1]);
    close(pipe_orden_hamburguesa[0]);
    close(pipe_orden_papas[0]);

    close(pipe_clienteVIP[0]);
    close(pipe_clienteCOMUN[1]);
    close(pipe_clienteVIP[0]);
    close(pipe_clienteCOMUN[1]);
    close(pipe_cola[0]);
    close(pipe_cola[1]);
    
    while(1){
        
            read(pipe_orden_vegano[0],&buffer,sizeof(buffer));
            printf("Preparando menu vegano\n");
            write(pipe_entrega_vegano[1],&buffer,sizeof(int));
            printf("Menu vegano listo.\n");
         
    }
    close(pipe_orden_vegano[0]);
    close(pipe_entrega_vegano[1]);       
    exit(0);
}
void EmpleadoPapas(){
    //cerrar pipes
    close(pipe_entrega_hamburguesa[0]);
     close(pipe_entrega_hamburguesa[1]);
    close(pipe_entrega_vegano[0]);
     close(pipe_entrega_vegano[1]);
    close(pipe_entrega_papas[0]);
   
    close(pipe_orden_hamburguesa[1]);
    close(pipe_orden_vegano[1]);
    close(pipe_orden_papas[1]);
    close(pipe_orden_hamburguesa[0]);
    close(pipe_orden_vegano[0]);

    close(pipe_clienteVIP[0]);
    close(pipe_clienteCOMUN[1]);
    close(pipe_clienteVIP[0]);
    close(pipe_clienteCOMUN[1]);
    close(pipe_cola[0]);
    close(pipe_cola[1]);
    
    while(1){
            read(pipe_orden_papas[0],&buffer,sizeof(buffer));
            printf("Preparando papas fritas\n");
            write(pipe_entrega_papas[1],&buffer,sizeof(buffer));
            printf("Papas fritas listas.\n");
         
    }
     close(pipe_orden_papas[0]);
    close(pipe_entrega_papas[1]);       
         
    }
        

void recibirPedido(){
    //cerrar pipes
    close(pipe_clienteVIP[1]);
    close(pipe_clienteCOMUN[1]);
    
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
    
    fcntl(pipe_clienteCOMUN[0], F_SETFL,O_NONBLOCK);
    fcntl(pipe_clienteVIP[0], F_SETFL,O_NONBLOCK);
    
    pedido p;
    while(1){
        read(pipe_cola[0],&buffer,sizeof(buffer));
        //atiende a los vip
        if(read(pipe_clienteVIP[0],&buffer,sizeof(buffer))!=-1){
            printf("Atendiendo a un cliente VIP \n");
            if (p.tipo == 0)
                write(pipe_orden_hamburguesa[1],&buffer,sizeof(buffer));
            else 
            if(p.tipo == 1)
                write(pipe_orden_vegano[1],&buffer,sizeof(buffer));
            else
                write(pipe_orden_papas[1],&buffer,sizeof(buffer));
        }
        else {
            if(read(pipe_clienteCOMUN[0],&buffer,sizeof(buffer))!=-1){
                printf("Atendiendo a cliente comun\n");
                if (p.tipo == 0)
                    write(pipe_orden_hamburguesa[1],&buffer,sizeof(buffer));
                else 
                   if(p.tipo == 1)
                    write(pipe_orden_vegano[1],&buffer,sizeof(buffer));
                else
                    write(pipe_orden_papas[1],&buffer,sizeof(buffer));
            }
        }
    }

    close(pipe_cola[0]);
    close(pipe_clienteCOMUN[0]);
    close(pipe_clienteVIP[0]);
    close(pipe_orden_hamburguesa[1]);
    close(pipe_orden_vegano[1]);
    close(pipe_orden_papas[1]);

    exit(0);
}

void cliente(){
    //cerrar pipes
    close(pipe_clienteVIP[0]);
    close(pipe_clienteCOMUN[0]);
    close(pipe_cola[0]);
    close(pipe_entrega_hamburguesa[1]);
    close(pipe_entrega_vegano[1]);
    close(pipe_entrega_papas[1]);
    close(pipe_orden_hamburguesa[0]);
    close(pipe_orden_papas[0]);
    close(pipe_orden_vegano[0]);
    close(pipe_orden_hamburguesa[1]);
    close(pipe_orden_papas[1]);
    close(pipe_orden_vegano[1]);

    srand(getpid());

    int despacho;
 
    pedido cliente;

    //La condicion del while simula que el cliente se vaya si hay mucha fila
    while(rand()%10!=9){
        sleep(rand()%10);
        cliente.VIP = rand()%2;
        cliente.tipo = rand()%3;
        printf("Llega un cliente vip VIP: %i, y su pedido es: %i.\n",cliente.VIP,cliente.tipo);
        if(cliente.VIP==0)
            write(pipe_clienteCOMUN[1],&cliente,sizeof(pedido));
        else
            write(pipe_clienteVIP[1],&cliente,sizeof(pedido));
            
        write(pipe_cola[1],&buffer,sizeof(buffer));
        
        if(cliente.tipo == 0)
            read(pipe_entrega_hamburguesa[0],&buffer,sizeof(buffer));
        else if(cliente.tipo == 1)
            read(pipe_entrega_vegano[0],&buffer,sizeof(buffer));
        else
            read(pipe_entrega_papas[0],&buffer,sizeof(buffer));
        printf("Se va cliente, VIP: %i, con su pedido : %i.\n",cliente.VIP,cliente.tipo);
    }
    printf("Un cliente se va porque hay mucha fila\n");

    close(pipe_clienteVIP[1]);
    close(pipe_clienteCOMUN[1]);
    close(pipe_cola[1]);
    close(pipe_entrega_hamburguesa[0]);
    close(pipe_entrega_vegano[0]);
    close(pipe_entrega_papas[0]);

    exit(0);
}

int main(int argc, char **argv){
    //crear pipes
    if(pipe(pipe_clienteCOMUN)==-1) return 1;
    if(pipe(pipe_clienteVIP)==-1) return 1;
    if(pipe(pipe_entrega_hamburguesa)==-1) return 1;
    if(pipe(pipe_entrega_papas)==-1) return 1;
    if(pipe(pipe_entrega_vegano)==-1) return 1;
    if(pipe(pipe_orden_hamburguesa)==-1) return 1;
    if(pipe(pipe_orden_papas)==-1) return 1;
    if(pipe(pipe_orden_vegano)==-1) return 1;
    if(pipe(pipe_cola)==-1) return 1;
    
    //crear procesos empleados
    if(fork()==0)
	    EmpleadoHambuguesa();
	else if(fork()==0)
	    EmpleadoVegano();
	else if(fork()==0)
	    EmpleadoPapas();
    else if(fork()==0)
	    EmpleadoPapas();
	else if(fork()==0)
	    recibirPedido();
    
    //crear procesos clientes
    for(int i = 0; i<CANT_CLIENTES; i++){
	if(fork()==0)
	    cliente();
    }

    //espero que se vayan los clientes
    for(int i = 0; i<CANT_CLIENTES; i++)
	    wait(NULL);
    
    //termino los procesos empleados
    for(int i = 0; i<5; i++)
	    wait(NULL);//Falta matar a todos los hijos
    
    return 0;
}