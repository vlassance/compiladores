#ifndef LEX_PCS2056

# define LEX_PCS2056
# define MAX_NUM_TRANSITIONS 50
# define MAX_NUM_STATES 50
# define MAXLENGTHSTATESTR 50
# define ENCODING_MAX_CHAR_NUM 256 
# define MAX_SIZE_OF_A_TOKEN 2048

typedef struct State {
    char* name;
    int number_of_transitions;
    long* masks[MAX_NUM_TRANSITIONS];
    struct State* transitions[MAX_NUM_TRANSITIONS];
} State;

typedef struct Token {
    long line;
    long column;
    long size;
    State* origin_state;
    char* str;
} Token;

int _number_of_states;

State* state_table[MAX_NUM_STATES];
char buff_token[MAX_SIZE_OF_A_TOKEN];
long buff_token_end;

void state_from_name(char* statename, State** st);
void add_mask_to_state(State** from, State** to, long* mask);
void print_state(State* st);
void print_all_states();
int lex_parser_read_char(FILE* f);
void print_token(Token* t);
void find_next_state_from_char(char c, State** from, State** to);
int next_token(FILE* f, Token** t);

#endif
