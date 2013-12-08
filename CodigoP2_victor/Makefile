CC=gcc
LEXFOLDER=./lex
SYNFOLDER=./syn
SEMFOLDER=./semantico
CFLAGS=-g
VALFLAGS= --leak-check=full --track-origins=yes --show-reachable=yes
EX1=./examples/simprolog.pro
EX2=./examples/simprolog2.pro


all: lex.o syn.o testall

lex.o:
	${CC} -c ${LEXFOLDER}/lex.c ${CFLAGS} -o $@

syn.o:  
	${CC} -c ${SYNFOLDER}/parse_mdfa.c  ${CFLAGS} -o $@
	
sem.o:  
	${CC} -c ${SEMFOLDER}/generate_rna.c  ${CFLAGS} -o $@


testall: lextest syntest compilertest 


lextest: lex.o 
	${CC} ${LEXFOLDER}/test.c lex.o ${CFLAGS} -o $@
	$(info ===== TESTING LEX =====)
	./lextest ./ENTRADA.txt 

syntest: syn.o lex.o sem.o
	${CC} ${SYNFOLDER}/test.c syn.o lex.o sem.o ${CFLAGS} -o $@
	$(info ===== TESTING SYN =====)
	./syntest 

compilertest: syn.o lex.o sem.o
	${CC} lex.o syn.o sem.o ${SYNFOLDER}/parse.c ${CFLAGS} -o $@
	$(info ===== INT LEX/SYN =====)
	$(info ======= ${EX1} ========)
	./compilertest ${EX1}
	$(info ======= ${EX2} ========)
	./compilertest ${EX2}

valgrind: valsynlex valsyn vallex 


valsynlex: compilertest
	valgrind ${VALFLAGS} ./compilertest ${EX1}
	valgrind ${VALFLAGS} ./compilertest ${EX2}

valsyn: syntest 
	valgrind ${VALFLAGS} ./syntest 

vallex: lextest 
	valgrind ${VALFLAGS} ./lextest ${EX1}
	valgrind ${VALFLAGS} ./lextest ${EX1}




clean:
	rm -f lextest lex.o syn.o sem.o syntest compilertest  

