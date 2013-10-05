#include "strhashtable.h"
#include "hashtable.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

int __strhh_cmpfunc(void* A, void* B, size_t sizeA, size_t sizeB) {
    return strcmp(A, B);
}

char* __strhh_getstrnode(void* a, size_t size){
    return (char*)a;
}

void __strhh_postgetstrnode(void* a, size_t size, char* str){
    // nothing
}

Hashtable* newStrHashTable() {
    int i;
    Hashtable *hasht = malloc(sizeof(Hashtable)); 
    for (i = 0; i < (1<<HASHBITSIZE_S); i++) {
        hasht->vector[i] = NULL;
    }
    hasht->cmpfunc = __strhh_cmpfunc;
    hasht->tostrfunc = __strhh_getstrnode;
    hasht->posttostrfunc = __strhh_postgetstrnode;
    return hasht;
}

int strhh_insert(Hashtable* H, char* str) {
    int len = strlen(str);
    int result;
    char* cpy = malloc((len + 1) * sizeof(char));
    strcpy(cpy, str);
    result = __hh_doaction(H, cpy, len, HH_INSERT);
    if (result != INSERTED) {
        free(cpy);
    }
    return result;
}



int strhh_remove(Hashtable* H, char* str) {
    return __hh_doaction(H, str, strlen(str), HH_REMOVE);
}

int strhh_contains(Hashtable* H, char* str) {
    return __hh_doaction(H, str, strlen(str), HH_FIND) == WAS_THERE;
}

void printAll(Hashtable* H) {
    int i;
    LLHashTable* ll;
    int first;
    for (i = 0; i < (1<<HASHBITSIZE_S); i++) {
        first = 1;
        ll = H->vector[i];
        while(ll != NULL) {
            if (first) {
                printf("%d: \n   ", i);
                first = 0;
            }   
            fflush(stdout);
            LOG_HASH("(%lX, %lX = )", (uint64_t)ll, (uint64_t)ll->var);
            fflush(stderr);
            printf("'%s' ", (char*) ll->var);  
            ll = ll->next;
        }
        if (!first)
            printf("\n");
    }
}
