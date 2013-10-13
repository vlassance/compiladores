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
    long cod;
    long maskterm, maskdepl;
    int i;
    State *from;
    State *to;

    long masktermsize = sizeof(long) * 8; // number of byts on a long

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
        cod = (long) c;
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
	int i;
	for (i = 0; i < videntifiers_size; i++) {
        if (strcmp(name_ident, videntifiers[i]) == 0) {       
            break;
        }
	}
	if (i == videntifiers_size) {
		videntifiers[videntifiers_size] = malloc(sizeof(char) * (strlen(name_ident) + 1L));
		strcpy(videntifiers[videntifiers_size++], name_ident);
	}
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
            (*t)->class_name = malloc(6 * sizeof(char));
            strcpy((*t)->class_name, "IDENT");
			add_identifier_to_list((*t)->str);
        } else {
            (*t)->class_name = malloc(9 * sizeof(char));
            // name it RESERVED in case it is
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
    // tmpend is static, if i read something that was 
    // EOF in the last step, this is the end and I should set t to null 
    if (tmpend == EOF) {
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
    (*t) = NULL;
    return 1;
}

void initialize_lex() {
    FILE *lex_file, *keywords_file;
    vkeywords_size = 0;
	videntifiers_size = 0;
    _number_of_states = 0;

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
}

void print_identifiers() {
	int i;
	
	printf("\nLista de identificadores:\n\n");
	for (i = 0; i < videntifiers_size; i++) {
        printf(">> %s\n", videntifiers[i]);
	}
}
