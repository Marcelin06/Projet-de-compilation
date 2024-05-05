/* file parseur.y
* compilation: bison -d parseur.y
* result: parseur.tab.c = C code for syntaxic analyser
* result: parseur.tab.h = def. of lexical units aka lexems
*/

%{ // the code between %{ and %} is copied at the start of the generated .c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
int yylex(void); // -Wall : avoid implicit call
int yyerror(const char*); // on fonctions defined by the generator
%}

%token NUMBER // kinds of non-trivial tokens expected from the lexer
%token BOOLEAN

%token EQUALS //token for the multisymbol '=='
%token MORE_THAN_OR_EQUALS //token for the multisymbol '>='
%token LESS_THAN_OR_EQUALS //token for the multisymbol '<='
%token DIFFERENT_FROM //token for the multisymbol '!='
%token NOT //token for !


%start program// main non-terminal

%left EQUALS DIFFERENT_FROM
%left MORE_THAN_OR_EQUALS LESS_THAN_OR_EQUALS '<' '>'
%left '+' '-'
%left '%'
%left '*' '/'
%nonassoc UMOINS
%nonassoc NOT
%%

program : /* empty */ | program command
command : expression ';' 
expression:
NUMBER|BOOLEAN
| expression '+' expression 
| expression '-' expression 
| expression '*' expression 
| expression '/' expression
| expression '%' expression 
| '(' expression ')' 
| '-' expression %prec UMOINS
| expression EQUALS expression
| expression LESS_THAN_OR_EQUALS expression
| expression MORE_THAN_OR_EQUALS expression
| expression DIFFERENT_FROM expression
| expression '>' expression
| expression '<' expression
| NOT expression %prec NOT
;
   // everything after %% is copied at the end of the generated .c
%%
int yyerror(const char *msg){ // called by the parser if the parsing fails
    printf("Parsing:: syntax error %s\n", msg);
    return 1; // to distinguish with the 0 retured by the success
}