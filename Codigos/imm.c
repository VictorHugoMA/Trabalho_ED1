#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"TArquivos.h"

/*  Caminhos para os arquivos

    Victor: "C:\\Trabalho_ED1\\Trabalho_ED1\\Arquivos\\GIMP.pgm"
    Pedro: "C:\\Users\\Zorak\\OneDrive\\Documentos\\trab\\Trabalho_ED1\\Arquivos\\GIMP.pgm"

*/

    int main(int argc, char *argv[]){

        if(strcmp(argv[1], "-open")==0){
            print_file(argv[2]);

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