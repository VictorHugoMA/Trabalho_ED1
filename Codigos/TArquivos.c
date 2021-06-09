#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"TArquivos.h"
    
    int open_file(char *argv){
        printf("Abrir %s\n", argv);
        if (identify_type(argv) == TXT_FILE)
        {        
            char c;
            FILE *fp;
            fp = fopen(argv, "r");

            if(fp==NULL){
                printf("Erro na abertura do arquivo\n");
                return INVALID_NULL_POINTER;
            }
            int cont=0;
            while(cont<3){
                c = fgetc(fp);
                if(c==' '){
                    printf(" ");
                }
                else{
                    printf("%c", c);
                }
                if(c=='\n'){
                    printf("\n");
                    cont++;
                }

            }
            while(!feof(fp)){
                c = fgetc(fp);
                if(c=='\n'){
                    printf("\n");
                }
                else{
                    printf("%c", c);
                }

            }

            return SUCCESS;

            fclose(fp);

        }
        else if (identify_type(argv) == IMM_FILE)
        {
            printf("Abrir %s\n", argv);
        }
          
    }


    int convert_file(char *argv1, char *argv2){
        printf("Converte %s para %s\n", argv1, argv2);
    }

    int segment_imm(char *argv1, char *argv2, char *argv3){
        printf("Faz o thresholding com um valor %s da imagem %s e escreve o resultado em %s\n", argv1, argv2, argv3);
    }

    int cc_imm(char *argv1, char *argv2){
        printf("Detecta os componentes conexos de %s e sai para %s\n", argv1, argv2);
    }

    int lab_txt(char *argv1, char *argv2){
        printf("Recebe um labirinto %s e mostra o caminho em %s\n", argv1, argv2);
    }

    int identify_type(char *file) {

        int tamanho;

        tamanho = strlen(file);

        if (file[tamanho-3] == 't' && file[tamanho-2] == 'x' && file[tamanho-1] == 't')
        {
            return TXT_FILE;
        }
        else if (file[tamanho-3] == 'i' && file[tamanho-2] == 'm' && file[tamanho-1] == 'm')
        {
            return IMM_FILE;
        }
        else
        {
            return INCONSISTENT_FILE;
        }

    }