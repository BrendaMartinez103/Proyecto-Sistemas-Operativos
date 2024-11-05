#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>

#define CLIENTES_COMUNES 10
#define CLIENTES_VIP 3

typedef struct {
    int esVIP;     // 0 = no VIP, 1 = VIP
    int tipoPedido; // 0 = Hamburguesa, 1 = MenuVegano, 2 = Papas fritas
} Pedido;

// Pipes para la comunicación
int pipePedidoHamburguesa[2];
int pipePedidoPapas[2];
int pipePedidoVegano[2];
int pipeClienteComun[2];
int pipeClienteVIP[2];
int pipeRetiroHamburguesa[2];
int pipeRetiroPapas[2];
int pipeRetiroVegano[2];

int cocineroHamburguesas() {
    close(pipePedidoPapas[0]);
    close(pipePedidoPapas[1]);
    close(pipePedidoVegano[0]);
    close(pipePedidoVegano[1]);
    close(pipeClienteComun[0]);
    close(pipeClienteComun[1]);
    close(pipeClienteVIP[0]);
    close(pipeClienteVIP[1]);
    close(pipeRetiroPapas[0]);
    close(pipeRetiroPapas[1]);
    close(pipeRetiroVegano[0]);
    close(pipeRetiroVegano[1]);

    close(pipePedidoHamburguesa[1]);
    close(pipeRetiroHamburguesa[0]);

    while (1) {
        Pedido pedido;
        int respuesta;
        read(pipePedidoHamburguesa[0], &pedido, sizeof(pedido));
        printf("Haciendo Hamburguesa--> pedido tipo %d\n", pedido.tipoPedido);
        printf("Hamburguesa Lista\n");
        respuesta = 0;
        write(pipeRetiroHamburguesa[1], &respuesta, sizeof(respuesta));
    }
    exit(0);
}



void cocineroVegano() {
    close(pipePedidoHamburguesa[0]);
    close(pipePedidoHamburguesa[1]);
    close(pipePedidoPapas[0]);
    close(pipePedidoPapas[1]);
    close(pipeClienteComun[0]);
    close(pipeClienteComun[1]);
    close(pipeClienteVIP[0]);
    close(pipeClienteVIP[1]);
    close(pipeRetiroHamburguesa[0]);
    close(pipeRetiroHamburguesa[1]);
    close(pipeRetiroPapas[0]);
    close(pipeRetiroPapas[1]);

    close(pipePedidoVegano[1]);
    close(pipeRetiroVegano[0]);

    while (1) {
        Pedido pedido;
        int respuesta=1;
        read(pipePedidoVegano[0], &pedido, sizeof(pedido));
        printf("Haciendo menu vegano--> pedido tipo  %d\n", pedido.tipoPedido);
        printf("Menu vegano listo\n");
        write(pipeRetiroVegano[1], &respuesta, sizeof(respuesta));
    }
    exit(0);
}
void cocineroPapas(int id) {
    close(pipePedidoHamburguesa[0]);
    close(pipePedidoHamburguesa[1]);
    close(pipePedidoVegano[0]);
    close(pipePedidoVegano[1]);
    close(pipeClienteComun[0]);
    close(pipeClienteComun[1]);
    close(pipeClienteVIP[0]);
    close(pipeClienteVIP[1]);
    close(pipeRetiroHamburguesa[0]);
    close(pipeRetiroHamburguesa[1]);
    close(pipeRetiroVegano[0]);
    close(pipeRetiroVegano[1]);

    close(pipePedidoPapas[1]);
    close(pipeRetiroPapas[0]);

    while (1) {
        Pedido pedido;
        int respuesta=2;
        read(pipePedidoPapas[0], &pedido, sizeof(pedido));
        printf("Haciendo papas,Empleado %d --> pedido tipo %d\n", id, pedido.tipoPedido);
        printf("Papas Listas, Empleado %d\n", id);
        write(pipeRetiroPapas[1], &respuesta, sizeof(respuesta));
    }
    exit(0);
}
void recibirPedido() {
    close(pipeRetiroHamburguesa[0]);
    close(pipeRetiroHamburguesa[1]);
    close(pipeRetiroPapas[0]);
    close(pipeRetiroPapas[1]);
    close(pipeRetiroVegano[0]);
    close(pipeRetiroVegano[1]);

    close(pipeClienteComun[1]);
    close(pipeClienteVIP[1]);

    close(pipePedidoHamburguesa[0]);
    close(pipePedidoPapas[0]);
    close(pipePedidoVegano[0]);

    while (1) {
        Pedido pedido;

        int flags = fcntl(pipeClienteVIP[0], F_GETFL, 0);
        fcntl(pipeClienteVIP[0], F_SETFL, flags | O_NONBLOCK);
        
        if (read(pipeClienteVIP[0], &pedido, sizeof(pedido)) > 0) {
            printf("Se recibió un pedido tipo %d de un cliente VIP\n", pedido.tipoPedido);
        } else {
            read(pipeClienteComun[0], &pedido, sizeof(pedido));
            printf("Se recibió un pedido tipo %d de un cliente común\n", pedido.tipoPedido);
        }

        switch (pedido.tipoPedido) {
            case 0:
                printf("se despacha pedido para cocinar hamburguesas\n");
                write(pipePedidoHamburguesa[1], &pedido, sizeof(pedido));
                break;
            case 1:
                printf("se despacha pedido para que uno de los empleados cocine papas\n");
                write(pipePedidoPapas[1], &pedido, sizeof(pedido));
                break;
            case 2:
                printf("se despacha pedido para cocinar menu vegano\n");
                write(pipePedidoVegano[1], &pedido, sizeof(pedido));
                break;
        }
    }
    exit(0);
}

