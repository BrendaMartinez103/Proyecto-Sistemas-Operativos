#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

struct dirent *dire;

void main(int argc, char *argv[]){
	char dir[100];
    if(argv[1]==NULL) {
		printf("Falta un introducir un parametro.\n \n");
	}
    else {

	strncpy(dir, argv[1], sizeof(dir));
	DIR *d = opendir(dir);
	if (d == NULL){
		printf("El directorio no fue encontrado.\n");
	}
	else{
		while ((dire = readdir(d)) != NULL){//mientras se pueda leer listamos el contenido
			printf("%s | ", dire->d_name);
		}
		printf("\n");
	}
	closedir(d);
    }
}
