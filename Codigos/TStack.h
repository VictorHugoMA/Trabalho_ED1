#ifndef _TStack_
#define _TStack_

#include "Ponto.h"

typedef struct Stack Stack;

Stack *stack_create();

void stack_free(Stack *st);

int stack_push(Stack *st, ponto p);

int stack_pop(Stack *st);

int stack_top(Stack *st, ponto *p);

int stack_size(Stack *st);

int stack_print(Stack *st);

#endif