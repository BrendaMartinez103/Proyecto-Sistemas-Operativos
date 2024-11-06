
#include <stdio.h>
#include <string.h>

#define MAXIMO 500

void main(int argc, char *argv[]){
	char dir[100];
    if(argv[1]==NULL)
        printf("Falta un introducir un parametro. \n");
    else {

	strncpy(dir, argv[1], sizeof(dir));
	//se abre el archivo en modo lectura
	FILE *f = fopen(dir, "r");
	char lee[MAXIMO];
	if (f != NULL){
		//mientras se pueda leer del archivo imprimimos el contenido 
		while (fgets(lee, MAXIMO, f) != NULL){
			printf(lee);
		}
	}
	else{
		printf("El archivo no fue encontrado");
	}
	fclose(f);
}
}
