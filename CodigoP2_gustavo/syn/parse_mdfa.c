#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include "automata.h"
#include "../lex/lex.h"


uint32_t automaton_pop(Automaton** a) {
    if (automata_stack_size == 0) {
        fflush(stdout);
        fprintf(stderr, "\nError: Unexpected token at the end of file.\n");
        exit(1);
    }
    uint64_t s = automata_stack[--automata_stack_size];
    (*a) = automata_list + (s>>32);
    return s&((1L<<32)-1L);
}

void automaton_push_back(Automaton* a, uint32_t state) {
    uint64_t s = ((uint64_t)(a->id))<<32;
    s|=(uint64_t)state;
    automata_stack[automata_stack_size++] = s;
}

void read_finals(FILE* f) {
    char tmpfinals[1000][1000];
    int i = 0;
    tmpfinals[i][0] = '\0';
    while(fscanf(f, "%s", tmpfinals[i]) && strlen(tmpfinals[i]) > 0) {
        i++; 
        tmpfinals[i][0] = '\0';
    }
    finals = malloc(sizeof(char*) * (i + 1));
    finals[i] = NULL;
    while(--i >= 0) {
        HARDCOPYSTR(finals[i], tmpfinals[i]);
    }
}

void read_mdfa(Automaton* a, char* name, uint32_t id, FILE* f) {
    uint32_t node, dest;
    uint32_t i, j;
    char chartmp;
    char* ptrtmp;
    char tmp[1000];
    static char tmpid[MAX_AUT_STRING_SIZE];

    static char calls[MAX_AUT_STATES][MAX_AUT_STRING_SIZE];
    static char transitions_str[MAX_AUT_STATES][MAX_AUT_STATES][MAX_AUT_STRING_SIZE];
    static uint32_t transitions[MAX_AUT_STATES][MAX_AUT_STATES];
    static uint32_t push_back[MAX_AUT_STATES];
    static uint32_t size_transitions[MAX_AUT_STATES];
    static uint32_t final_states[MAX_AUT_STATES];
    for (i = 0; i < MAX_AUT_STATES; i++) {
        calls[i][0] = '\0';
        push_back[i] = INVALID_AUT_ID;
        size_transitions[i] = 0U;
        final_states[i] = 0U;
        for (j = 0; j < MAX_AUT_STATES; j++) {
            transitions_str[i][j][0] = '\0';
            transitions[i][j] = INVALID_AUT_ID; 
        }
    }

    a->name = malloc(strlen(name) + 1U);
    strcpy(a->name, name);
    a->id = id;
    a->nstates = 0;
    fscanf(f, " %*s %d", &(a->initial_state));
    PUTMAX(a->nstates, a->initial_state);
    fscanf(f, " %s %d", tmp, &dest);
    final_states[dest] = 1U;
    PUTMAX(a->nstates, dest);
    while (fscanf(f, "%c", &chartmp) && chartmp != '\n') {
        if (chartmp == ',') {
            fscanf(f, "%d", &dest);
            final_states[dest] = 1U;
            PUTMAX(a->nstates, dest);
        }

    }

    while(fscanf(f, " (%d, %s -> %d", &node, tmpid, &dest) != EOF) {
        PUTMAX(a->nstates, node);
        PUTMAX(a->nstates, dest);
        if (tmpid[0] == '"') {
            sscanf(
                tmpid, 
                "\"%[^\"]", 
                transitions_str[node][size_transitions[node]]
            );
            transitions[node][size_transitions[node]] = dest;

            size_transitions[node]++;
            if (size_transitions[node] > 20) { // XXX remove
                fflush(stdout);
                exit(1);
            }
        } else {
            i = 0;
            while (tmpid[++i] != ')') {
                // nothing
            }
            tmpid[i] = '\0';
            i = 0;
            while (finals[i] != NULL) {
                if (strcmp(finals[i], tmpid) == 0) {
                    ptrtmp = transitions_str[node][size_transitions[node]];
                    ptrtmp[0] = AUT_FINAL_CHAR;
                    strcpy(ptrtmp + 1UL, tmpid);
                    transitions[node][size_transitions[node]] = dest;
                    size_transitions[node]++; 
                    break;
                }
                i++;
            }
            if (finals[i] == NULL) {
                if (calls[node][0] != '\0') {
                    fprintf(stderr,  
                        "Non Deterministic (%s)\n   calls[%d] was: %s, trying to assign to %s\n", 
                        name,
                        node, 
                        calls[node], 
                        tmpid
                    );
                    fprintf(stderr, "Non Deterministic Automaton.\n");
                    exit(1);
                } 
                strcpy(calls[node],tmpid);
                push_back[node] = dest;
            }
        }
    }
    a->nstates++;
    a->calls = malloc(sizeof(char*) * a->nstates); // malloc strings
    a->push_back = malloc(sizeof(uint32_t) * a->nstates);
    a->transitions = malloc(sizeof(uint32_t*) * a->nstates);
    a->transitions_str = malloc(sizeof(char**) * a->nstates);
    a->final_states = malloc(sizeof(uint32_t) * a->nstates);
    a->size_transitions = malloc(sizeof(uint32_t) * a->nstates);

    for (i = 0; i < a->nstates; i++) {
        a->size_transitions[i] = size_transitions[i];
        a->transitions[i] = malloc(sizeof(uint32_t) * size_transitions[i]);
        a->transitions_str[i] = malloc(sizeof(char*) * size_transitions[i]);
    }
    for (i = 0; i < a->nstates; i++) {
        // Calls
        HARDCOPYSTR(a->calls[i], calls[i]);
        // push_back
        a->push_back[i] = push_back[i];
        // transitions
        for (j = 0; j < a->size_transitions[i]; j++) {
            a->transitions[i][j] = transitions[i][j];
            HARDCOPYSTR(a->transitions_str[i][j], transitions_str[i][j]);
        }
        // final_states
        a->final_states[i] = final_states[i];
    }
}

