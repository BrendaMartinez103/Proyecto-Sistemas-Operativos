#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define MAXIMO 500

void main(int argc, char *argv[]){
	char file[100];
 if(argv[1]==NULL)
        printf("Falta un introducir un parametro.\n");
	else {
	strncpy(file, argv[1], sizeof(file));
	FILE *f;
	f = fopen(file, "w+");
	if(f != NULL)
		printf("Archivo creado correctamente");
	else
		perror("Error al crear el Archivo");
	fclose(f);
}
}
