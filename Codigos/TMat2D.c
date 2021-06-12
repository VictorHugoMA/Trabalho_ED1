#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "TMat2D.h"

    struct TadMat{
        int nlinhas;
        int ncolunas; 
        int *dado;
    };
    //Cria a matriz
    //dado o numero de linhas e colunas
    //Retorno NULL para erro, Struct TadMat para sucesso
    TadMat *criar_mat(int nlin, int ncol){
        TadMat *mat;

        mat = malloc(sizeof(TadMat));
        if(mat==NULL)
            return NULL;
        
        mat->nlinhas = nlin;
        mat->ncolunas = ncol;
        mat->dado = malloc((nlin*ncol)*sizeof(int));

        if(mat->dado==NULL){
            free(mat);
            return NULL;
        }

        return mat;
    }

    //Libera a memoria
    //dado o ponteiro para a struct
    //Retorno -1 para erro e 0 para sucesso
    int free_mat(TadMat *mat){
        if(mat==NULL)
            return -1;
        else{
            free(mat->dado);
            free(mat);
            return 0;
        }
    }

    //Escreve um valor na posicao desejada da matriz
    //dado o ponteiro para a struct, a posicao(linha e coluna) e o valor 
    //Retorno -1 para erro e 0 para sucesso
    int escrever_mat(TadMat *mat, int nlin, int ncol, int val){
        if(mat==NULL)
            return -1;

        else{
            int pos;
            pos = ncol * mat->nlinhas + nlin;
            mat->dado[pos] = val;
        
            return 0;
        }
    }

    //Acessa uma posicao da matriz
    //dado o ponteiro para a struct, a posicao(linha e coluna), e um ponteiro para int(aux)
    //Retorno -1 para erro e 0 para sucesso
    int acessar_mat(TadMat *mat, int lin, int col, int *val){
        if(mat==NULL)
            return -1;

        else{
            int pos;
            pos = col * mat->nlinhas + lin;

            *val=mat->dado[pos];

            return 0;
        }
    }

    //Preenche uma matriz com valores aleatorios
    //dado o ponteiro para a struct e o valor maximo de preenchimento
    //Retorno -1 para erro e 0 para sucesso
    int preencher_mat(TadMat *mat, int max){
        if(mat==NULL)
            return -1;

        else{
            srand(time(NULL));
            for(int i=0; i<mat->nlinhas*mat->ncolunas; i++){
                mat->dado[i]=(rand()/(int)RAND_MAX)*max;
            }
            return 0;
        }
    }

    //Soma duas matrizes
    //dado dois ponterios para as structs a serem somadas e um para o resultado
    //Retorno -1 para erro e 0 para sucesso
    int soma_mat(TadMat *m1, TadMat *m2, TadMat *sm){
        int i;

        if(m1==NULL || m2==NULL || m1->nlinhas!=m2->nlinhas || m1->ncolunas!=m2->ncolunas){
            return -1;
        }

        else{
            for(i=0; i<m1->nlinhas*m1->ncolunas; i++){
                sm->dado[i] = m1->dado[i] + m2->dado[i];
            }
            return 0;
        }
    }

    //Multiplica duas matrizes 
    //dado o ponteiro para struct as duas matrizes a serem multiplicadas e o ponteiro para struct resultante
    //Retorno -1 para erro e 0 para sucesso 
    int mult_mat(TadMat *m1, TadMat *m2, TadMat *tm){
        if(m1==NULL || m2==NULL || tm==NULL || m1->ncolunas!=m2->nlinhas){
            return -1;
        }
        else{
            int i, j, k, pos1, pos2, pos3;
            int total=0;

            for(i=0; i<m1->nlinhas; i++){
		        for(j=0; j<m2->ncolunas; j++){

			        for(k=0; k<m1->ncolunas; k++){
                        pos1 = k*m1->nlinhas+i;
                        pos2 = j*m2->nlinhas+k;
                        
                        total+=m1->dado[pos1] * m2->dado[pos2];
			        }
                    pos3 = j*tm->nlinhas+i;
                    tm->dado[pos3]=total;
                    total=0;
		        }
	        }
            return 0;
        }
    }

    //Multiplica uma matriz por um numero
    //dado o ponteiro para a struct, ponteiro para a struct resultado e o valor a ser multiplicado
    //Retorno -1 para erro e 0 para sucesso
    int multConst_mat(TadMat *m1, TadMat *tm, int num){
        if(m1==NULL){
            return -1;
        }

        else{
            for(int i=0; i<m1->nlinhas*m1->ncolunas; i++){
                tm->dado[i]=m1->dado[i]*num;
            }
        return 0;
        }
    }

    //Traco ou soma da diagonal principal de uma matriz
    //dado o ponteiro para a struct e o ponterio de int para receber a soma
    //Retorno -1 para erro e 0 para sucesso
    int traco_mat(TadMat *m1, int *tot){

        if(m1==NULL){
            return -1;
        }
        else{
            int i, j, pos;
            int dp;
            for(i=0; i<m1->nlinhas; i++){
                for(j=0; j<m1->ncolunas; j++){
                    if(i==j){
                        pos = j*m1->nlinhas+i;
                        dp+=m1->dado[pos];
                    }
                }
            }
            *tot=dp;
            return 0;
        }
    }

    //Vetor com a soma das linahs
    //dado o ponteiro para a struct e o endereco de um ponteiro que sera o vetor
    //Retorno -1 para erro e 0 para sucesso
    int somaL_mat(TadMat *m1, int **sl){
            if(m1==NULL){
                return -1;
            }
            else{
                int i, j, pos;
            
                *sl=calloc(m1->nlinhas, sizeof(int));
                
                for(i=0; i<m1->nlinhas; i++){
                    for(j=0; j<m1->ncolunas; j++){
                        pos = j*m1->nlinhas+i;
                        (*sl)[i]+=m1->dado[pos];
                    }
                }
                
                return 0;
            }
    }

    //Vetor com a soma das colunas
    //dado o ponteiro para a struct e o endereco de um ponteiro que sera o vetor
    //Retorno -1 para erro e 0 para sucesso
    int somaC_mat(TadMat *m1, int **sc){
        if(m1==NULL){
            return -1;
        }
        else{
            int i, j, pos;

            *sc=calloc(m1->ncolunas, sizeof(int));

            for(i=0; i<m1->nlinhas; i++){
                for(j=0; j<m1->ncolunas; j++){
                    pos = j*m1->nlinhas+i;
                    (*sc)[j]+=m1->dado[pos];
                }
            }
            return 0;
        }
    }

    int print_matriz(TadMat *p){
        if(p==NULL){
            return -1;
        }
        int i, j, val;
        printf("\n");
        for(i=0; i<p->nlinhas; i++){
            for(j=0; j<p->ncolunas; j++){
                acessar_mat(p, i, j, &val);
                printf("%3d ", val);
            }
            printf("\n");
        }
        return 0;
    }

    int nl_nc_mat(TadMat *mat, int *nl, int *nc){
        if(mat==NULL){
            return -1;
        }
        
        *nl = mat->nlinhas;
        *nc = mat->ncolunas;

        return 0;
    }