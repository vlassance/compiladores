#include "hashtable.h"

#ifndef HASHTABLE_STR
# define HASHTABLE_STR

Hashtable* newStrHashTable(); 
int strhh_insert(Hashtable* H, char* str, void** content, size_t* contentsize);
int strhh_remove(Hashtable* H, char* str, void** content, size_t* contentsize);
int strhh_contains(Hashtable* H, char* str, void** content, size_t* contentsize);
void printAll(Hashtable* H);

#endif

