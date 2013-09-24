#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "lex.h"

void state_from_name(char* statename, state** st) {
    int i;
    for (i = 0; i < _number_of_states; i++) {
        if (strcmp(statename, state_table[i]->name) == 0) {
            *st = state_table[i];
            return;
        }
    }
    state_table[_number_of_states] = malloc(sizeof(state));
    state_table[_number_of_states]->name = malloc(
        sizeof(char) * (strlen(statename) + 1)
    );
    strcpy(state_table[_number_of_states]->name, statename);
    *st = state_table[_number_of_states++];
}

void add_mask_to_state(state** from, state** to, long* mask) {
    (*from)->masks[(*from)->number_of_transitions] = mask;
    (*from)->transitions[(*from)->number_of_transitions++] = *to;
}

void print_state(state* st) {
    int i;
    long maskterm, maskdepl, cod;
    long masktermsize = sizeof(long) * 8;
    printf("[%s]\n", st->name);
    for (i = 0; i < st->number_of_transitions; i++) {   
        printf(" ");
        for (cod = 0; cod < ENCODING_MAX_CHAR_NUM; cod++) {
            maskterm = cod / masktermsize;
            maskdepl = cod % masktermsize;
            printf(
                "%c", 
                (st->masks[i][maskterm] & (1L<<maskdepl))?'1':'0'
            );
        }
        printf("\n ");
        for (cod = 0; cod < ENCODING_MAX_CHAR_NUM; cod++) {
            maskterm = cod / masktermsize;
            maskdepl = cod % masktermsize;
            if (st->masks[i][maskterm] & (1L<<maskdepl)) {
                printf("%d ", cod);
            }
        }
        printf(" -> %s\n", st->transitions[i]->name);
    }

}

void print_all_states() {
    int i;
    for (i = 0; i < _number_of_states; i++) {
        print_state(state_table[i]);
    }
}

int lex_parser_read_char(FILE* f) {
    char fromname[MAXLENGTHSTATESTR];
    char toname[MAXLENGTHSTATESTR];
    long *mask;
    char sep;
    char c;
    long cod;
    long maskterm, maskdepl;
    int i;
    long readbytes;
    state *from;
    state *to;

    long masktermsize = sizeof(long) * 8; // number of byts on a long

    if (fscanf(f, " %c", &sep) == EOF || sep == EOF) {
        return 0;
    }
     
    mask = malloc(ENCODING_MAX_CHAR_NUM / (8));
    for (i = 0; i < ENCODING_MAX_CHAR_NUM / (8 * sizeof(long)); i++) {
        mask[i] = (sep == '@')?(-1L):(0L);
    }

    while (fscanf(f, "%c", &c) && c != sep && c != EOF) {
        cod = (long) c;
        maskterm = cod / masktermsize;
        maskdepl = cod % masktermsize;
        mask[maskterm] |= (1L<<maskdepl);
    }
    fscanf(f, " %s", fromname);
    state_from_name(fromname, &from);
    fscanf(f, " %s", toname);
    state_from_name(toname, &to);
    add_mask_to_state(&from, &to, mask);
    return 1;
}

void print_token(token* t){
    printf("[%s]\n", t->origin_state->name);
    printf(" >>%s<<\n", t->str);
    printf(" at (%d, %d), with size %d\n", t->line, t->column, t->size);
}

void find_next_state_from_char(char c, state** from, state** to) {
    long masktermsize = sizeof(long) * 8; // number of byts on a long
    long cod, maskterm, maskdepl;
    int i;
    (*to) = NULL;
    cod = (long) c;
    maskterm = cod / masktermsize;
    maskdepl = cod % masktermsize;
    for (i = 0; i < (*from)->number_of_transitions; i++) {            
        if ((*from)->masks[i][maskterm] & (1L<<maskdepl)) {
            (*to) = (*from)->transitions[i];
            break; 
        }
    }
}

void happy_token(token* t) { 
}

int next_token(FILE* f, token** t){
    static state *current_state = NULL;
    static long cline = 1;
    static long ccolumn = 0;
    static long line = 1;
    static long column = 1;
    static char tmpend = 1;
    int i;
    long masktermsize = sizeof(long) * 8; // number of byts on a long
    long cod;
    long maskterm, maskdepl;
    char next_c;

    state* next_state; 

    if (tmpend == EOF) {
        (*t) = NULL;
        return 1;
    }
    if (current_state == NULL) {
        state_from_name("Q0", &current_state);
        buff_token_end = 0; 
        buff_token[0] = '\0';
    }     
    do {
        tmpend = fscanf(f, "%c", &next_c); 
        if (next_c == '\n') {
            cline++;
            ccolumn = -ccolumn;
        } else {
            if (ccolumn < 0) {
                ccolumn = 1;
            } else {
                ccolumn++;
            }
        }
        next_state = NULL;
        find_next_state_from_char(next_c, &current_state, &next_state);
        if (next_state != NULL && strcmp(next_state->name, "Q0") == 0){
            (*t) = malloc(sizeof(token));
            (*t)->str = malloc(sizeof(char) * (strlen(buff_token) + 1L));
            strcpy((*t)->str, buff_token);
            (*t)->line = line;
            (*t)->column = column;
            (*t)->origin_state = current_state;
            (*t)->size = strlen(buff_token);
            find_next_state_from_char(next_c, &next_state, &current_state);
            column = ccolumn;
            line = cline;
            buff_token[0] = next_c;
            buff_token[1] = '\0';
            buff_token_end = 1;
            if (current_state == NULL) {    
                printf(
                    "buff_token: <%s>, error at line %d column %d\n", 
                    buff_token, 
                    cline, 
                    ccolumn
                );
                return 0; 
            }
            return 1;
        } else {
            buff_token[buff_token_end++] = next_c;
            buff_token[buff_token_end] = '\0';
        }

        if (next_state == NULL) {
           printf(
                "buff_token: <%s>, error at line %d column %d\n", 
                buff_token, 
                cline, 
                ccolumn
            );
            return 0; // oops, something wrong happened TODO(gpgouveia) put this on stderr
        }
        current_state = next_state;
    } while (tmpend != EOF);
    (*t) = NULL;
    return 1;
}
