#ifndef _TArquivo_
#define _TArquivo_

#define SUCCESS 0
#define INVALID_NULL_POINTER -1
#define TXT_FILE 9
#define IMM_FILE 8
#define INCONSISTENT_FILE 10

int open_file(char *argv);

int convert_file(char *argv1, char *argv2);

int segment_imm(char *argv1, char *argv2, char *argv3);

int cc_imm(char *argv1, char *argv2);

int lab_txt(char *argv1, char *argv2);

int identify_type(char *file);

#endif