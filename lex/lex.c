#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lex.h"

void state_from_name(char* statename, State** st) {
    int i, size;
    char* pch;
    for (i = 0; i < _number_of_states; i++) {
        if (strcmp(statename, state_table[i]->name) == 0) {
            *st = state_table[i];
            return;
        }
    }
    state_table[_number_of_states] = malloc(sizeof(State));
    state_table[_number_of_states]->name = malloc(
        sizeof(char) * (strlen(statename) + 1)
    );

    strcpy(state_table[_number_of_states]->name, statename);

    pch = strrchr(statename, '_');
    if (!pch) {
        size = strlen(statename);
    } else {
        size = pch - statename;
    }
    state_table[_number_of_states]->class_name = malloc(
        sizeof(char) * (size + 1)
    );
    strncpy(state_table[_number_of_states]->class_name, statename, size);
    *st = state_table[_number_of_states++];
}

void add_mask_to_state(State** from, State** to, long* mask) {
    (*from)->masks[(*from)->number_of_transitions] = mask;
    (*from)->transitions[(*from)->number_of_transitions++] = *to;
}

void print_state(State* st) {
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
                printf("%ld ", cod);
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
    State *from;
    State *to;

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

void print_token(Token* t) {
    printf("> [%s]", t->class_name);
    printf(" >>%s<<", t->str);
    printf(" at (%ld, %ld), with size %ld\n", t->line, t->column, t->size);
}

void find_next_state_from_char(char c, State** from, State** to) {
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

int next_useful_token(FILE* f, Token** t) {
    int res, i;
    
    do {
        res = next_token(f, t);
    } while(
        *t != NULL && 
        res && 
        strcmp((*t)->origin_state->class_name, "SPACE") == 0
    );

    if (!res || *t == NULL){
        return res;
    }

    if (strcmp((*t)->origin_state->class_name, "IDENT") == 0) {
        for (i = 0; i < vkeywords_size; i++) {
            if (strcmp((*t)->str, vkeywords[i]) == 0) {       
                break;
            }
        }
        if (i == vkeywords_size) {
            (*t)->class_name = malloc(6 * sizeof(char));
            strcpy((*t)->class_name, "IDENT");
        } else {
            (*t)->class_name = malloc(9 * sizeof(char));
            strcpy((*t)->class_name, "RESERVED");
        }
    } else {
        (*t)->class_name = malloc(
            (strlen((*t)->origin_state->class_name) + 1) * sizeof(char)
        );
        strcpy((*t)->class_name, (*t)->origin_state->class_name);
    }
    // to be sure that this will not be used
    (*t)->origin_state = NULL;
    return res;
}

int next_token(FILE* f, Token** t) {
    static State *current_state = NULL;
    static long cline = 1;
    static long ccolumn = 0;
    static long line = 1;
    static long column = 1;
    static char tmpend = 1;
    char next_c;

    State* next_state; 

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
            ccolumn = 0;
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
            (*t) = malloc(sizeof(Token));
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
                fprintf(
                    stderr,
                    "buff_token: <%s>, error at line %ld column %ld\n", 
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
           fprintf(
               stderr,
                "buff_token: <%s>, error at line %ld column %ld\n", 
                buff_token, 
                cline, 
                ccolumn
            );
            return 0; 
        }
        current_state = next_state;
    } while (tmpend != EOF);
    (*t) = NULL;
    return 1;
}

void initialize_lex() {
    FILE *lex_file, *keywords_file;
    vkeywords_size = 0;

    lex_file = fopen("./languagefiles/lang.lex", "r");
    keywords_file = fopen("./languagefiles/keywords.txt", "r");

    while (lex_parser_read_char(lex_file)) {
    }
    while (fscanf(keywords_file, " %s", buff_token) != EOF) {
        vkeywords[vkeywords_size] = malloc(sizeof(char) * (strlen(buff_token) + 1L));
        strcpy(vkeywords[vkeywords_size++], buff_token);
    }
    //print_all_states();
}
