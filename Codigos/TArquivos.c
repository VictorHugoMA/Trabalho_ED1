#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TArquivos.h"

int open_file(char *file, TadMat **mat)
{
    FILE *fp;
    int aux, nl, nc, i, j, num;

    printf("Abrir %s\n", file);

    if (identify_type(file) == TXT_FILE)
    {

        fp = fopen(file, "r");
        if (fp == NULL)
        {
            printf("Erro na abertura do arquivo\n");
            return INVALID_NULL_POINTER;
        }

        aux = tam_mat_file(file, &nl, &nc);
            if(aux!=SUCCESS){
                return ERROR;
            }

        *mat = criar_mat(nl, nc);

        for (i = 0; i < nl; i++)
        {
            for (j = 0; j < nc; j++)
            {
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
        if (fp == NULL)
        {
            printf("Erro na abertura do arquivo\n");
            return INVALID_NULL_POINTER;
        }
        fread(&nl, sizeof(int), 1, fp);
        fread(&nc, sizeof(int), 1, fp);

        *mat = criar_mat(nl, nc);

        for (i = 0; i < nl; i++)
        {
            for (j = 0; j < nc; j++)
            {
                fread(&num, sizeof(int), 1, fp);
                escrever_mat(*mat, i, j, num);
            }
        }
        fclose(fp);
        return SUCCESS;
    }

    else
    {
        return INCONSISTENT_FILE;
    }
}

int convert_file(char *fileIN, char *fileOUT)
{
    printf("Converte %s para %s\n", fileIN, fileOUT);

    TadMat *mat;
    int aux;

    aux = open_file(fileIN, &mat);

    if (aux == SUCCESS)
    {
        aux = mat_to_file(mat, fileOUT);

        if (aux == SUCCESS)
            return SUCCESS;
    }
    else
    {
        return ERROR;
    }
}

int segment_file(int thr, char *fileIN, char *fileSEG)
{
    printf("Faz o thresholding com um valor %d da imagem %s e escreve o resultado em %s\n", thr, fileIN, fileSEG);

    TadMat *mat;
    int i, j, nl, nc, num, aux;

    aux = open_file(fileIN, &mat);

    if (aux != SUCCESS)
        return ERROR;

    nl_nc_mat(mat, &nl, &nc);

    for (i = 0; i < nl; i++)
    {
        for (j = 0; j < nc; j++)
        {
            acessar_mat(mat, i, j, &num);

            if (num > thr)
            {
                escrever_mat(mat, i, j, 1);
            }
            else
            {
                escrever_mat(mat, i, j, 0);
            }
        }
    }

    mat_to_file(mat, fileSEG);

    free_mat(mat);

    return SUCCESS;
}


int cc_imm(char *fileSEG, char *fileOUT)
{
    
    Stack *st;
    TadMat *img, *img_rot;
    ponto vetp[4], base;
    int nl, nc, i, j, k, valA, valB, rotulo = 0;
    

    open_file(fileSEG, &img); // Abre o arquivo segmentado 

    nl_nc_mat(img, &nl, &nc); // Recebe numero de linhas e colunas
    img_rot = criar_mat(nl, nc); // Cria imagem de contrução com as mesmas proporções da original

    preencher_mat(img_rot, 0); // Preenche a img com zeros

    st = stack_create();

    for(i = 0; i < nl; i++)
    {
        for(j = 0; j < nc; j++)
        {
            acessar_mat(img, i, j, &valA);
            acessar_mat(img_rot, i, j, &valB);

            if(valA == 1 && valB == 0)//Estopim de procura do objeto 
            {
                rotulo++;

                escrever_mat(img_rot, i, j, rotulo);

                base.x = i;
                base.y = j;

                stack_push(st, base);

                while(stack_size(st) > 0)
                {
                    vetp[0].x= base.x;
                    vetp[0].y= base.y-1;
                    vetp[1].x= base.x-1;
                    vetp[1].y= base.y;
                    vetp[2].x= base.x;
                    vetp[2].y= base.y+1;
                    vetp[3].x= base.x+1;
                    vetp[3].y= base.y;

                    for(k = 0; k < 4; k++)
                    {
                        acessar_mat(img, vetp[k].x, vetp[k].y, &valA);
                        acessar_mat(img_rot, vetp[k].x, vetp[k].y, &valB);
                        if(valA == 1 && valB == 0)
                        {
                            escrever_mat(img_rot, vetp[k].x, vetp[k].y, rotulo);
                            stack_push(st, vetp[k]);
                            base = vetp[k];
                            break;
                        }
                        else
                        {
                            if(k == 3)
                            {
                                stack_pop(st);
                                stack_top(st, &base);
                            }

                            if(stack_size(st) == 1 && k == 3)
                                stack_pop(st);
                        }
                    } 
                }
            }
        }
    }
    mat_to_file(img_rot, fileOUT);

    free_mat(img);
    free_mat(img_rot);
    stack_free(st);

    return SUCCESS;
}


int lab_txt(char *argv1, char *argv2)
{
    printf("Recebe um labirinto %s e mostra o caminho em %s\n", argv1, argv2);

    int label = 1;
}


int identify_type(char *file)
{

    int tamanho;

    tamanho = strlen(file); //Recebe o tamanho da string, para verificar cada caracter

    if (tamanho > 4 && file[tamanho - 4] == '.' && file[tamanho - 3] == 't' && file[tamanho - 2] == 'x' && file[tamanho - 1] == 't')
    { //Analisa cada caracter da string para conferir se eh "txt"
        return TXT_FILE;
    }
    else if (tamanho > 4 && file[tamanho - 4] == '.' && file[tamanho - 3] == 'i' && file[tamanho - 2] == 'm' && file[tamanho - 1] == 'm')
    { //Analisa cada caracter da string para conferir se eh "imm"
        return IMM_FILE;
    }
    else
    { //Caso nao seja nenhum dos 2
        return INCONSISTENT_FILE;
    }
}

int tam_mat_file(char *file, int *nl, int *nc)
{

    FILE *fp;
    int aux, elementos = 0, nlin = 0, ncol = 0;
    char atual, anterior;

    atual = ' ';

    if (identify_type(file) == TXT_FILE)
    {
        fp = fopen(file, "r");
        if (fp == NULL)
        {
            return INVALID_NULL_POINTER;
        }

        while (1)
        {
            anterior = atual;
            elementos++;

            fscanf(fp, "%d", &aux);
            if (feof(fp))
            { 
                break;
            }
            fscanf(fp, "%c", &atual);
            if (feof(fp))       
            { 
                break;
            }
            if (atual == '\n')
            {
               nlin++;
            }
        }

        elementos++; 
        
        if (feof(fp))
        {
            if (anterior != '\n')
                nlin++;
        }

        ncol = elementos / nlin;

        *nl=nlin;
        *nc=ncol;

        fclose(fp);
        return SUCCESS;
    }
    else if (identify_type(file) == IMM_FILE)
    {

        fp = fopen(file, "rb");
        
        if(fp == NULL)
        {
            return ERROR;
        }

        fscanf(fp,"%d", &nlin);
        fscanf(fp,"%d", &ncol);

        *nl=nlin;
        *nc=ncol;

        fclose(fp);
        return SUCCESS;

    }
    else{
        return INCONSISTENT_FILE;
    }
}

int mat_to_file(TadMat *mat, char *fileOUT)
{

    FILE *fp;
    int num, nl, nc, i, j;

    nl_nc_mat(mat, &nl, &nc);

    if (identify_type(fileOUT) == TXT_FILE)
    {

        fp = fopen(fileOUT, "w");
        if (fp == NULL)
        {
            return INVALID_NULL_POINTER;
        }

        for (i = 0; i < nl; i++)
        {
            for (j = 0; j < nc; j++)
            {
                acessar_mat(mat, i, j, &num);
                fprintf(fp, "%3d", num);

                if (j != nc - 1)
                {
                    fprintf(fp, " ");
                }
            }
            if (i != nl - 1)
            {
                fprintf(fp, "\n");
            }
        }
        fclose(fp);
        return SUCCESS;
    }

    else if (identify_type(fileOUT) == IMM_FILE)
    {

        fp = fopen(fileOUT, "wb");
        if (fp == NULL)
        {
            return INVALID_NULL_POINTER;
        }

        fwrite(&nl, sizeof(int), 1, fp);
        fwrite(&nc, sizeof(int), 1, fp);

        for (i = 0; i < nl; i++)
        {
            for (j = 0; j < nc; j++)
            {
                acessar_mat(mat, i, j, &num);
                fwrite(&num, sizeof(int), 1, fp);
            }
        }

        fclose(fp);
        return SUCCESS;
    }
    else
    {
        return INCONSISTENT_FILE;
    }
}

int print_file(char *file)
{
    TadMat *mat;
    int aux;

    aux = open_file(file, &mat);

    if (aux != SUCCESS)
    {
        return ERROR;
    }

    print_matriz(mat);
    free_mat(mat);

    return SUCCESS;
}