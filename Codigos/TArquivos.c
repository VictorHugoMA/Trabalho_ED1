#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"TArquivos.h"
    
    int open_file(char *file, TadMat **mat){
        FILE *fp;
        int aux, nl, nc, i, j, num;  

        printf("Abrir %s\n", file);

        if (identify_type(file) == TXT_FILE)
        {      

            fp = fopen(file, "r");
            if(fp==NULL){
                printf("Erro na abertura do arquivo\n");
                return INVALID_NULL_POINTER;
            }

            aux = tam_mat_file(file, &nl, &nc);
            if(aux!=SUCCESS){
                return ERROR;
            }

            /* 
            nl=32;
            nc=23; 
            */

            *mat = criar_mat(nl, nc);

            for(i=0; i<nl; i++){
                for(j=0; j<nc; j++){
                    fscanf(fp, "%d", &num);
                    escrever_mat(*mat, i, j, num);
                }
            }

            fclose(fp);

            return SUCCESS;


        }
        else if (identify_type(file) == IMM_FILE)
        {

            fp = fopen(file, "rb");
            if(fp==NULL){
                printf("Erro na abertura do arquivo\n");
                return INVALID_NULL_POINTER;
            }
            fread(&nl, sizeof(int), 1, fp);
            fread(&nc, sizeof(int), 1, fp);

            *mat = criar_mat(nl, nc);

            for(i=0; i<nl; i++){
                for(j=0; j<nc; j++){
                    fread(&num, sizeof(int), 1, fp);
                    escrever_mat(*mat, i, j, num);
                }
            }
            fclose(fp);
            return SUCCESS;
        }

        else{
            return INCONSISTENT_FILE;
        }
          
    }


    int convert_file(char *fileIN, char *fileOUT){
        printf("Converte %s para %s\n", fileIN, fileOUT);

        TadMat *mat;

        open_file(fileIN, &mat);
        mat_to_file(mat, fileOUT);

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

        tamanho = strlen(file); //Recebe o tamanho da string, para verificar cada caracter 

        if (file[tamanho-3] == 't' && file[tamanho-2] == 'x' && file[tamanho-1] == 't')
        {//Analisa cada caracter da string para conferir se eh "txt"
            return TXT_FILE;
        }
        else if (file[tamanho-3] == 'i' && file[tamanho-2] == 'm' && file[tamanho-1] == 'm')
        {//Analisa cada caracter da string para conferir se eh "imm"
            return IMM_FILE;
        }
        else
        {//Caso nao seja nenhum dos 2
            return INCONSISTENT_FILE;
        }

    }


    int tam_mat_file(char *file, int *nl, int *nc){

        FILE *fp;

        int aux, i, j, num;  

        if (identify_type(file) == TXT_FILE)
        {
            fp = fopen(file, "r");
            if (fp == NULL)
            {
                return ERROR;
            }
            
        }
        else if (identify_type(file) == IMM_FILE)
        {
            /* code */
        }
        

    }



    int mat_to_file(TadMat *mat, char *fileOUT){


    }



    int print_file(char *file){
        TadMat *mat;
        int aux;

        aux = open_file(file, &mat);

        if(aux!=SUCCESS){
            return ERROR;
        }

        print_matriz(mat);
        free_mat(mat);

        return SUCCESS;
    }