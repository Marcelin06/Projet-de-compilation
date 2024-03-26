/* file main.c
* compilation: gcc -o compiler main.c parser.tab.c lexer.tab.c
* result: executable
*/

#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"

int main(void) {
    AST_comm rez;
    if (!yyparse(&rez)) { // call to the parsing (and lexing) function
        printf("\nParsing:: c'est bien une expression arithmétique\n"); // reached if parsing follows
    }
    print_comm(rez);
    exit(EXIT_SUCCESS);
}