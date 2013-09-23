#include <stdio.h>
#include "lex.h"

int main() {
    _number_of_states = 0;
    while (lex_parser_read_char(stdin)) {}
    print_all_states();
    return 0;
}
