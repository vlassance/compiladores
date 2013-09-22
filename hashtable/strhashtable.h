#include "hashtable.h"

#ifndef HASHTABLE_STR
# define HASHTABLE_STR

Hashtable* newStrHashTable(); 
int strhh_insert(Hashtable* H, char* str);
int strhh_remove(Hashtable* H, char* str);
int strhh_contains(Hashtable* H, char* str);
void printAll(Hashtable* H);

#endif

