#include "hashtable.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int __hh_calchash(void* structure, size_t size) {
    // verrrryyy dummmmyy hash! but it works better than expected.
    // it is faster than the magic 31 * hash + c for random strings
    int hash = 0;
    size_t i;
    for (i = 0; i < size; i++) {
        hash ^= *((char*)(structure + i));
        hash<<= 2;
    }
    LOG_HASH("Size of hashtable is: %ld\n", 1L<<HASHBITSIZE_S);
    return (hash) & ((1L<<HASHBITSIZE_S) - 1L);
}


int __hh_doaction(
    Hashtable* H, 
    void** structure, 
    size_t* size, 
    void** content, 
    size_t* contentsize, 
    int action
) {
    int hash;
    LLHashTable *list;
    LLHashTable *backptr;
    void* ptrtmp;
    size_t size_ttmp;
    int (*cmpfunc)(void* a, void* b, size_t sizea, size_t sizeb);
    
    hash = __hh_calchash(*structure, *size); 
    LOG_HASH("calculated hash: %d\n", hash);
    list = H->vector[hash];
    backptr = list;

    if (action == HH_INSERT && list == NULL) {
        H->vector[hash] = __LL_pool_get();
        H->vector[hash]->var = *structure;
        H->vector[hash]->next = NULL;
        H->vector[hash]->size = *size;
        H->vector[hash]->content = *content;
        H->vector[hash]->contentsize = *contentsize;
        LOG_HASH(
            "put %lX at %d, ptr was %lX\n", 
            (uint64_t) *structure, 
            hash, 
            (uint64_t) H->vector[hash]
        );
        H->size++;
        LOG_HASH("Hashtable has %ld elements\n", H->size);
        return INSERTED;
    } else if (list == NULL) {
        return NOT_IN_HASH_TABLE;
    }
    
    cmpfunc = H->cmpfunc;
    NODE_REPRINT(H, list->var, list->size, "ptrlist: %lX to %s << \n", (uint64_t)list);
    while (cmpfunc(list->var, *structure, list->size, *size) != 0) {
        if (list->next == NULL) {
            if (action == HH_INSERT) {
                list->next = __LL_pool_get();
                list->next->var = *structure;
                list->next->next = NULL;
                list->next->size = *size;
                list->next->content = *content;
                list->next->contentsize = *contentsize;
                LOG_HASH(
                    "[while] put %lX at %d, ptr was %lX \n", 
                    (uint64_t)*structure, 
                    hash, 
                    (uint64_t)list
                );
                H->size++;
                LOG_HASH("Hashtable has %ld elements\n", H->size);
                return INSERTED;
            }
            *content = NULL;
            *contentsize = 0;
            return NOT_IN_HASH_TABLE;
        }
        backptr = list;
        list = list->next;
    }
    // found
    if (action == HH_INSERT) {

        ptrtmp = list->content;
        list->content = *content;
        *content = ptrtmp;

        size_ttmp = list->contentsize;
        list->contentsize = *contentsize;
        *contentsize = size_ttmp;

    } else if (action == HH_FIND) {
        
        *content = list->content;
        *contentsize = list->contentsize;

    } if (action == HH_REMOVE) {
        LOG_HASH("REMOVING %lX\n", (uint64_t)list->var);
        if (backptr == list) {
            // this is the first element
            H->vector[hash] = list->next;
        } else {
            backptr->next = list->next; // TODO isso nao funciona
        }
        *structure = list->var;
        *size = list->size;

        list->next = NULL;
        list->var = NULL;
        list->size = 0;
        H->size--;
        __LL_pool_release(list); // give it up to the pool
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
        __ptr_LL_pool__[0] = __LL_pool__;
        __nptr_LL_pool__ = 1;
        __LL_pool__->next = NULL;
    }
    assert(__LL_pool__ != NULL);
    if (__LL_pool__->next == NULL) {
        LLHashTable *node;
        LOG_HASH("Alocating %d elements for the pool!\n", size_to_alloc);
        node = malloc(sizeof(LLHashTable) * size_to_alloc);
        __ptr_LL_pool__[__nptr_LL_pool__++] = node;
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

void freepool() {
    int i;
    for (i = 0; i < __nptr_LL_pool__; i++) {
        free(__ptr_LL_pool__[i]);
    }
}


void freelinkedlist(Hashtable* H, LLHashTable* node) {
    if (node != NULL) {
        freelinkedlist(H, node->next);

        H->freevar(node->var, node->size);
        node->var = NULL;
        node->size = 0;

        H->freecontent(node->content, node->contentsize);
        node->content = NULL;
        node->contentsize = 0;

        __LL_pool_release(node);
    }
}

void freehashtable(Hashtable* H) {
    size_t i;
    uint64_t numberempty = 0;
    for (i = 0; i < (1LL<<HASHBITSIZE_S); i++) {
        if (H->vector[i] == NULL) {
            numberempty++;
        }
        freelinkedlist(H, H->vector[i]);
    }
    LOG_HASH("There were %ld empty slots on the hashtable\n", numberempty);
    free(H);
}
