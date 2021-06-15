#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"TArquivos.h"

/*  Caminhos para os arquivos

    Victor: "Arquivos\\GIMP.txt"
    Pedro: "Arquivos\\GIMP.pgm"

*/

    int main(int argc, char *argv[]){
        int aux, thr;

        if(strcmp(argv[1], "-open")==0){
            aux = print_file(argv[2]);

            if(aux!=SUCCESS)
                printf("Erro\n");

        }
        else if(strcmp(argv[1], "-convert")==0){
            aux = convert_file(argv[2], argv[3]);

            if(aux==SUCCESS)
                printf("Conversao feita com sucesso\n");
            else{
                printf("Erro\n");
            }


        }
        else if(strcmp(argv[1], "-segment")==0){
            thr = atoi(argv[2]);

            aux = segment_file(thr, argv[3], argv[4]);

            if(aux==SUCCESS)
                printf("Segmentacao feita com sucesso\n");
            else{
                printf("Erro\n");
            }
            
        }

        else if(strcmp(argv[1], "-cc")==0){
            cc_imm(argv[2], argv[3]);
        }

        else if(strcmp(argv[1], "-lab")==0){
            lab_txt(argv[2], argv[3]);
        }

        else{
            printf("Comando nao encontrado\n");
        }
        
        return 0;
    }