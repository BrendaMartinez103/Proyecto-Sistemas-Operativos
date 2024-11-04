#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void main(int argc, char *argv[]){
	char dir[100];
    if(argv[1]==NULL)
		printf("Falta un introducir un parametro.\n");
	else {

    	strncpy(dir, argv[1], sizeof(dir));
    	int resultado;
    	resultado =rmdir(argv[1]);
        if (resultado == 0)
        	printf("Directorio eliminado correctamente");
        else
			perror("Error al eliminar el directorio \n \n");
    }
}
