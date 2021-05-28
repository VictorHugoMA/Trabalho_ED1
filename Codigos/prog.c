#include<stdio.h>
#include<string.h>
#include<stdlib.h>

    int open_txt(char *argv);

    int main(int argc, char *argv[]){

        if(strcmp(argv[1], "-open")==0){
            open_txt(argv[2]);
        }
        else{
            printf("Comando nao encontrado\n");
        }
        
        return 0;
    }

    int open_txt(char *argv){
        printf("Abrir\n");
        printf("%s\n", argv);
    }

    