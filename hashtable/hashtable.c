#include "hashtable.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
int __hh_calchash(void* structure, size_t size) {
    int hash = 0;
    size_t i;
    for (i = 0; i < size; i++) {
        hash ^= *((char*)(structure + i));
        hash ^= i;
    }
    return hash & ((1<<HASHBITSIZE_S) - 1L);
}

int __hh_doaction(Hashtable* H, void* structure, size_t size, int action) {
    int hash;
    LLHashTable *list;
    LLHashTable *backptr;
    int (*cmpfunc)(void* a, void* b, size_t sizea, size_t sizeb);
    
    hash = __hh_calchash(structure, size); 
    LOG_HASH("calculated hash: %d\n", hash);
    list = H->vector[hash];
    backptr = list;

    if (action == HH_INSERT && list == NULL) {
        H->vector[hash] = __LL_pool_get();
        H->vector[hash]->var = structure;
        H->vector[hash]->next = NULL;
        H->vector[hash]->size = size;
        LOG_HASH(
            "put %lX at %d, ptr was %lX\n", 
            (uint64_t) structure, 
            hash, 
            (uint64_t) H->vector[hash]
        );
        return INSERTED;
    } else if (list == NULL) {
        return NOT_IN_HASH_TABLE;
    }
    
    cmpfunc = H->cmpfunc;
    NODE_REPRINT(H, list->var, list->size, "ptrlist: %lX to %s << \n", (uint64_t)list);
    while (cmpfunc(list->var, structure, list->size, size) != 0) {
        if (list->next == NULL) {
            if (action == HH_INSERT) {
                list->next = __LL_pool_get();
                list->next->var = structure;
                list->next->next = NULL;
                list->next->size = size;
                LOG_HASH(
                    "[while] put %lX at %d, ptr was %lX \n", 
                    (uint64_t)structure, 
                    hash, 
                    (uint64_t)list
                );
                return INSERTED;
            }
            return NOT_IN_HASH_TABLE;
        }
    }
    if (action == HH_REMOVE) {
        LOG_HASH("REMOVING %lX\n", (uint64_t)list->var);
        backptr = list->next; // TODO isso nao funciona
        __LL_pool_release(list);
    }
    LOG_HASH("It (%lX) was there\n", (uint64_t)list->var);
    return WAS_THERE;
}

void __LL_pool_release(LLHashTable* node) {
    assert(__LL_pool__ != NULL); // ver o tamanho da lista e liberar nós se for o caso.
    LOG_HASH(
        "%lX - %lX turned to %lX - %lX\n", 
        (uint64_t)__LL_pool__,
        (uint64_t)__LL_pool__ + sizeof(LLHashTable),
        (uint64_t)((uint64_t)node),
        (uint64_t)((uint64_t)node) + sizeof(LLHashTable)
    );
    node->next = __LL_pool__;
    __LL_pool__ = node;
}

LLHashTable* __LL_pool_get() {
    static int size_to_alloc = -1;
    size_t i; 
    if (size_to_alloc == -1 ) { 
        size_to_alloc = INITIAL_SIZE_LL_POLL;
        LOG_HASH("Alocating first linked list element\n");
        __LL_pool__ = malloc(sizeof(LLHashTable));
        __LL_pool__->next = NULL;
    }
    assert(__LL_pool__ != NULL);
    if (__LL_pool__->next == NULL) {
        LLHashTable *node;
        LOG_HASH("Alocating %d elements for the pool!\n", size_to_alloc);
        node = malloc(sizeof(LLHashTable) * size_to_alloc);
        LOG_HASH(
            "Space from %lX to %lX was allocated\n", 
            (uint64_t)node, 
            (uint64_t)((uint64_t)node) + (sizeof(LLHashTable) * size_to_alloc)
        );
        for (i = 0L; i < size_to_alloc; i++) {
            __LL_pool_release(&(node[i]));
        }
        size_to_alloc *= 2;
    }
    LLHashTable *returnnode = __LL_pool__;
    __LL_pool__ = __LL_pool__->next;
    LOG_HASH(
        "%lX A node for you!\n", 
        (uint64_t)returnnode
    );
    return returnnode;
}
