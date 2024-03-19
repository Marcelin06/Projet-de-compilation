/* file parseur.y
* compilation: bison -d parseur.y
* result: parseur.tab.c = C code for syntaxic analyser
* result: parseur.tab.h = def. of lexical units aka lexems
*/

%{ // the code between %{ and %} is copied at the start of the generated .c
#include <stdio.h>
#include "AST.h"
int yylex(void); // -Wall : avoid implicit call
int yyerror(AST_comm arg, const char*); // on fonctions defined by the generator
%}

%union { double number ; AST_expr expr ; AST_comm comm}
%token <number> NUMBER // kinds of non-trivial tokens expected from the lexer
%type <expr> expression
%type <comm> commande
%start commande // main non-terminal

%left '+' '-'
%left '*' '/'
%nonassoc UMOINS
%parse-param {AST_comm *rez}
%%
commande: 
    expression ';'
                {*rez = new_command($1);}
expression:
expression '+' expression
                { $$ = new_binary_expr('+',$1,$3); }
| expression '-' expression
                { $$ = new_binary_expr('-',$1,$3); }
| expression '*' expression
                { $$ = new_binary_expr('*',$1,$3); }
| '(' expression ')'
                { $$ = $2; }
| '-' expression %prec UMOINS
                { $$ = new_unary_expr('M',$2); }
| NUMBER
                { $$ = new_number_expr($1); }                
;

   // everything after %% is copied at the end of the generated .c
%%
int yyerror(AST_comm arg, const char *msg){ // called by the parser if the parsing fails
    printf("Parsing:: syntax error %s\n");
    return 1; // to distinguish with the 0 retured by the success
}