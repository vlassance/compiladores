#include "strhashtable.h"
#include "hashtable.h"

#include <stdio.h>
#include <assert.h>
int main() {
    Hashtable* ht = newStrHashTable();     
    int i;
    char str[10] = "thisis0";

    for (i = 0; i < 900; i++) {
        str[6] = '0'+(i%10);
        strhh_insert(ht, str);
    }
    printf("All\n");
    printAll(ht);

    for (i = 0; i < 900; i++) {
        str[6] = '0'+(i%10);
        strhh_remove(ht, str);
    }
    printf("none\n");
    printAll(ht);

    strhh_insert(ht, "Test");
    strhh_insert(ht, "TesT");
    strhh_insert(ht, "TeSt");
    strhh_insert(ht, "TEst");
    strhh_insert(ht, "test");

    assert(strhh_contains(ht, "Test"));

    assert(strhh_contains(ht, "Test"));
    assert(strhh_contains(ht, "TEst"));
    assert(strhh_contains(ht, "TesT"));
    assert(strhh_contains(ht, "TeSt"));
    assert(!strhh_contains(ht, "TEST"));
    assert(!strhh_contains(ht, "T778ST"));
    printf("Test\n");
    printAll(ht);

    return 0;
}
