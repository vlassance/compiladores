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
    printf(" ");
    for (i = 0; i < st->number_of_transitions; i++) {   
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
    state *from;
    state *to;

    long masktermsize = sizeof(long) * 8; // number of byts on a long

    scanf(" %c", &sep);
    if (sep == EOF) {
        return 0;
    }
     
    mask = malloc(ENCODING_MAX_CHAR_NUM / (8));
    for (i = 0; i < ENCODING_MAX_CHAR_NUM / (8 * sizeof(long)); i++) {
        mask[i] = 0L;
    }
    while (scanf("%c", &c) && c != sep && c != EOF) {
        cod = (long) c;
        maskterm = cod / masktermsize;
        maskdepl = cod % masktermsize;
        mask[maskterm] |= (1L<<maskdepl);
    }
    scanf(" %s", fromname);
    state_from_name(fromname, &from);
    scanf(" %s", toname);
    state_from_name(toname, &to);
    add_mask_to_state(&from, &to, mask);
    return 1;
}
