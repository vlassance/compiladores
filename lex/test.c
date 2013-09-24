#include <stdio.h>
#include "lex.h"

int main() {
    FILE *lex_file, *input_file;
    Token* tk;
    _number_of_states = 0;
    lex_file = fopen("./languagefiles/lang.lex", "r");
    while (lex_parser_read_char(lex_file)) {
    }
    print_all_states();
    
	input_file = fopen(".languagefiles/ex.czar", "r");
	while (next_token(input_file, &tk) && tk != NULL) {
       print_token(tk); 
    }
	
    return 0;
}
