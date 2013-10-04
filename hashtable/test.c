#include "strhashtable.h"
#include "hashtable.h"

#include <stdio.h>

int main() {
    Hashtable* ht = newStrHashTable();     
    int i;
    char str[10] = "thisis0";
    for (i = 0; i < 900; i++) {
        str[6] = '0'+(i%10);
        strhh_insert(ht, str);
    }
    printAll(ht);

//int strhh_remove(Hashtable* H, char* str);
//int strhh_contains(Hashtable* H, char* str);


    return 0;
}
