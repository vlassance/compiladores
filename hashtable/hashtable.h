#include <stdlib.h>
#ifndef HASHTABLE_S
# define HASHTABLE_S

# define HASHBITSIZE_S 16
# define INITIAL_SIZE_LL_POLL 4

typedef enum {WAS_THERE, NOT_IN_HASH_TABLE, INSERTED} hh_results;
typedef enum {HH_INSERT, HH_REMOVE, HH_FIND} hh_action;

typedef struct LLHashTable {
    void* var;
    void* content;
    size_t size;
    size_t contentsize;
    struct LLHashTable* next;
} LLHashTable;

typedef struct Hashtable {
    LLHashTable* vector[1<<HASHBITSIZE_S];
    int size;
    int (*cmpfunc)(void* a, void* b, size_t sizea, size_t sizeb);
} Hashtable;

LLHashTable* __LL_pool__;

int __hh_doaction(Hashtable* H, void* structure, size_t size, int action);

void __LL_pool_release(LLHashTable* node);

LLHashTable* __LL_pool_get();

#endif  
