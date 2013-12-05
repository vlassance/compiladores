#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "automata.h"
#include "../lex/lex.h"


int main() {
    read_all_syn_files(); 
    free_automata();
    return 0;
}
