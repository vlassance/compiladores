#include <stdio.h>
#include "lex.h"

int main(int argc, char *argv[]) {
    FILE *input_file;
    Token* tk;

    if (argc <= 1) {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  %s <input file>\n", argv[0]);
        return 1;
    }

    initialize_lex();
    
    input_file = fopen(argv[1], "r");

    while (next_useful_token(input_file, &tk) && tk != NULL) {
       print_token(tk); 
    }
	
	print_identifiers();

    if (tk == NULL) 
        return 0;
    return 1;
}
