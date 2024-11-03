#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void main(int argc,char *argv[]){
    
    int result;
    int resultado = -1;
    int encontre = 0;

    if(argv[1]==NULL)
        printf("Falta un introducir un parametro.\n \n");
    else {

        if(argv[2]!=NULL){
            
            for (int i=0; i<3 && !encontre; i++){
                
                switch(i){
                    
                    case 0: 
                        if(strcmp(argv[2],"lectura")==0) { 
                            resultado=0; 
                            encontre=1; 
                          
                        } 
                        break;
                        
                    case 1: 
                        if(strcmp(argv[2],"escritura")==0) { 
                            resultado=2; 
                            encontre=1; 
                            
                        } 
                        break;
                        
                    case 2: 
                        if(strcmp(argv[2],"ejecucion")==0) { 
                            resultado=3; 
                            encontre=1; 
                            
                        } 
                        break;
                        
                }
            }
        }

        if(resultado==-1)
            printf(" parametro incorrecto, las opciones son 'lectura', 'ejecucion' y 'escritura'.\n \n");
        else {
            char dir[100];

            strncpy(dir, argv[1], sizeof(dir));
        	int valor = atoi(argv[2]);
        	result= chmod(dir, valor);
            if (result == -1) 
                perror("Error al cambiar los permisos del archivo \n \n");
            else
                printf("Permisos del archivo cambiados correctamente.\n \n");
        }
    }
}

