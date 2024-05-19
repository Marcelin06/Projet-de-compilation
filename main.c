/* file main.c
* compilation: gcc -o compiler main.c parser.tab.c lexer.tab.c
* result: executable 
*/

#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"
#include <string.h>

int main(int argc, char *args[]) {
    extern FILE* yyin;
    yyin = fopen(args[1], "r");
    if (!yyparse()) { // call to the parsing (and lexing) function
        printf("\nParsing:: c'est bien une expression arithmétique\n"); // reached if parsing follows
    }
    exit(EXIT_SUCCESS);
}