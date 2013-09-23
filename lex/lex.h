#ifndef LEX_PCS2056
# define LEX_PCS2056
# define MAX_NUM_TRANSITIONS 50
# define MAX_NUM_STATES 50
# define MAXLENGTHSTATESTR 50
# define ENCODING_MAX_CHAR_NUM 256 
# define MAX_SIZE_OF_A_TOKEN 2048
typedef struct state {
    char* name;
    int number_of_transitions;
    long* masks[MAX_NUM_TRANSITIONS];
    struct state* transitions[MAX_NUM_TRANSITIONS];
} state;

typedef struct token {
    long line;
    long column;
    long size;
    state* origin_state;
    char* str;
} token;

int _number_of_states;

state* state_table[MAX_NUM_STATES];
char buff_token[MAX_SIZE_OF_A_TOKEN];
long buff_token_end;

#endif
