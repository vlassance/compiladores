CC=gcc
LEXFOLDER=./lex
CFLAGS=-g

all: lex.o test


lex.o: 
	${CC} -c ${LEXFOLDER}/lex.c ${CFLAGS} -o $@

test: 
	${CC} ${LEXFOLDER}/test.c lex.o ${CFLAGS} -o $@

clean:
	rm -f test lex.o 

