#include "hashtable.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

int __hh_calchash(void* structure, size_t size) {
    int hash = 0;
    size_t i;
    for (i = 0; i < size; i++) {
        hash ^= *((char*)(structure + i));
        hash ^= i;
    }
    return hash & ((1<<HASHBITSIZE_S) -1);
}

int __hh_doaction(Hashtable* H, void* structure, size_t size, int action) {
    int hash;
    LLHashTable *list, *backptr;
    int (*cmpfunc)(void* a, void* b, size_t sizea, size_t sizeb);
    hash = __hh_calchash(structure, size); 
    list = H->vector[hash];
    backptr = list;

    if (action == HH_INSERT && list == NULL) {
        H->vector[hash] = __LL_pool_get();
        H->vector[hash]->var = structure;
        printf("inseriu %s em %d ptr was %ld \n", structure, hash, H->vector[hash]);
        return INSERTED;
    }

    cmpfunc = H->cmpfunc;
    while (cmpfunc(list->var, structure, list->size, size) != 0) {
        if (list->next == NULL) {
            if (action == HH_INSERT) {
                list->next = __LL_pool_get();
                list->next->var = structure;
                return INSERTED;
            }
            return NOT_IN_HASH_TABLE;
        }
    }
    if (action == HH_REMOVE) {
        backptr = list->next; // TODO isso nao funciona
        __LL_pool_release(list);
    }
    return WAS_THERE;
}

void __LL_pool_release(LLHashTable* node) {
    assert(__LL_pool__ != NULL); // ver o tamanho da lista e liberar nós se for o caso.
    node->next = __LL_pool__;
    __LL_pool__ = node;
}

LLHashTable* __LL_pool_get() {
    static int size_to_alloc = -1;
    int i; 
    if (size_to_alloc == -1 ) { 
        size_to_alloc = INITIAL_SIZE_LL_POLL;
        __LL_pool__ = malloc(sizeof(LLHashTable));
        __LL_pool__->next = NULL;
    }
    assert(__LL_pool__ != NULL);
    if (__LL_pool__->next == NULL) {
        LLHashTable *node;
        node = malloc(sizeof(LLHashTable) * size_to_alloc);
        for (i = 0; i < size_to_alloc; i++) {
            __LL_pool_release(node + i * sizeof(LLHashTable));
        }
        size_to_alloc *= 2;
    }
    LLHashTable *returnnode = __LL_pool__;
    __LL_pool__ = __LL_pool__->next;
    return returnnode;
}
