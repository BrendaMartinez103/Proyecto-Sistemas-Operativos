#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>


void main(int argc, char *argv[]){
	char dir[100];
if(argv[1]==NULL)
		printf("Falta un introducir un parametro. \n");
	else {

	strncpy(dir, argv[1], sizeof(dir));
	int resultado;
	resultado = mkdir(dir, 0777);
	if(resultado == 0)
		printf("Directorio creado correctamente");
	else
		perror("Error al crear el directorio");
}
}