void clienteComun(int id) {
    close(pipePedidoHamburguesa[0]);
    close(pipePedidoHamburguesa[1]);
    close(pipePedidoPapas[0]);
    close(pipePedidoPapas[1]);
    close(pipePedidoVegano[0]);
    close(pipePedidoVegano[1]);
    close(pipeClienteVIP[0]);
    close(pipeClienteVIP[1]);

    close(pipeClienteComun[0]);

    close(pipeRetiroHamburguesa[1]);
    close(pipeRetiroPapas[1]);
    close(pipeRetiroVegano[1]);

    srand(time(NULL) + getpid());
    printf("Cliente comun %d: llega\n", id);

    int irse = rand() % 10 + 1;

    if (irse == 1) {
        printf("Cliente comun %d se va porque hay mucha cola\n", id);
        return;
    } else {
        
        Pedido pedido = {0, rand() % 3};
        printf("Cliente comun  %d: hace pedido tipo %d\n", id, pedido.tipoPedido);
        write(pipeClienteComun[1], &pedido, sizeof(pedido));

        int respuesta;
        switch (pedido.tipoPedido) {
            case 0:
                read(pipeRetiroHamburguesa[0], &respuesta, sizeof(respuesta));
                printf("Cliente comun %d: recibió su pedido de tipo %d\n", id, respuesta);
                break;
            case 1:
                read(pipeRetiroVegano[0], &respuesta, sizeof(respuesta));
                printf("Cliente %d: recibió su pedido de tipo %d\n", id, respuesta);
                break;
            case 2:
                read(pipeRetiroPapas[0], &respuesta, sizeof(respuesta));
                printf("Cliente %d: recibió su pedido de tipo %d\n", id, respuesta);
                break;
            
        }
        printf("Cliente %d se va con su pedido.\n", id);
    }
    exit(0);
}

void clienteVIP(int id) {
    close(pipePedidoHamburguesa[0]);
    close(pipePedidoHamburguesa[1]);
    close(pipePedidoPapas[0]);
    close(pipePedidoPapas[1]);
    close(pipePedidoVegano[0]);
    close(pipePedidoVegano[1]);
    close(pipeClienteComun[0]);
    close(pipeClienteComun[1]);

    close(pipeClienteVIP[0]);

    close(pipeRetiroHamburguesa[1]);
    close(pipeRetiroPapas[1]);
    close(pipeRetiroVegano[1]);

    srand(time(NULL) + getpid());
    printf("Cliente VIP %d: llega\n", id);

    int irse = rand() % 10 + 1;

    if (irse == 1) {
        printf("Cliente VIP %d se va porque hay mucha cola\n", id);
        return;
    } else {
        Pedido pedido = {1, rand() % 3};  // Cliente VIP (VIP=1), tipo aleatorio (0, 1 o 2)
        printf("Cliente VIP %d: hace pedido tipo %d\n", id, pedido.tipoPedido);
        write(pipeClienteVIP[1], &pedido, sizeof(pedido));

        int respuesta;
        switch (pedido.tipoPedido) {
            case 0:
                read(pipeRetiroHamburguesa[0], &respuesta, sizeof(respuesta));
                printf("Cliente VIP %d: recibió su pedido de tipo  %d\n", id, respuesta);
                break;
            case 1:
                read(pipeRetiroVegano[0], &respuesta, sizeof(respuesta));
                printf("Cliente VIP %d: recibió su pedido de tipo  %d\n", id, respuesta);
                break;
             case 2:
                read(pipeRetiroPapas[0], &respuesta, sizeof(respuesta));
                printf("Cliente VIP %d: recibió su pedido de tipo %d\n", id, respuesta);
                break;
        }
        printf("Cliente VIP %d se va con su pedido\n", id);
    }
    exit(0);
}

int main() {
    pipe(pipePedidoHamburguesa);
    pipe(pipePedidoPapas);
    pipe(pipePedidoVegano);
    pipe(pipeClienteComun);
    pipe(pipeClienteVIP);
    pipe(pipeRetiroHamburguesa);
    pipe(pipeRetiroPapas);
    pipe(pipeRetiroVegano);

    if (fork() == 0) cocineroHamburguesas();
    if (fork() == 0) cocineroPapas(0);
    if (fork() == 0) cocineroPapas(1);
    if (fork() == 0) cocineroVegano();
    if (fork() == 0) recibirPedido();

    for (int i = 0; i < CLIENTES_COMUNES; i++) {
        if (fork() == 0) clienteComun(i);
    }

    for (int i = 0; i < CLIENTES_VIP; i++) {
        if (fork() == 0) clienteVIP(i);
    }

    for (int i = 0; i < (CLIENTES_COMUNES + CLIENTES_VIP + 5); i++) {
        wait(NULL);
    }

    printf("Termina la simulación\n");
    return 0;
}
