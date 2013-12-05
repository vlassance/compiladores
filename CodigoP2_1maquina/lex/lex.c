#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lex.h"

/**
 * Printing procedures
 */
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

void print_token(Token* t) {
    printf("> [%s]", t->class_name);
    printf(" >>%s<<", t->str);
    printf(" at (%ld, %ld), with size %ld\n", t->line, t->column, t->size);
}

/**
 * This is a very dummy implementation for a search'n'insert operation on 
 * a 'set'. 
 */
void state_from_name(char* statename, State** st) {
    int i, size;
    char* pch;
    // find a state that matchs, if so, return it within st
    for (i = 0; i < _number_of_states; i++) {
        if (strcmp(statename, state_table[i]->name) == 0) {
            *st = state_table[i];
            return;
        }
    }
    // malloc size of State, here we do not care about freeing states, 
    // since the lex parser will run until the life span of the compiler run,
    // the memory will be used until the end. No need to free it. 
    state_table[_number_of_states] = malloc(sizeof(State));
    memset(state_table[_number_of_states], 0, sizeof(State));
    state_table[_number_of_states]->name = malloc(
        sizeof(char) * (strlen(statename) + 1) // +1 for the \0 
    );

    strcpy(state_table[_number_of_states]->name, statename);
    // we should cut the '_' they are just different variations of the same class 
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
    state_table[_number_of_states]->class_name[size] = '\0';
    *st = state_table[_number_of_states++];
}

void add_mask_to_state(State** from, State** to, long* mask) {
    (*from)->masks[(*from)->number_of_transitions] = mask;
    (*from)->transitions[(*from)->number_of_transitions++] = *to;
}

int lex_parser_read_char(FILE* f) {
    char fromname[MAXLENGTHSTATESTR];
    char toname[MAXLENGTHSTATESTR];
    long *mask;
    char sep;
    char c;
    unsigned long cod;
    long maskterm, maskdepl;
    int i;
    State *from;
    State *to;

    long masktermsize = sizeof(long) * 8; // number of bits on a long

    if (fscanf(f, " %c", &sep) == EOF || sep == EOF) {
        return 0;
    }
    // complete mask of chars 
    mask = malloc(ENCODING_MAX_CHAR_NUM / (8));
    for (i = 0; i < ENCODING_MAX_CHAR_NUM / (8 * sizeof(long)); i++) {
        // operator that means "all transitions" (in order to simulate the 'transductor')
        mask[i] = (sep == '@')?(-1L):(0L);
    }
    // for each char different from sep, insert a transition 
    while (fscanf(f, "%c", &c) && c != sep && c != EOF) {
        cod = (unsigned long) c;
        cod &= 0x0FF;
        maskterm = cod / masktermsize;
        maskdepl = cod % masktermsize;
        mask[maskterm] |= (1L<<maskdepl);
    }
    // origin state 
    fscanf(f, " %s", fromname);
    state_from_name(fromname, &from);
    // destiny state 
    fscanf(f, " %s", toname);
    state_from_name(toname, &to);
    add_mask_to_state(&from, &to, mask);
    return 1;
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
        // search for mathing out states.
        if ((*from)->masks[i][maskterm] & (1L<<maskdepl)) {
            (*to) = (*from)->transitions[i];
            break; 
        }
    }
}

void add_identifier_to_list(char* name_ident) {
    Escopo* esc;
    int i, j;
    bool found = false;

    for (i = vescopos_size-1; i >= 0; i--) {
        esc = vescopos[i];
        if (esc->level < escopo_atual) {
            // Significa que eu não há mais idents do mesmo escopo que o atual
            break;
        }
        if (esc->level == escopo_atual) {
            for (j = 0; j < esc->videntifiers_size; j++) {
                if (strcmp(name_ident, esc->videntifiers[j]) == 0) {   
                    found = true;    
                    break;
                }
            }
        }
    }
    if (!found) {
        esc = vescopos[vescopos_size-1];
        esc->videntifiers[esc->videntifiers_size] = malloc(sizeof(char) * (strlen(name_ident) + 1L));
        strcpy(esc->videntifiers[esc->videntifiers_size++], name_ident);
    }
}

void increase_scope() {
    vescopos[++vescopos_size-1] = malloc(sizeof(Escopo));
    vescopos[vescopos_size-1]->level = ++escopo_atual;
    vescopos[vescopos_size-1]->videntifiers_size = 0;
}

