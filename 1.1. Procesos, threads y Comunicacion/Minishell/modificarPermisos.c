#include <stdio.h>
#include <sys/stat.h>
#include <string.h>


void main(int argc,char *argv[]){
    struct stat file;
    mode_t nuevoPermiso;
    int param=0;
    
    if (argc < 3){
        printf("debe indicar [chmod], [nombre directorio], [permiso]");
        return 1;
    }
    
    if (stat(argv[1],&file)==1){
        perror("error al obtener los permisos actuales");
        return 1;
    }
    nuevoPermiso =file.st_mode;
    if (strcmp(argv[2],"+lectura")==0){
        nuevoPermiso|=S_IRUSR;
        param=1;
    }
    else if (strcmp(argv[2],"-lectura")==0){
         nuevoPermiso &= ~S_IRUSR;
         param=1;
    }
    else if (strcmp(argv[2],"+escritura")==0){
         nuevoPermiso |=S_IWUSR;
         param=1;
    }
    else if (strcmp(argv[2],"-escritura")==0){
         nuevoPermiso &= ~S_IWUSR;
         param=1;
    }
    else if (strcmp(argv[2],"+ejecutar")==0){
         nuevoPermiso |=S_IXUSR;
         param=1;
    }
    else if (strcmp(argv[2],"-ejecutar")==0){
         nuevoPermiso &= ~S_IXUSR;
         param=1;
    }
    if (!param){
        printf("ingrese un parametro correct: +escritura, +lectura, +ejecutar,- escritura, -lectura, -ejecutar");
        return 1;
    }
    
    if (chmod(argv[1],nuevoPermiso)==-1){
        perror("error no se cambiaron los permisos");
        return 1;
    }else
    printf("permiso correctamente modificado");

}


