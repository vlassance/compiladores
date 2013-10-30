#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "automata.h"
#include "../lex/lex.h"


int main() {
    Automaton a;
    read_mdfa(&a, "ATOMO", 0U, fopen("scripts/output/VARIDENT.mdfa", "r"));
    print_automaton(&a, stdout);
    free_automaton(&a);
    read_all_syn_files();
    
    return 0;
}