void decrease_scope() {
    if (escopo_atual <= 0)
        return;

    vescopos[++vescopos_size-1] = malloc(sizeof(Escopo));
    vescopos[vescopos_size-1]->level = --escopo_atual;
    vescopos[vescopos_size-1]->videntifiers_size = 0;
}

int next_useful_token(FILE* f, Token** t) {
    int res, i;

    do {
        res = next_token(f, t);
    } while(
        *t != NULL && 
        res && 
        (strcmp((*t)->origin_state->class_name, "SPACE") == 0 || // ignore SPACES 
         strcmp((*t)->origin_state->class_name, "COMMENTS") == 0) // ignore COMMENTS
        );

    if (!res || *t == NULL){
        return res; // if error or no token, return it to the caller. 
    }

    if (strcmp((*t)->origin_state->class_name, "IDENT") == 0) {
        for (i = 0; i < vkeywords_size; i++) {
            // dummy search for keywords, this should become a hashtable
            // for the next project 
            if (strcmp((*t)->str, vkeywords[i]) == 0) {       
                break;
            }
        }
        if (i == vkeywords_size) {
            free((*t)->class_name);
            (*t)->class_name = malloc(6 * sizeof(char));
            strcpy((*t)->class_name, "IDENT");
            add_identifier_to_list((*t)->str);
        } else {
            free((*t)->class_name);
            (*t)->class_name = malloc(9 * sizeof(char));
            // name it RESERVED in case it is
            strcpy((*t)->class_name, "RESERVED");
        }
    } else {
        if (strcmp((*t)->origin_state->class_name, "DELIM") == 0) {
            if (strcmp((*t)->str, "{") == 0) {
                increase_scope();
            } else if (strcmp((*t)->str, "}") == 0) {
                decrease_scope();
            }
        }

        (*t)->class_name = malloc(
            (strlen((*t)->origin_state->class_name) + 1) * sizeof(char)
            );
        strcpy((*t)->class_name, (*t)->origin_state->class_name);
    }
    // to be sure that this will not be used
    (*t)->origin_state = NULL;
    return res;
}

void freeToken(Token* t) {
    if (t == NULL)
        return;
    FREEANDNULL(t->class_name);
    FREEANDNULL(t->str);
    FREEANDNULL(t);
}

