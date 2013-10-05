#include <stdlib.h>
#include <stdint.h>
#ifndef HASHTABLE_S
# define HASHTABLE_S

// Call gcc with -D VERBOSITY_HASH to activate verbose mode
# ifdef VERBOSITY_HASH 
char* NODE_P__ptr;
#  define LOG_HASH(...); fprintf(stderr, __VA_ARGS__);
#  define NODE_PRINT(__Htable__,__struct__,__size__);  \
    NODE_P__ptr = __Htable__->tostrfunc(__struct__,__size__); \
    LOG_HASH("%s", NODE_P__ptr); \
    __Htable__->posttostrfunc(__struct__,__size__,NODE_P__ptr);

#  define NODE_REPRINT(__Htable__,__struct__,__size__,...);  \
    NODE_P__ptr = __Htable__->tostrfunc(__struct__,__size__); \
    LOG_HASH(__VA_ARGS__,NODE_P__ptr); \
    __Htable__->posttostrfunc(__struct__,__size__,NODE_P__ptr);

# else  
#  define LOG_HASH(...); 
#  define NODE_PRINT(__Htable__,__struct__,__size__);  
#  define NODE_REPRINT(__Htable__,__struct__,__size__,...);  
# endif


# define HASHBITSIZE_S 20
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
    uint64_t size;
    int (*cmpfunc)(void* a, void* b, size_t sizea, size_t sizeb);
    char* (*tostrfunc)(void* a, size_t size);
    void (*posttostrfunc)(void* a, size_t size, char* str);
    void (*freevar)(void* var, size_t size);
    void (*freecontent)(void* content, size_t size);
} Hashtable;

LLHashTable* __LL_pool__;
LLHashTable* __ptr_LL_pool__[70];
int __nptr_LL_pool__;

int __hh_doaction(
    Hashtable* H, 
    void** structure, 
    size_t* size, 
    void** content, 
    size_t* contentsize, 
    int action
);

void freehashtable(Hashtable* H);

void freepool();

void __LL_pool_release(LLHashTable* node);

LLHashTable* __LL_pool_get();


#endif  
