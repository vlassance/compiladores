CC=gcc
LEXFOLDER=./lex
SYNFOLDER=./syn
CFLAGS=-g

all: lex.o syn.o testall

lex.o:
	${CC} -c ${LEXFOLDER}/lex.c ${CFLAGS} -o $@

syn.o: lex.o 
	${CC} -c lex.o ${SYNFOLDER}/parse_mdfa.c  ${CFLAGS} -o $@



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
	./synlextest ./examples/exemplo-programa-fatorial.czar 
	./synlextest ./examples/exemplo-programa-geral.czar

clean:
	rm -f lextest lex.o syn.o syntest synlextest  

