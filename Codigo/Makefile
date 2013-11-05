CC=gcc
LEXFOLDER=./lex
SYNFOLDER=./syn
CFLAGS=-g
VALFLAGS= --leak-check=full --track-origins=yes --show-reachable=yes
EX1=./examples/exemplo-programa-geral.czar
EX2=./examples/exemplo-programa-fatorial.czar


all: lex.o syn.o testall

lex.o:
	${CC} -c ${LEXFOLDER}/lex.c ${CFLAGS} -o $@

syn.o:  
	${CC} -c ${SYNFOLDER}/parse_mdfa.c  ${CFLAGS} -o $@


testall: lextest syntest synlextest 


lextest: lex.o 
	${CC} ${LEXFOLDER}/test.c lex.o ${CFLAGS} -o $@
	$(info ===== TESTING LEX =====)
	./lextest ./ENTRADA.txt 

syntest: syn.o lex.o
	${CC} ${SYNFOLDER}/test.c syn.o lex.o ${CFLAGS} -o $@
	$(info ===== TESTING SYN =====)
	./syntest 

synlextest: syn.o lex.o
	${CC} lex.o syn.o ${SYNFOLDER}/parse.c ${CFLAGS} -o $@
	$(info ===== INT LEX/SYN =====)
	./synlextest ${EX1}
	./synlextest ${EX2}

valgrind: valsynlex valsyn vallex 


valsynlex: synlextest
	valgrind ${VALFLAGS} ./synlextest ${EX1}
	valgrind ${VALFLAGS} ./synlextest ${EX2}

valsyn: syntest 
	valgrind ${VALFLAGS} ./syntest 

vallex: lextest 
	valgrind ${VALFLAGS} ./lextest ${EX1}
	valgrind ${VALFLAGS} ./lextest ${EX1}




clean:
	rm -f lextest lex.o syn.o syntest synlextest  

