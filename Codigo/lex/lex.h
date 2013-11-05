#ifndef LEX_PCS2056

# define LEX_PCS2056

# define MAX_NUM_TRANSITIONS 50
# define MAX_NUM_STATES 50
# define MAXLENGTHSTATESTR 50
# define ENCODING_MAX_CHAR_NUM 256 
# define MAX_SIZE_OF_A_TOKEN 2048
# define MAX_NUMBER_OF_KEYWORDS 256
# define MAX_NUMBER_OF_IDENTIFIERS 2048
# define MAX_NUMBER_OF_ESCOPOS 512
# define FREEANDNULL(__x__) if ((__x__) != NULL) {free(__x__); (__x__) = NULL;};


typedef enum { false, true } bool;

typedef struct State {
    char* name;
    char* class_name;
    int number_of_transitions;
    long* masks[MAX_NUM_TRANSITIONS];
    struct State* transitions[MAX_NUM_TRANSITIONS];
} State;

typedef struct Token {
    long line;
    long column;
    long size;
    char* class_name;
    State* origin_state;
    char* str;
} Token;

typedef struct Escopo {
    long level;
    char* videntifiers[MAX_NUMBER_OF_IDENTIFIERS];
    long videntifiers_size;
} Escopo;

int _number_of_states;

State* state_table[MAX_NUM_STATES];
char buff_token[MAX_SIZE_OF_A_TOKEN];
long buff_token_end;


char* vkeywords[MAX_NUMBER_OF_KEYWORDS];
long vkeywords_size;
long escopo_atual;
Escopo* vescopos[MAX_NUMBER_OF_ESCOPOS];
long vescopos_size;

void initialize_lex();
int next_useful_token(FILE* f, Token** t);
void print_token(Token* t);

#endif