int next_token(FILE* f, Token** t) {
    static State *current_state = NULL;
    static long cline = 1;
    static long ccolumn = 0;
    static long line = 1;
    static long column = 1;
    static char tmpend = 1;
    char next_c = EOF;

    State* next_state;
    next_state = NULL; 
    // tmpend is static, if i read something that was 
    // EOF in the last step, this is the end and I should set t to null 
    if (tmpend == EOF) {
        freeToken(*t);
        (*t) = NULL;
        return 1;
    }

    if (current_state == NULL) {
        // current_state is null, it means that this is initialization 
        // change it to Q0 and set the buffer to ""
        state_from_name("Q0", &current_state);
        buff_token_end = 0; 
        buff_token[0] = '\0';
    }

    do {
        // get char, lookahead
        tmpend = fscanf(f, "%c", &next_c); 
        if (next_c == '\n') { // column management 
            cline++;
            ccolumn = 0;
        } else {
            ccolumn++;
        }

        next_state = NULL;
        // let's see if there's a defined next state 
        find_next_state_from_char(next_c, &current_state, &next_state);
        // if next state is Q0, it means that this is acceptation, 
        // we should stop, go to Q0 and reevaluate the transition. 
        //  Since the transductor have an empty transition to Q0, we are
        //  obligated to do so.
        if (next_state != NULL && strcmp(next_state->name, "Q0") == 0){
            freeToken(*t);
            (*t) = malloc(sizeof(Token));
            (*t)->class_name = NULL;
            (*t)->str = malloc(sizeof(char) * (strlen(buff_token) + 1L));
            strcpy((*t)->str, buff_token);
            (*t)->line = line;
            (*t)->column = column;
            (*t)->origin_state = current_state;
            (*t)->size = strlen(buff_token);
            find_next_state_from_char(next_c, &next_state, &current_state);
            column = ccolumn;
            line = cline;
            // memorize next_c 
            buff_token[0] = next_c;
            buff_token[1] = '\0';
            buff_token_end = 1;
            // no current_state but no end of file either, this seams to be a
            // problem.
            if (current_state == NULL && tmpend != EOF) {    
                fprintf(
                    stderr,
                    "buff_token (1): <%s>, error at line %ld column %ld\n", 
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
        // no next state, raise error. 
        if (next_state == NULL) {
           fprintf(
               stderr,
                "buff_token (2): <%s>, error at line %ld column %ld\n", 
                buff_token, 
                cline, 
                ccolumn
            );
            return 0; 
        }
        current_state = next_state;
    } while (tmpend != EOF);
    freeToken(*t);
    (*t) = NULL;
    return 1;
}

void initialize_lex() {
    FILE *lex_file, *keywords_file, *primitive_types_file;
    vkeywords_size = 0;
    escopo_atual = 0;
    _number_of_states = 0;
    vescopos_size = 1;
    vescopos[0] = malloc(sizeof(Escopo));
    vescopos[0]->level = 0;
    vescopos[0]->videntifiers_size = 0;

    primitive_types_file = fopen("./languagefiles/primitive_types.txt", "r");
    while (fscanf(primitive_types_file, " %s", buff_token) != EOF) {
        add_identifier_to_list(buff_token);
    }

    lex_file = fopen("./languagefiles/lang.lex", "r");
    keywords_file = fopen("./languagefiles/keywords.txt", "r");

    // parse the configuration file 
    while (lex_parser_read_char(lex_file)) {
    }
    // read keywords file 
    while (fscanf(keywords_file, " %s", buff_token) != EOF) {
        vkeywords[vkeywords_size] = malloc(sizeof(char) * (strlen(buff_token) + 1L));
        strcpy(vkeywords[vkeywords_size++], buff_token);
    }
    fclose(lex_file);
    fclose(keywords_file);
    fclose(primitive_types_file);
}

void print_identifiers() {
    int i, j, k;

    printf("\nLista de identificadores:\n\n");
    for (i = 0; i < vescopos_size; i++) {
        if (vescopos[i]->videntifiers_size > 0) {
            printf("Escopo nivel %ld:\n", vescopos[i]->level);
            for (j = 0; j < vescopos[i]->videntifiers_size; j++) {
                for (k = 0; k < vescopos[i]->level+1; k++)
                    printf(">>");
                printf(" %s\n", vescopos[i]->videntifiers[j]);
            }
        }
    }
}


void freeLex() {
    int i, j;
    for (i = 0; i < _number_of_states; i++) {
        if (state_table[i] == NULL)
            continue;
        FREEANDNULL(state_table[i]->name);
        FREEANDNULL(state_table[i]->class_name);
        for (j = 0; j < state_table[i]->number_of_transitions; j++) {
            FREEANDNULL(state_table[i]->masks[j]);

        }
    }

    for (i = 0; i < _number_of_states; i++) {
        FREEANDNULL(state_table[i]);
    }

    for (i = 0; i < vescopos_size; i++) {
        for (j = 0; j < vescopos[i]->videntifiers_size; j++) {
            FREEANDNULL(vescopos[i]->videntifiers[j]);
        }
        FREEANDNULL(vescopos[i]);
    }
    for (i = 0; i < vkeywords_size; i++) {
        FREEANDNULL(vkeywords[i]);
    }
}


//typedef struct State {
//    char* name;
//    char* class_name;
//    int number_of_transitions;
//    long* masks[MAX_NUM_TRANSITIONS];
//    struct State* transitions[MAX_NUM_TRANSITIONS];
//} State;
//
//typedef struct Token {
//    long line;
//    long column;
//    long size;
//    char* class_name;
//    State* origin_state;
//    char* str;
//} Token;
//
//typedef struct Escopo {
//    long level;
//    char* videntifiers[MAX_NUMBER_OF_IDENTIFIERS];
//    long videntifiers_size;
//} Escopo;
//
//int _number_of_states;
//
//State* state_table[MAX_NUM_STATES];
//char buff_token[MAX_SIZE_OF_A_TOKEN];
//long buff_token_end;
//
//
//char* vkeywords[MAX_NUMBER_OF_KEYWORDS];
//long vkeywords_size;
//long escopo_atual;
//Escopo* vescopos[MAX_NUMBER_OF_ESCOPOS];
//long vescopos_size;
//
//void initialize_lex();
//int next_useful_token(FILE* f, Token** t);
//void print_token(Token* t);
