//
//  main.c
//  RNA
//
//  Created by Cyrus Hackford on 2011.
//  Copyright 2011 SI devIk. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RNA_INTERP_VERSION "1.0"
#define RNA_STR_INIT_SIZE 32

#ifndef OBJC_BOOL_DEFINED
#define OBJC_BOOL_DEFINED
typedef signed char BOOL;
#define NO ((BOOL)0)
#define YES ((BOOL)1)
#endif


typedef struct _Memory {
	unsigned char *mem;
	unsigned long alloc;
	unsigned long used;
} Memory;

static BOOL appendMem(Memory *memory, const char *appendable) {
	size_t leng=strlen(appendable);
	
	if((memory->alloc-memory->used)<leng) {
		unsigned char *test=(unsigned char *)realloc(memory->mem, (size_t)(memory->used+leng+1));
		if(test==NULL)
			return NO;
		memory->mem=test;
		memory->alloc+=leng;
	}
	
	{
		unsigned char *temp=memory->mem;
		size_t length=leng;
		
		while(*temp!='\0')
			temp++;
		
		while(length!=0 && (*temp=*appendable++)!='\0') {
			length--;
			temp++;
		}
		
		if(*temp!='\0')
			*temp='\0';
	}
	
	memory->used+=leng;
	
	return YES;
}

static BOOL createMem(Memory **memory) {
	*memory=(Memory *)malloc(sizeof(Memory));
	if(*memory==NULL)
		return NO;
	
	(*memory)->mem=(unsigned char *)malloc(RNA_STR_INIT_SIZE+1);
	if((*memory)->mem==NULL) {
		free(*memory);
		
		return NO;
	}
	
	memset((*memory)->mem, 0, RNA_STR_INIT_SIZE+1);
	
	(*memory)->alloc=RNA_STR_INIT_SIZE;
	(*memory)->used=0;
	
	return YES;
}

static void destroyMem(Memory **memory, BOOL completely) {
	if(completely==YES)
		free((*memory)->mem);
	free(*memory);
}

static void dumpMem(Memory *memory, unsigned long limit) { // debugging purpose
	unsigned long loop=0, loopy;
	
	if(limit==0)
		loop=memory->alloc;
	else
		loop=limit<memory->alloc?limit:memory->alloc;
	
	printf("\nMemory#%lu: [", (unsigned long)memory);
	for(loopy=0; loopy<loop; ++loopy) {
		printf("%u", memory->mem[loopy]);
		
		if(loopy!=(loop-1))
			printf(", ");
	}
	printf("]\n");
}

static BOOL expandMem(Memory *memory, size_t size) {
	unsigned char *test;
	
	if(memory->alloc>=size)
		return YES;
	
	test=(unsigned char *)realloc(memory->mem, size+1);
	if(test==NULL)
		return NO;
	memory->mem=test;
	
	return YES;
}


static size_t ustrlen(const unsigned char *s) {
	const unsigned char *p=s;
	
	while(*p!='\0')
		p++;
	
	return (size_t)(p-s);
}

