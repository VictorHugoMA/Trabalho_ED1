#ifndef _TArquivo_
#define _TArquivo_

#define SUCCESS 0
#define INVALID_NULL_POINTER -1
#define ERROR -2
#define TXT_FILE 9
#define IMM_FILE 8
#define INCONSISTENT_FILE 10

#include"TMat2D.h"
#include"TLinkedList.h"

int open_file(char *file, TadMat **mat);

int convert_file(char *fileIN, char *fileOUT);

int segment_file(int thr, char *fileIN, char *fileSEG);

int cc_imm(char *argv1, char *argv2);

int lab_txt(char *argv1, char *argv2);

int print_file(char *file);

int identify_type(char *file);

int tam_mat_file(char *file, int *nl, int *nc);

int mat_to_file(TadMat *mat, char *fileOUT);

#endif