#include <stdint.h>
#ifndef AUTOMATA_H
# define AUTOMATA_H
# define INVALID_AUT_ID 0xFFFFFFFF
# define MAX_AUT_STATES 100
# define MAX_AUT_STRING_SIZE 100
# define MAX_AUT_LENGTH_STACK 10000
# define MAX_AUTOMATA 100
# define PUTMAX(__x__,__y__) __x__=(__x__>__y__)?(__x__):(__y__)
# define HARDCOPYSTR(__dest__,__orig__) \
    if (__orig__ == NULL || __orig__[0] == '\0') { \
        __dest__ = NULL;\
    } else {\
        __dest__=malloc(strlen(__orig__)+1UL);\
        strcpy(__dest__,__orig__);\
    }
# define AUT_FINAL_CHAR ' '

typedef struct {
    uint32_t id;
    uint32_t initial_state;
    uint32_t* final_states;
    uint32_t nfinal_states;
    char* name;
    uint32_t nstates;

    char** calls; // ["IDENT, NULL, "INT", ...] 
    uint32_t* push_back; // [2, FFF, 3, ...]   0 -(IDENT)->2 0 -(INT)->3
    // this could be a skip list :D
    char*** transitions_str; // ["(", NULL, .. ]
    uint32_t** transitions;  // [5, FFFF, .. ]  0 -("(")->5
    uint32_t* size_transitions; 
} Automaton;

char lista_constantes[250][500];
int size_lista_constantes;
char lista_variaveis[250][500];
int size_lista_variaveis;
char lista_sentencas[250][500];
int size_lista_sentencas;
char lista_clausulas[250][500];
int size_lista_clausulas;

int lista_fatos[250];
int size_lista_fatos;
int meta;

char sentenca[500];

uint32_t automaton_program_id;
uint32_t automata_len;
Automaton automata_list[MAX_AUTOMATA]; 
uint64_t automata_stack[MAX_AUT_LENGTH_STACK];
uint32_t automata_stack_size;
char** finals;
#endif
