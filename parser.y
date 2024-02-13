/* file parseur.y
* compilation: bison -d parseur.y
* result: parseur.tab.c = C code for syntaxic analyser
* result: parseur.tab.h = def. of lexical units aka lexems
*/

%{ // the code between %{ and %} is copied at the start of the generated .c
#include <stdio.h>
int yylex(void); // -Wall : avoid implicit call
int yyerror(const char*); // on fonctions defined by the generator
%}

%start commande // main non-terminal


%union { double number };
%token <number> NUMBER
%type <number> expression

%left '+' '-'
%left '*' '/'
%nonassoc UMOINS
%%
commande : 
    expression ';'
                {printf("Resultat= %f\n", $1);}
expression:
    expression '+' expression
                { $$ = $1+$3; }
    | expression '-' expression
                { $$ = $1-$3; }
    | expression '*' expression
                { $$ = $1*$3; }
    | '(' expression ')'
                { $$ = $2; }
    | '-' expression %prec UMOINS
                { $$ = -$2; }
    | NUMBER
                { $$ = $1; }
    ;

   // everything after %% is copied at the end of the generated .c
%%
int yyerror(const char *msg){ // called by the parser if the parsing fails
    printf("Parsing:: syntax error %s\n");
    return 1; // to distinguish with the 0 retured by the success
}