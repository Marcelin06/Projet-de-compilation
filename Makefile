all: main parser.tab.c lex.yy.c

main: main.c parser.tab.c lex.yy.c AST.c
	gcc -o main AST.c main.c parser.tab.c lex.yy.c

parser.tab.c: parser.y 
	bison -d parser.y 

lex.yy.c: lexeur.l
	flex lexeur.l



.PHONY: clean
clean:
	rm -f *.o main parser.tab.c lex.yy.c parser.tab.h