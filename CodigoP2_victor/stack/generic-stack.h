#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef STACK_H
# define STACK_H

typedef void *ElementAddr;
typedef void (*PfCbFree)(ElementAddr);

typedef struct StackRecord
{
        ElementAddr *array;
        int     elemsize;
        int     loglength;
        int     alloclength;
        PfCbFree    freefn;
} *Stack;
/* Create a new stack */
Stack stack_create(int elemsize, PfCbFree freefn);

/* Dispose the stack */
void stack_dispose(Stack stk);

/* Make the given stack empty */
void stack_make_empty(Stack stk);

/* Return true if the stack is empty */
int stack_is_empty(Stack stk);

/* Insert a new element onto stack */
void stack_push(Stack stk, ElementAddr elemaddr);

/* Delete the top element off the stack */
void stack_pop(Stack stk);

/* Fetch the top element from the stack */
void stack_top(Stack stk, ElementAddr elemaddr);

/* Fetch & Delete the top element from the stack */
void stack_top_and_pop(Stack stk, ElementAddr elemaddr);

#endif