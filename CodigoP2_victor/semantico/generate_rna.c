// To test generate RNA:
// make clean && make compilertest
// ./rna/rna.out rna/program.rna && cd rna && ./conversor.out program.rna program.c rna2c && cd ..

#include "generate_rna.h"

FILE* rna_create_file() {
	FILE* f = fopen("rna/program.rna", "w");
	//Inicia o programa e seta a primeira posição com o valor 1 e a segunda com o valor 0 para serem usados em todos os lugares
	fprintf(f, "AUG UGG ACA GAA AAA ACA GAA CAA ");
	return f;
}

bool rna_write_var(FILE* f, int index, int value) {
	if (index < POS_INICIAL || index >= 256)
		return false;
	if (value < 0 || value >= 256)
		return false;
	//Seta a posição desejada (index) com o valor desejado (value)
	fprintf(f, "UGG ");
	while (index--)
		fprintf(f, "AAA ");
	fprintf(f, "ACA GAA CAA UGG ");
	while (value--)
		fprintf(f, "AGA ");
	return true;
}

bool rna_read_var_shifted(FILE* f, int index, int shift) {
	int shift_it;
	
	if (index < 1 || index >= 256)
		return false;
	if (shift < 0)
		return false;
	fprintf(f, "UGG ");
	while (index--)
		fprintf(f, "AAA ");
	fprintf(f, "ACA UGG ");
	shift_it = shift;
	while (shift_it--)
		fprintf(f, "AGA ");
	fprintf(f, "CUA ");
	shift_it = shift;
	while (shift_it--)
		fprintf(f, "CAA ");
	return true;
}

void rna_output_text(FILE* f, char* text) {
	int i = 0;
	
	// Usa a posição 1 que vale zero para shiftar para a letra desejada, imprimi-la e voltar ao valor original
	while (text[i] != '\0')
		rna_read_var_shifted(f, 1, text[i++]);
}

void rna_write_search_algorithm(FILE* f, int n_fatos) {
	fprintf(f, "UGG ACA GAC UGG AAA AAA AAA AAA AAA AAA ACA UGG AGA UGG AAA AAA AAA AAA ACA GAA CAA AAA AAA AGA GCA AGA UGG AAA AAA AAA AAA AAA AAA CAA UGG AAA AAA GAA UGG AAA GAA UGG AAA AAA AAA AAA AAA ACA GAA CAA AAA AGA GCA AGA UGG AAA AAA AAA AAA AAA AAA CAA UGG AAA GAA UGG AAA GAA UGG AAA AAA AAA ACA GAA CAA AAA AGA AAA AGC UAC UGG AAA AAA AAA AAA ACA UGG AAA GAA ");
}

void rna_read_all_vars(FILE* f, int n) {
	int i = 0;
	
	for (i = 1; i < n; i++) {
		rna_read_var_shifted(f, i, '0'); // Faz o shift do valor da variável para leitura
	}
}

bool rna_read_var(FILE* f, int index) {
	return rna_read_var_shifted(f, index, 0);
}

void rna_close_file(FILE* f) {
	fprintf(f, "UAA ");
	fclose(f);
}

void generate_rna_file(int meta, int n_fatos, int fatos[]) {
	// FITA:
	// value: 1 0 <meta> <continue> <found> <!is_over> <it>   <fatos>      0
	// index: 0 1   2         3         4         5      6    7..7+(f-1) 8+(f-1)
	
	FILE* f;
	int i, pos_inicial = POS_INICIAL;
	
    f = rna_create_file();
	
	rna_write_var(f, pos_inicial, meta);
	
	rna_write_var(f, pos_inicial+1, 0); // Continue loop bool
	
	rna_write_var(f, pos_inicial+2, 0); // Found bool
		
	rna_write_var(f, pos_inicial+3, 0); // Is_over bool
	
	rna_write_var(f, pos_inicial+4, pos_inicial+4); // Local onde será iterado a busca
	
	for (i = 0; i < n_fatos; i++)
		rna_write_var(f, i+pos_inicial+5, fatos[i]);
	
	rna_write_var(f, n_fatos+pos_inicial+5, 0); // Simboliza o fim dos fatos
	
	rna_output_text(f, "Memoria antes (a partir da posicao 1): ");
	rna_read_all_vars(f, n_fatos+pos_inicial+6);
	rna_output_text(f, "\n");
	
	rna_write_search_algorithm(f, n_fatos);
	
	rna_output_text(f, "Memoria depois (a partir da posicao 1): ");
	rna_read_all_vars(f, n_fatos+pos_inicial+6);
	rna_output_text(f, "\n");
	
	rna_output_text(f, "Resultado da query: ");
	rna_read_var_shifted(f, pos_inicial+2, '0'); // Faz o shift do valor da variável para leitura
	rna_output_text(f, "\n");
	
	rna_close_file(f);
}