int main(int argc, const char **argv) {
	unsigned char *instructions;
	
	if(argc==1) {
		printf("The RNA Programming Language -- Interpreter v%s\nUsage: %s filename\n", RNA_INTERP_VERSION, argv[0]);
		
		return EXIT_FAILURE;
	}
	
	{
		BOOL failed;
		char *tempStrg;
		FILE *stream;
		Memory *instructionHolder;
		
		if(createMem(&instructionHolder)==NO) {
			printf("Interpreter error: Cannot allocate memory to hold RNA instructions.\nTerminated.\n");
			
			return EXIT_FAILURE;
		}
		
		if((stream=fopen(argv[1], "r"))==NULL) {
			printf("Interpreter error: Cannot open RNA instruction file: %s.\nTerminated.\n", argv[1]);
			destroyMem(&instructionHolder, YES);
			
			return EXIT_FAILURE;
		}
		
		failed=NO;
		if((tempStrg=(char *)malloc(sizeof(char)*512))==NULL) {
			printf("Interpreter error: Cannot allocate memory to hold RNA instructions.\nTerminated.\n");
			fclose(stream);
			destroyMem(&instructionHolder, YES);
			
			return EXIT_FAILURE;
		}
		
		while(feof(stream)==0) {
			fread(tempStrg, sizeof(char), 512, stream);
			if(appendMem(instructionHolder, tempStrg)==NO) {
				failed=YES;
				
				break;
			}
		}
		free(tempStrg);
		fclose(stream);
		
		if(failed==YES) {
			printf("Interpreter error: Cannot resize memory space used to hold RNA instructions.\nTerminated.\n");
			destroyMem(&instructionHolder, YES);
			
			return EXIT_FAILURE;
		}
		
		instructions=instructionHolder->mem;
		destroyMem(&instructionHolder, NO);
	}
	
	{
		// A=1 U=2 G=3 C=4
		unsigned char *ptr;
		Memory *memory;
		if(createMem(&memory)==NO) {
			printf("Interpreter error: Cannot allocate memory to hold on-execution data.\nTerminated.\n");
			free(instructions);
			
			return EXIT_FAILURE;
		}
		
		unsigned int codon=0, codonSize=100, loopActivatorIndex=0;
		unsigned long loop, loopActivators[2][64], loopy, strg;
		BOOL loopActivated=NO, skipUntilLoopTerminator=NO, started=NO;
		for(loop=ustrlen(instructions), loopy=0; loopy<loop; ++loopy) {
			{
				unsigned int inst=0;
				switch(instructions[loopy]) {
					case 'A':
					case 'a':
						inst=1;
						break;
					case 'U':
					case 'u':
						inst=2;
						break;
					case 'G':
					case 'g':
						inst=3;
						break;
					case 'C':
					case 'c':
						inst=4;
						break;
				}
				
				if(inst==0)
					continue;
				codon+=inst*codonSize;
			}
			
			if(codonSize==1) {
				if(started==NO) {
					if(codon==123) { // Metionine
						started=YES;
						strg=0;
						ptr=memory->mem;
					}
				} else {
					if(skipUntilLoopTerminator==YES && (codon==212 || codon==214))
						skipUntilLoopTerminator=NO;
					else {
						switch(codon) {
							case 211: // Termination
							case 213:
							case 231:
								started=NO;
								break;
							case 233: // Tryptophan
								strg=0;
								break;
							case 111: // Lysine
							case 113:
								++strg;
								break;
							case 112: // Asparagin
							case 114:
								--strg;
								break;
							case 341: // Alanine
							case 342:
							case 343:
							case 344:
								strg=(unsigned long)*ptr;
								break;
							case 141: // Threonine
							case 142:
							case 143:
							case 144:
								expandMem(memory, strg+1);
								ptr=&(memory->mem[strg]);
								break;
							case 441: // Proline
							case 442:
							case 443:
							case 444:
								{
									int temp;
									scanf("%d", &temp);
									*ptr=(unsigned char)(temp&0xF);
								}
								break;
							case 421: // Leucine
							case 422:
							case 423:
							case 424:
								printf("%c", (char)(*ptr&0x7F));
								break;
							case 131: // Arginine
							case 133:
							case 431:
							case 432:
							case 433:
							case 434:
								expandMem(memory, strg+1);
								*ptr+=memory->mem[strg];
								break;
							case 132: // Serine
							case 134:
							case 241:
							case 242:
							case 243:
							case 244:
								expandMem(memory, strg+1);
								*ptr*=memory->mem[strg];
								break;
							case 411: // Glutamine
							case 413:
								expandMem(memory, strg+1);
								*ptr-=memory->mem[strg];
								break;
							case 412: // Histidine
							case 414:
								expandMem(memory, strg+1);
								*ptr/=memory->mem[strg];
								break;
							case 311: // Glutamic acid
							case 313:
								expandMem(memory, strg+1);
								*ptr=(*ptr==memory->mem[strg]?1:0);
								break;
							case 312: // Aspartic acid
							case 314:
								if(*ptr==0) {
									if(loopActivated==NO)
										skipUntilLoopTerminator=YES;
									else {
										loopy=loopActivators[1][loopActivatorIndex++]-1;
										loopActivated=NO;
									}
								} else {
									loopActivated=YES;
									loopActivators[0][loopActivatorIndex++]=loopy-2;
								}
								break;
							case 212: // Tyrosine
							case 214:
								if(loopActivated==YES) {
									loopActivators[1][--loopActivatorIndex]=loopy+1;
									loopy=loopActivators[0][loopActivatorIndex]-1;
								}
								break;
							// Other codons are no-op yet. :)
						}
					}
				}
				
				codon=0;
				codonSize=100;
			} else
				codonSize/=10;
		}
	}
	
	free(instructions);
	
	return EXIT_SUCCESS;
}
