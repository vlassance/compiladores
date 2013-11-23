#include <stdio.h>
#include "syn/automata.h"
#include "lex/lex.h"

int version = .5;

void print_note(FILE* f) {

    fprintf(f, "Escola Politecnica da Universidade de Sao Paulo\n");
    fprintf(f, "===============================================\n");
    fprintf(f, "CZAR Compiler for MVN\n");
    fprintf(f, "Version: %1.1f\n", version);

}


int main(int argc, char** argv) {
    FILE *input_file;
    Token* tk;
    uint32_t state;
    Automaton* a;
    read_all_syn_files();
    print_note();
    a = automata_list + automaton_program_id;
    if (argc <= 1) {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  %s <input file>\n", argv[0]);
        return 1;
    }
    initialize_lex();
    printf("Lex Initialized\n");
    printf("Compiling File: %s\n\n\n", argv[1]);
    input_file = fopen(argv[1], "r");
    tk = NULL;
    state = a->initial_state;
    while (next_useful_token(input_file, &tk) && tk != NULL) {
        while(syn(tk, &a, &state) == 0) { 
        }
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