void print_automaton(Automaton* a, FILE* f) {
    int i, j;
    int first = 1;
    fprintf(f, "---------------------------------\n");
    fprintf(f, "ID(%d, %s)\n\n", a->id, a->name);
    fprintf(f, "initial: %d\n", a->initial_state);
    fprintf(f, "final: ");
    for (i = 0; i < a->nstates; i++) {
        if (a->final_states[i]) {
            if (!first) {
                fprintf(f, ", ");
            }
            first = 0;
            fprintf(f, "%d", i);
        }
    }
    fprintf(f, "\n");
    for (i = 0; i < a->nstates; i++) {
        if (a->calls[i] != NULL) {
            fprintf(f, "(%d, %s) -> %d\n",i ,a->calls[i], a->push_back[i]);
        } 
        {
            for (j = 0; j < a->size_transitions[i]; j++) {
                fprintf(f, "(%d \"%s\") -> %d\n", i, a->transitions_str[i][j], a->transitions[i][j]);
            }
        }
    }



    fprintf(f, "---------------------------------\n");
    fflush(f);
}

void free_automaton(Automaton* a) {
    uint64_t i, j;
    free(a->name);
    free(a->push_back);
    for (i = 0; i < a->nstates; i++) {
        free(a->calls[i]);
        for (j = 0; j < a->size_transitions[i]; j++) {
            free(a->transitions_str[i][j]);
        }
        free(a->transitions_str[i]); 
        free(a->transitions[i]);
    }
    free(a->size_transitions);
    free(a->transitions_str);
    free(a->transitions);
    free(a->calls);
    free(a->final_states);
    a = NULL;

}

void read_syn_file(char* dir, char* file) {
    FILE* f;
    char tmp[200];
    char nametmp[200];

    sprintf(tmp, "%s%s", dir, file);
    fflush(stdout);
    int i = 0;
    while (file[i] != '.'){
        nametmp[i] = file[i];
        i++;
    }
    nametmp[i] = '\0';

    f = fopen(tmp, "r");
    if (strcmp(nametmp, "PROGRAM") == 0) {
        automaton_program_id = automata_len;
    }
    read_mdfa(&(automata_list[automata_len]), nametmp, automata_len, f);
    fclose(f);
    print_automaton(&(automata_list[automata_len]), stdout);
    automata_len++;
}

void read_all_syn_files() {
    DIR *dp;
    struct dirent *ep;
    int lenname;
    FILE* f;

    f = fopen("./scripts/FINALS.txt", "r");
    read_finals(f);
    fclose(f);

    dp = opendir ("./scripts/output/");
    automata_len = 0;
    if (dp != NULL) {
        while (ep = readdir (dp)){
            lenname = strlen(ep->d_name);
            if (strcmp(ep->d_name + lenname - 5, ".mdfa") == 0) {
                read_syn_file("./scripts/output/", ep->d_name);
            }
        }    
        (void) closedir (dp);
    } else {
        perror ("Couldn't open the directory");
    }
}

