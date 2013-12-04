#include <stdio.h>
#include "automata.h"
#include "../lex/lex.h"

int main(int argc, char** argv) {
    read_all_syn_files();
    FILE *input_file;
    Token* tk;
    uint32_t state;
    Automaton* a;
    a = automata_list + automaton_program_id;
    if (argc <= 1) {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  %s <input file>\n", argv[0]);
        return 1;
    }
    printf("Parsing file <%s>\n\n\n", argv[1]);
    initialize_lex();
    
    input_file = fopen(argv[1], "r");
    tk = NULL;
    state = a->initial_state;
    while (next_useful_token(input_file, &tk) && tk != NULL) {
        while(syn(tk, &a, &state) == 0) {
            //printf("-> %s, %d \n", a->name, state);
            // following without reading
        }
        //printf("-> %s, %d \n", a->name, state);
        //printf("read %s\n", tk->str);
    }      
    while(syn(tk, &a, &state));

    print_identifiers();
    free_automata();
    freeLex();
    fclose(input_file);
    if (tk == NULL) 
        return 0;
    return 1;
}
