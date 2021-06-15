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

//Funcao labirinto 
int lab_file(char *fileIn, char *fileOUT)
{

    Stack *st;
    TadMat *img, *img_rot;
    ponto inicio, atual, vetp[4]; //Especial para coordenadas vizinhas
    int i, j, nl, nc, val, valA, valB, x, y;

    open_file(fileIn, &img); //Abre o arquivo com o Binario

    nl_nc_mat(img, &nl, &nc); //Recebe numero de linhas e colunas
    img_rot = criar_mat(nl ,nc); //Cria img de construcao semelhante a original
    
    preencher_mat(img_rot, 0); //Preenche a img_rot com zeros

    st = stack_create();

    printf("Teste 1\n");
    //Recebe a posicao do inicio do labirinto na TadMat img
    for(i = 0; i < nl; i++)
    {
        acessar_mat(img, i, 0, &val);
        if(val == 1)
        {
            inicio.x = i;
            inicio.y = 0;
            break;
        }
    }
    printf("Teste 2\n");

    stack_push(st, inicio);
    escrever_mat(img_rot, inicio.x, inicio.y, 2);

    atual.x = inicio.x;
    atual.y = inicio.y + 1;
    escrever_mat(img_rot, atual.x, atual.y, 2); // m_insert_int

    printf("Teste 3\n");
    while(atual.y != nc-1)//Enquanto nao chegar na ultima coluna
    {
        vetp[0].x= atual.x ;
        vetp[0].y= atual.y-1;
        vetp[1].x= atual.x-1;
        vetp[1].y= atual.y;
        vetp[2].x= atual.x;
        vetp[2].y= atual.y+1;
        vetp[3].x= atual.x+1;
        vetp[3].y= atual.y;

        printf("Teste 4\n");
        for(i = 0; i < 4; i++)
        {
            x = vetp[i].x;//Faz a rotacao x
            y = vetp[i].y;//Faz a rotacao y

            acessar_mat(img, x, y, &valA);//(m_select_int)Pega um valor na matriz original
            acessar_mat(img_rot, x, y, &valB);//Pega o mesmo valor na mesma pos da em contrucao

            if(valA == 1 && valB == 0)//Se houver um caminho valido(1), rotula e anda no primeiro que achar
            {
                stack_push(st, vetp[i]);//Empilha a posicao
                escrever_mat(img_rot, x , y, 2);//Insere 2 na matriz
                atual = vetp[i];//Atual recebe o ponto de chegada
                break;
            }
            else if(i == 3)//Significa que nao ha nenhum a ser percorrido, volta para o ponto anterior
            {
                escrever_mat(img_rot, atual.x, atual.y, 1);//Insere na matriz em contrucao as posicoes atuais
                stack_pop(st);//As posicoes salvas sa oretiradas da pilha
                stack_top(st, &atual);//Logo em seguida eh consultado e passado para o atual um novo valor que eh a ultima posicao da pilha
            }
            
        }
    }

    printf("Teste 5\n");
    //Remove os caminhos sem fim rotulados de 1.Atribui a essas posicoes o valor 0.
    for(i = 0; i < nl; i++)
    {
        for(j = 0; j < nc; j++)
        {
            acessar_mat(img_rot, i, j, &val);//Pega um valor na posicao

            if(val != 2 && val != 0)//Caso for 2 eh limpado
                escrever_mat(img_rot, i, j, 0);//Inserindo 0
        }
    }

    mat_to_file(img_rot, fileOUT);
    printf("Teste 6\n");

    free_mat(img);
    free_mat(img_rot);
    stack_free(st);
    return SUCCESS;
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