void free_automata() {
    int i;
    for (i = 0; i < automata_len; i++) {
        free_automaton(&(automata_list[i]));
    }
    i = 0;
    while (finals[i] != NULL) {
        free(finals[i]);
        i++;
    }
    free(finals);
}

uint32_t findAutomatonByName(char* name) {
    int i;
    for (i = 0; i < automata_len; i++) {
        if (strcmp(name, automata_list[i].name) == 0) {
            return i;
        }
    }
    return INVALID_AUT_ID;
}

uint32_t poponly(Automaton** a, uint32_t* state) {
    if ((*a)->final_states[*state]) {
        if (strcmp((*a)->name, "PROGRAM") == 0) {
            printf("Desempilhou PROGRAM em %d com stack: %d", 
                *state, automata_stack_size);
            if (automata_stack_size >= 1) {
                fprintf(stderr, "Stack wasn't empty.\n");
                exit(1);
            }
            
            return 0;
        }
        printf(
            "Desempilhou: (automato: %s, estado:%d) -> ", 
            (*a)->name, *state
        );
        (*state) = automaton_pop(a);
        printf(
            "(automato: %s, estado:%d)\n", 
            (*a)->name, *state
        );

        return 1; // ok
    }
    fprintf(stderr, "Ooops, can't pop.\n"); 
    exit(1);
}

void semantico_tbd() {
    printf("TODO\n");
}

uint32_t syn(Token* tk, Automaton** a, uint32_t* state) {
    if (tk == NULL) {
        semantico_tbd();
        return poponly(a, state);
    }
    uint32_t i;
    char* strtrans;
    for (i = 0U; i < (*a)->size_transitions[*state]; i++) {
        strtrans = (*a)->transitions_str[*state][i];
        if (strcmp(strtrans, tk->str) == 0) {
            printf(
                "Leu: %s (automato: %s, estado:%d) -> (automato: %s, estado:%d)\n", 
                strtrans, (*a)->name, *state, (*a)->name, (*a)->transitions[*state][i]
            );
            *state =  (*a)->transitions[*state][i];
            return 1; // read
        }
    }

    for (i = 0U; i < (*a)->size_transitions[*state]; i++) {
        strtrans = (*a)->transitions_str[*state][i];
        if (strtrans[0] == AUT_FINAL_CHAR) {
            if (strcmp(strtrans + 1UL, tk->class_name) == 0) {
                printf(
                    "Leu: <%s> (automato: %s, estado:%d) -> (automato: %s, estado:%d)\n", 
                    strtrans+1, (*a)->name, *state, (*a)->name, (*a)->transitions[*state][i]
                );
                *state = (*a)->transitions[*state][i];
                return 1;
            }
        }
    }

    if ((*a)->calls[*state] != NULL) {

        automaton_push_back((*a), (*a)->push_back[*state]);
        i = findAutomatonByName((*a)->calls[*state]);

        if (i == INVALID_AUT_ID) {
            fprintf(stderr, "Name (%s) wasn't found\n",(*a)->calls[*state]);
            fflush(stderr);
            fprintf(stderr, "Internal Error, Invalid automata.\n");
            exit(1);
        }

        printf(
            "Empilhou: (automato: %s, estado:%d) -> (automato: %s, estado:%d)\n", 
            (*a)->name, *state, ((automata_list + i))->name, ((automata_list+i)->initial_state) 
        );
        *a = automata_list + i;
        *state = (*a)->initial_state;
        return 0; // didn't read
    }

    if ((*a)->final_states[*state]) {
        printf(
            "Desempilhou: (automato: %s, estado:%d) -> ", 
            (*a)->name, *state
        );
        (*state) = automaton_pop(a);
        printf(
            "(automato: %s, estado:%d)\n", 
            (*a)->name, *state
        );
        semantico_tbd();
        return 0; // didn't read
    }
    fprintf(stderr, "Automata(%s, %d), token(%s, %s)\n", (*a)->name, *state, tk->str, tk->class_name);
    fprintf(stderr, "Expecting one of:\n");

    for (i = 0U; i < (*a)->size_transitions[*state]; i++) {
        fprintf(stderr, "  \"%s\"\n", (*a)->transitions_str[*state][i]);
    }
    fprintf(stderr, "Found:\n");
    print_token(tk);
    fflush(stderr);
    fprintf(stderr, "Syntax error.\n");
    exit(1);

}
