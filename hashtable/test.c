#include "strhashtable.h"
#include "hashtable.h"

#include <stdio.h>
#include <assert.h>

void gen_random(char *s, const int len) {
    int i;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "_+-@[](){}";

    for (i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}



int main() {
    Hashtable* ht = newStrHashTable();
    char buff[100];
    int i;
    char str[10] = "thisis0";
    char object[90] = "this is a shared object";
    size_t sizeobject = sizeof(char) * 90;

    for (i = 0; i < 18; i++) {
        str[6] = '0'+(i%10);
        strhh_insert(ht, str, (void**)&object, &sizeobject);
    }
    printf("All\n");
    printAll(ht);

    for (i = 0; i < 18; i++) {
        str[6] = '0'+(i%10);
        strhh_remove(ht, str, (void**)&object, &sizeobject); 
    }
    printf("none\n");
    printAll(ht);

    strhh_insert(ht, "Test", (void**)&object, &sizeobject);
    strhh_insert(ht, "TesT", (void**)&object, &sizeobject);
    strhh_insert(ht, "TeSt", (void**)&object, &sizeobject);
    strhh_insert(ht, "TEst", (void**)&object, &sizeobject);
    strhh_insert(ht, "test", (void**)&object, &sizeobject);

    assert(strhh_contains(ht, "Test", (void**)&object, &sizeobject));
    assert(strhh_contains(ht, "Test", (void**)&object, &sizeobject));
    assert(strhh_contains(ht, "TEst", (void**)&object, &sizeobject));
    assert(strhh_contains(ht, "TesT", (void**)&object, &sizeobject));
    assert(strhh_contains(ht, "TeSt", (void**)&object, &sizeobject));
    assert(!strhh_contains(ht, "TEST", (void**)&object, &sizeobject));
    assert(!strhh_contains(ht, "T778ST", (void**)&object, &sizeobject));
    printf("Test\n");
    printAll(ht);

    

    for (i = 0; i < 90000; i++) {
        gen_random(buff, 90);
        strhh_insert(ht, buff, (void**)&object, &sizeobject);
    }

    strhh_remove(ht, "|||blubla##", (void**)&object, &sizeobject);
    


    
    freehashtable(ht);
    freepool();
    return 0;
}
