CC=gcc
LEXFOLDER=./lex
CFLAGS=-g

all: lex.o testall


lex.o:
	${CC} -c ${LEXFOLDER}/lex.c ${CFLAGS} -o $@

testall: lextest lex.o 
	echo "===== TESTING ====="
	./lextest ./ENTRADA.txt 

lextest:  
	${CC} ${LEXFOLDER}/test.c lex.o ${CFLAGS} -o $@

clean:
	rm -f lextest lex.o 

