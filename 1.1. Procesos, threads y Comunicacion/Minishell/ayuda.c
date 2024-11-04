#include <stdlib.h>


void main(int argc, char *argv[]){

	printf("-mkdir nombre: Crea un directorio con el nombre indicado en la ubicacion actual.\n");
	printf("-rmdir nombre : Elimina el directorio con ese nombre indicado.\n");
	printf("-touch nombre: Crea un archivo con el nombre indicado en la dirección actual.\n");
	printf("-o puede indicar el directorio donde crear el archivo de la forma [DIRECTORIO]/[NOMBRE].\n ");
	printf("-ls nombre: Lista el contenido del directorio con el nombre indicado.\n");
	printf("-cat nombre: Muestra el contenido del archivo con el nombre indicado.\n");
	printf("-chmod nombre permisos : Cambia los permisos indicados de un archivo en el directorio con ese nombre.\n");
	printf(" Los comandos de los permisos son:\n");
	printf("           Para escritura: '+escitura'O '-escitura'\n");
	printf("           Para lectura: '+lectura' O '-lectura' \n");
	printf("           Para ejecucion: '+ejecutar'O'-ejecutar' \n");
	printf("-exit : Sale de la Minishell.\n");

}
