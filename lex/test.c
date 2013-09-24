#include <stdio.h>
#include "lex.h"

int main() {
    FILE *input_file;
    Token* tk;
    _number_of_states = 0;
	initialize_lex();
    
	input_file = fopen("../languagefiles/ex.czar", "r");
	while (next_useful_token(input_file, &tk) && tk != NULL) {
       print_token(tk); 
    }
	
    return 0;
}
