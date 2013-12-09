#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>
#include "automata.h"
#include "../lex/lex.h"
#include "../semantico/generate_rna.h"

int prioridade_operadores[256] = {['~'] = 3, ['&'] = 2, ['|'] = 1};
int num_operandos[256] = {['~'] = 1, ['&'] = 2, ['|'] = 2};
Stack stack_operando, stack_operador;

void strfreefn(ElementAddr elemaddr) {
	free(*(char **)elemaddr);
}

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
	
	size_lista_constantes = 0;
	size_lista_variaveis = 0;
	size_lista_sentencas = 0;
	size_lista_clausulas = 0;
	size_lista_fatos = 0;
	sentenca[0] = '\0';
	clausula[0] = '\0';

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
            printf("Desempilhou PROGRAM em %d com stack: %d\n", 
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

void get_variaveis_e_inicia_constantes(char* clausula_atual, int* size_variaveis, int variaveis[MAX_SIZE_LIST], int constantes[MAX_SIZE_LIST]) {
	printf("Selecionando todas as variáveis da clausula %s...\n", clausula_atual);
	*size_variaveis = 0;
	
	char *p = clausula_atual;
	while (*p) {
	    if (isdigit(*p)) {
	        int ind = (int)strtol(p, &p, 10)-1;
			char *p2 = lista_sentencas[ind];
			while (*p2) {
			    if (isdigit(*p2)) {
			        int val = (int)strtol(p2, &p2, 10);
					if (val > MAX_SIZE_LIST) {
						insere_lista_int(variaveis, size_variaveis, val);
						constantes[val-MAX_SIZE_LIST-1] = 1;
					}
			    } else {
			        p2++;
			    }
			}
	    } else {
	        p++;
	    }
	}
}

bool has_new_combination(int size_variaveis, int variaveis[MAX_SIZE_LIST], int constantes[MAX_SIZE_LIST]) {
	int i;
	
	constantes[variaveis[size_variaveis-1]-MAX_SIZE_LIST-1]++;
	for (i = size_variaveis-1; i >= 0; i--) {
		if (constantes[variaveis[i]-MAX_SIZE_LIST-1] <= size_lista_constantes) {
			return true;
		} else if (i > 0) {
			constantes[variaveis[i]-MAX_SIZE_LIST-1] = 1;
			constantes[variaveis[i-1]-MAX_SIZE_LIST-1]++;
		}
	}
	return false;
}

void insere_fato(char fato_gerado[MAX_SIZE_SENTENCA]) {
	int index;
	
	index = insere_lista_str(lista_sentencas, &size_lista_sentencas, fato_gerado);
	insere_lista_int(lista_fatos, &size_lista_fatos, index);
}

void unifica(char sentenca_gerada[MAX_SIZE_SENTENCA], char sentenca[MAX_SIZE_SENTENCA], int size_variaveis, int variaveis[MAX_SIZE_LIST], int constantes[MAX_SIZE_LIST]) {
	char pstr[MAX_SIZE_SENTENCA];
	char* pch;
	char* end_token;
	strcpy(pstr, sentenca);
	sentenca_gerada[0] = '\0';
	
	pch = strtok_r (pstr, " ", &end_token);
	while (pch != NULL) {
		if (atoi(pch) > MAX_SIZE_LIST) {
			sprintf(sentenca_gerada, "%s%d ", sentenca_gerada, constantes[atoi(pch)-MAX_SIZE_LIST-1]);
		} else {
			sprintf(sentenca_gerada, "%s%s ", sentenca_gerada, pch);
		}
		pch = strtok_r (NULL, " ", &end_token);
	}
}

void empilha_operando(char fato[MAX_SIZE_SENTENCA]) {
	// Verifica se fato pertence a lista_sentencas e indice a lista_fatos
	int i, index = -1, res = 0;
	
	for (i = 0; i < size_lista_sentencas; i++) {
		if (strcmp(lista_sentencas[i], fato) == 0) {
			index = i;
		}
	}
		
	if (index >= 0) {
		for (i = 0; i < size_lista_fatos; i++) {
			if (lista_fatos[i]-1 == index) {
				res = 1;
			}
		}
	}
	
	// Adiciona o resultado a pilha de operandos
	stack_push(stack_operando, &res);
}

int calculate_expression(int op, int* elems) {
	if (op == '~')
		return !elems[0];
	if (op == '&')
		return elems[0] && elems[1];
	if (op == '|')
		return elems[0] || elems[1];
	
	printf("Operador não reconhecido!\n");
	return 0;
}

void empilha_operador(char* pch) {
	int op = pch[0], op_pilha, elem, i;
	int elems[5];
	
	// Verifica se não tem operadores com maior prioridade para calculá-los primeiro
	if (!stack_is_empty(stack_operador)) {
		stack_top(stack_operador, &op_pilha);
		while (prioridade_operadores[op_pilha] > prioridade_operadores[op]) {
			stack_pop(stack_operador);
			for (i = 0; i < num_operandos[op_pilha]; i++) {
				stack_top_and_pop(stack_operando, &elem);
				elems[i] = elem;
			}
			elem = calculate_expression(op_pilha, elems);
			stack_push(stack_operando, &elem);
			
			if (stack_is_empty(stack_operador))
				break;
			stack_top(stack_operador, &op_pilha);
		}
	}
	
	// Adiciona o resultado a pilha de operadores
	stack_push(stack_operador, &op);
}

bool resultado_expressao() {
	int op, elem, i;
	int elems[5];
	
	// Desempilha o que for necessário e devolver o resultado da expressão
	while (!stack_is_empty(stack_operador)) {
		stack_top_and_pop(stack_operador, &op);
		for (i = 0; i < num_operandos[op]; i++) {
			stack_top_and_pop(stack_operando, &elem);
			elems[i] = elem;
		}
		elem = calculate_expression(op, elems);
		stack_push(stack_operando, &elem);
	}
	
	stack_top_and_pop(stack_operando, &elem);
	if (!stack_is_empty(stack_operando))
		printf("Problema na análise da expressão.\n");
	return elem;
}

void generate_facts_from_rule(char* clausula_atual, int size_variaveis, int variaveis[MAX_SIZE_LIST], int constantes[MAX_SIZE_LIST]) {
	
	char fato_gerado[MAX_SIZE_SENTENCA], pstr[MAX_SIZE_SENTENCA], sentenca[MAX_SIZE_SENTENCA];
	char* pch;
	char* end_str;
	strcpy(pstr, clausula_atual);
	
	stack_make_empty(stack_operando);
	stack_make_empty(stack_operador);
	
	pch = strtok_r (pstr, " ", &end_str);
	unifica(sentenca, lista_sentencas[atoi(pch) - 1], size_variaveis, variaveis, constantes);
	strcpy(fato_gerado, sentenca);
	
	pch = strtok_r (NULL, " ", &end_str);
	if (strcmp(pch, "=") != 0) {
		printf("Erro ao ler cláusula: %s!\n", clausula_atual);
		return;
	}
	
	pch = strtok_r (NULL, " ", &end_str);
	while (pch != NULL) {
		if (atoi(pch) > 0) {
			unifica(sentenca, lista_sentencas[atoi(pch) - 1], size_variaveis, variaveis, constantes);
			empilha_operando(sentenca);
		} else {
			empilha_operador(pch);
		}
		pch = strtok_r (NULL, " ", &end_str);
	}
	
	if (resultado_expressao())
		insere_fato(fato_gerado);
	
	/*int pr;
	printf("Generating new facts from rule %s\n", clausula_atual);
	for (pr = 0; pr < size_variaveis; pr++) {
		printf(">> Using %s (ID %d) = ", lista_variaveis[variaveis[pr]-MAX_SIZE_LIST-1], variaveis[pr]);
		printf("%s (ID %d)\n", lista_constantes[constantes[variaveis[pr]-MAX_SIZE_LIST-1]-1], constantes[variaveis[pr]-MAX_SIZE_LIST-1]);
	}*/
}

void generate_facts_from_rules() {
	int n_fatos, c, v, size_variaveis;
	int variaveis[MAX_SIZE_LIST], constantes_escolhidas[MAX_SIZE_LIST];
	
	stack_operando = stack_create(sizeof(int), NULL);
	stack_operador = stack_create(sizeof(int), NULL);
	
	do {
		n_fatos = size_lista_fatos;
		printf("New cycle - generating new facts from rules.\n");
		
		for(c = 0; c < size_lista_clausulas; c++) {
			get_variaveis_e_inicia_constantes(lista_clausulas[c], &size_variaveis, variaveis, constantes_escolhidas);
			do {
				generate_facts_from_rule(lista_clausulas[c], size_variaveis, variaveis, constantes_escolhidas);
			} while (has_new_combination(size_variaveis, variaveis, constantes_escolhidas));
		}
		
	} while(n_fatos < size_lista_fatos);
	
	stack_dispose(stack_operando);
	stack_dispose(stack_operador);
}

void semantico_desempilha() {
    // AS 11
	generate_rna_file(meta, size_lista_fatos, lista_fatos);
	
	int i;
	printf("\n>> Lista de constantes:\n");
	for (i = 0; i < size_lista_constantes; i++)
		printf("%s\n", lista_constantes[i]);
	printf("\n>> Lista de variáveis:\n");
	for (i = 0; i < size_lista_variaveis; i++)
		printf("%s\n", lista_variaveis[i]);
	printf("\n>> Lista de sentenças:\n");
	for (i = 0; i < size_lista_sentencas; i++)
		printf("%s\n", lista_sentencas[i]);
	printf("\n>> Lista de fatos:\n");
	for (i = 0; i < size_lista_fatos; i++)
		printf("%d\n", lista_fatos[i]);
	printf("\n>> Lista de cláusulas:\n");
	for (i = 0; i < size_lista_clausulas; i++)
		printf("%s\n", lista_clausulas[i]);
	printf("\n>> Meta:\n");
	printf("%d\n", meta);
}

int insere_lista_str(char lista[MAX_SIZE_LIST][MAX_SIZE_SENTENCA], int* size_lista, char* text) {
	int i;
	
	for (i = 0; i < *size_lista; i++)
		if (strcmp(lista[i], text) == 0)
			return i+1;
	i = *size_lista;
	strcpy(lista[i], text);
	(*size_lista)++;
	return i+1;
}

int insere_lista_int(int lista[MAX_SIZE_LIST], int* size_lista, int value) {
	int i;
	
	for (i = 0; i < *size_lista; i++)
		if (lista[i] == value)
			return i;
	i = *size_lista;
	lista[i] = value;
	(*size_lista)++;
	return i;
}

void semantico_token(Token* tk, char* nome_trans, int id_state_from, int id_state_to) {
	int index;
	
	// AS 1
	if (strcmp(nome_trans, " PRED") == 0 || strcmp(nome_trans, " NUM") == 0) {
		index = insere_lista_str(lista_constantes, &size_lista_constantes, tk->str);
		sprintf(sentenca, "%s%d ", sentenca, index);
	}
	// AS 2
	if (strcmp(nome_trans, " INF") == 0) {
		index = insere_lista_str(lista_variaveis, &size_lista_variaveis, tk->str);
		sprintf(sentenca, "%s%d ", sentenca, index+MAX_SIZE_LIST);
	}
	// AS 3
	if (id_state_from == 4 && id_state_to == 5) {
		index = insere_lista_str(lista_sentencas, &size_lista_sentencas, sentenca);
		insere_lista_int(lista_fatos, &size_lista_fatos, index);
		sentenca[0] = '\0';
		
	}
	// AS 4
	if (id_state_from == 8 && id_state_to == 9) {
		index = insere_lista_str(lista_sentencas, &size_lista_sentencas, sentenca);
		sprintf(clausula, "%s%d ", clausula, index);
		sprintf(clausula, "%s%s ", clausula, "=");
		sentenca[0] = '\0';
	}
	// AS 5
	if (id_state_from == 24 && id_state_to == 11) {
		if (strcmp(tk->str, "&") == 0)
			sprintf(clausula, "%s%s ", clausula, "&");
		else if (strcmp(tk->str, "or") == 0)
			sprintf(clausula, "%s%s ", clausula, "|");
	}
	// AS 6
	if (id_state_from == 11 && id_state_to == 19) {
		sprintf(clausula, "%s%s ", clausula, "~");
	}
	// AS 7
	if (id_state_from == 12 && id_state_to == 14) {
		// Apesar da gramática permitir, não há sentido em manter sentenças do tipo (pai X, Y :- 1, 2, 3)
		// pois não há como transformá-las em true, false
		sentenca[0] = '\0';
		clausula[0] = '\0';
		
	}
	// AS 8
	if (id_state_from == 25 && id_state_to == 14) {
		index = insere_lista_str(lista_clausulas, &size_lista_clausulas, clausula);
		clausula[0] = '\0';
		
	}
	// AS 9
	if (id_state_from == 20 && id_state_to == 21) {
		meta = insere_lista_str(lista_sentencas, &size_lista_sentencas, sentenca);
		sentenca[0] = '\0';
		generate_facts_from_rules();
	}
	// AS 10
	if (id_state_from == 23 && id_state_to == 24) {
		index = insere_lista_str(lista_sentencas, &size_lista_sentencas, sentenca);
		sprintf(clausula, "%s%d ", clausula, index);
		sentenca[0] = '\0';
	}
		
}

uint32_t syn(Token* tk, Automaton** a, uint32_t* state) {
    if (tk == NULL) {
		uint32_t res;
		res = poponly(a, state);
        semantico_desempilha();
        return res;
    }
    uint32_t i;
    char* strtrans;
    for (i = 0U; i < (*a)->size_transitions[*state]; i++) {
        strtrans = (*a)->transitions_str[*state][i];
        if (strcmp(strtrans, tk->str) == 0) {
			semantico_token(tk, strtrans, *state, (*a)->transitions[*state][i]);
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
				semantico_token(tk, strtrans, *state, (*a)->transitions[*state][i]);
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
		semantico_desempilha();
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
