#include <stdio.h>
#include <string.h>

void main(int argc, char *argv[]){
	char file[100];
	if(argv[1] == NULL)
        printf("Falta un introducir un parametro.\n");
	else {
		strncpy(file, argv[1], sizeof(file));
		FILE *f;
		f = fopen(file, "w+");
		if(f != NULL)
			printf("Archivo creado correctamente\n");
		else
			perror("Error al crear el Archivo");
		fclose(f);
	}
}
