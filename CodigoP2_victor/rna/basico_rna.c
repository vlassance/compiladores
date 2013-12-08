int main() {
	// pre-ambulo
	strg=0;
	ptr=&memory[strg];
	*ptr=*ptr==memory[strg]?1:0; // posição 0 vale 1 e sempre valerá
	
	// para setar um valor em uma posiçãp
	strg=0;
	++strg; // n vezes pra chegar na posição desejada
	ptr=&memory[strg];
	*ptr=*ptr==memory[strg]?1:0;
	*ptr-=memory[strg]; // a posição desejada tem o valor 0
	strg=0;
	*ptr+=memory[strg]; // n vezes para chegar ao valor desejado
	
	// para ler o valor de uma posição (a partir de 48)
	strg=0;
	++strg; // n vezes pra chegar na posição desejada
	ptr=&memory[strg];
	strg=0;
	*ptr+=memory[strg]; // 48 vezes para chegar ao valor que possa ser impresso (só funciona de 0 a 9 a impressão)
	printf(“%c”, *ptr);
	*ptr-=memory[strg]; // 48 vezes para voltar ao valor original
} end_main
