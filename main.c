/* file main.c
* compilation: gcc -o compiler main.c parser.tab.c lexer.tab.c
* result: executable 
*/

#include <stdio.h>
#include <stdlib.h>
#include "AST.h"
#include "parser.tab.h"
#include <string.h>

int main(int argc, char *args[]) {
    extern FILE* yyin;
    yyin = fopen(args[1], "r");
    AST_prog rez;
    if (!yyparse(&rez)) { // call to the parsing (and lexing) function
        printf("\nParsing:: c'est bien un programme JavaScript valide\n"); // reached if parsing follows
        
    }
    
    printf("\naffichage du programme : \n");
    
    print_prog(rez);

    printf("\n\n\naffichage du code assembleur : \n");
    affichage_code_prog(rez);
    printf("\nHalt\n");
    

    
   
    //free_prog(rez);

    printf("\n");

    exit(EXIT_SUCCESS);
}