CC=gcc
LEXFOLDER=./lex
SYNFOLDER=./syn
SEMFOLDER=./semantico
STACKFOLDER=./stack
RNAFOLDER=./rna
CFLAGS=-g
VALFLAGS= --leak-check=full --track-origins=yes --show-reachable=yes
EX=./examples/simprolog.pro

all: lex.o syn.o sem.o stack.o lextest compilertest runrna

lex.o:
	${CC} -c ${LEXFOLDER}/lex.c ${CFLAGS} -o $@

syn.o:  
	${CC} -c ${SYNFOLDER}/parse_mdfa.c  ${CFLAGS} -o $@
	
sem.o:  
	${CC} -c ${SEMFOLDER}/generate_rna.c  ${CFLAGS} -o $@
	
stack.o:  
	${CC} -c ${STACKFOLDER}/generic-stack.c  ${CFLAGS} -o $@

lextest: lex.o 
	${CC} ${LEXFOLDER}/test.c lex.o ${CFLAGS} -o $@
	$(info ===== TESTING LEX =====)
	./lextest ${EX}

compilertest: syn.o lex.o sem.o stack.o
	${CC} lex.o syn.o sem.o stack.o ${SYNFOLDER}/parse.c ${CFLAGS} -o $@
	$(info ===== TESTING COMPILER =====)
	./compilertest ${EX}

runrna: compilertest
	$(info ===== TESTING RNA PROGRAM =====)
	cd ${RNAFOLDER} && ./rna.out program.rna && cd ..

clean:
	rm -f lextest lex.o syn.o sem.o stack.o syntest compilertest  

