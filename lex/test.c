#include <stdio.h>
#include "lex.h"

int main() {
    FILE* lexfile;
    token* tk;
    _number_of_states = 0;
    lexfile = fopen("./languagefiles/lang.lex", "r");
    while (lex_parser_read_char(lexfile)) {
    }
    print_all_states();
    while (next_token(stdin, &tk) && tk != NULL) {
       print_token(tk); 
    }
    return 0;
}
