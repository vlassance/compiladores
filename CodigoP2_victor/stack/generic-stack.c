#include "generic-stack.h"

#define MIN_STACK_SIZE (4000)

/* Create a new stack */
Stack stack_create(int elemsize, PfCbFree freefn) {
    Stack stk;

    stk = malloc(sizeof(struct StackRecord));

    if ( stk == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }

    stk->array = malloc(elemsize * MIN_STACK_SIZE);
    if (stk->array == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    stk->elemsize = elemsize;
    stk->loglength = 0; 
    stk->alloclength = MIN_STACK_SIZE;
	stk->freefn = freefn;
	
	return stk;
}

/* Dispose the stack*/
void stack_dispose(Stack stk) {
    stack_make_empty(stk);
    free(stk->array);
    free(stk);
}

/* Make the given stack empty*/
void stack_make_empty(Stack stk) {
    if ( stk->freefn ) {
        int i;
        for ( i = 0; i < stk->loglength; ++i) {
            stk->freefn((char *)stk->array + 
                    i * stk->elemsize);
        }
    }
    stk->loglength = 0;
}

/* Return true if the stack is empty*/
int stack_is_empty(Stack stk) {
    return stk->loglength == 0;
}

static void stack_grow(Stack stk) {
    stk->alloclength *= 2;
    stk->array = realloc(stk->array, 
                 stk->alloclength * stk->elemsize);
}

/* Insert a new element onto stack */
void stack_push(Stack stk, ElementAddr elemaddr) {
    ElementAddr target;
    if ( stk->loglength == stk->alloclength )
        stack_grow(stk);
    target = (char *)stk->array + stk->loglength * stk->elemsize;
    memcpy(target, elemaddr, stk->elemsize);
    stk->loglength++;   
}

/* Delete the top element off the stack */
void stack_pop(Stack stk) {
    ElementAddr target;
    if ( stack_is_empty(stk) ) {
        fprintf(stderr, "Empty stack\n");
        exit(1);
    }
    if ( stk->freefn ) {
        target = (char *)stk->array + 
             (stk->loglength-1) * stk->elemsize;
    stk->freefn(target);
    }
    stk->loglength--;
}

/* Fetch the top element from the stack */
void stack_top(Stack stk, ElementAddr elemaddr) {
    void *target = (char *)stk->array + 
               (stk->loglength-1) * stk->elemsize;
    memcpy(elemaddr, target, stk->elemsize);
}

/* Fetch & Delete the top element from the stack */
void stack_top_and_pop(Stack stk, ElementAddr elemaddr) {
    ElementAddr target;
    if ( stack_is_empty(stk) ) {
        fprintf(stderr, "Empty stack\n");
        exit(1);
    }
    target = (char *)stk->array + 
         (stk->loglength-1) * stk->elemsize;
    memcpy(elemaddr, target, stk->elemsize);
    stk->loglength--;
}
