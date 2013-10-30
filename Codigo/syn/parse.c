

int main() {
    read_all_syn_files();
    FILE *input_file;
    Token* tk;
    uint32_t state;

    if (argc <= 1) {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  %s <input file>\n", argv[0]);
        return 1;
    }

    initialize_lex();
    
    input_file = fopen(argv[1], "r");

    while (next_useful_token(input_file, &tk) && tk != NULL) {
        while(followState(tk, a, &state) == 0) {
            // following without reading
        }
    }
	
    print_identifiers();
    if (tk == NULL) 
        return 0;
    return 1;
}
