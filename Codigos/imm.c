#define SUCCESS 0
#define INVALID_NULL_POINTER -1

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/*  Caminhos para os arquivos

    Victor: "C:\\Trabalho_ED1\\Trabalho_ED1\\Arquivos\\GIMP.pgm"


*/

    int open_txt(char *argv);
    int open_imm(char *argv);
    int convert_file(char *argv1, char *argv2);
    int segment_imm(char *argv1, char *argv2, char *argv3);
    int cc_imm(char *argv1, char *argv2);
    int lab_txt(char *argv1, char *argv2);

    int main(int argc, char *argv[]){

        if(strcmp(argv[1], "-open")==0){
            open_txt(argv[2]);
            /*if(strcmp(argv[2], "file.txt")==0)
                open_txt(argv[2]);
            else if(strcmp(argv[2], "file.imm")==0)
                open_imm(argv[2]);
            */
        }
        else if(strcmp(argv[1], "-convert")==0){
            convert_file(argv[2], argv[3]);
        }
        else if(strcmp(argv[1], "-segment")==0){
            segment_imm(argv[2], argv[3], argv[4]);
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

    int open_txt(char *argv){
        printf("Abrir %s\n", argv);

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

    int open_imm(char *argv){
        printf("Abrir %s\n", argv);
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