#include "strhashtable.h"
#include "hashtable.h"
#include <string.h>
#include <stdio.h>

int __strhh_cmpfunc(void* A, void* B, size_t sizeA, size_t sizeB) {
    return strcmp(A, B);
}

Hashtable* newStrHashTable() {
    int i;
    Hashtable *hasht = malloc(sizeof(Hashtable)); 
    for (i = 0; i < (1<<HASHBITSIZE_S); i++) {
        hasht->vector[i] = NULL;
    }
    hasht->cmpfunc = __strhh_cmpfunc;
    return hasht;
}

int strhh_insert(Hashtable* H, char* str) {
    return __hh_doaction(H, str, strlen(str), HH_INSERT);
}

int strhh_remove(Hashtable* H, char* str) {
    return __hh_doaction(H, str, strlen(str), HH_REMOVE);
}

int strhh_contains(Hashtable* H, char* str) {
    return __hh_doaction(H, str, strlen(str), HH_FIND);
}

void printAll(Hashtable* H) {
    int i;
    LLHashTable* ll;
    int first;
    for (i = 0; i < (1<<HASHBITSIZE_S); i++) {
        first = 1;
        while(ll != NULL) {
            if (first) {
                printf("%d: ", i);
                first = 0;
            }
            printf("'%s' ", ll->var);  
            ll = ll->next;
        }
    }
}
