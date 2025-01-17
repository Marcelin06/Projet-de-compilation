/* file parseur.y
* compilation: bison -d parseur.y
* result: parseur.tab.c = C code for syntaxic analyser
* result: parseur.tab.h = def. of lexical units aka lexems
*/

%{ // the code between %{ and %} is copied at the start of the generated .c
#include <stdio.h>

#include "AST.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
int yylex(void); // -Wall : avoid implicit call
int yyerror(AST_prog *arg, const char*); // on fonctions defined by the generator
%}


%union { double number ; int boolean ; AST_expr expr ; AST_comm comm ; AST_prog prog ; char* import; char *ident;}
%token <number> NUMBER // kinds of non-trivial tokens expected from the lexer
%token <boolean> BOOLEAN //token for boolean
%token <import> Import //token for key word import
%token <ident> IDENT //token for name of variables and functions
%type <expr> expression 
%type <comm> command
%type <prog> program
%token EQUALS //token for the multisymbol '=='
%token MORE_THAN_OR_EQUALS //token for the multisymbol '>='
%token LESS_THAN_OR_EQUALS //token for the multisymbol '<='
%token DIFFERENT_FROM //token for the multisymbol '!='
%token NOT //token for !
%token AND //token for "&&"
%token IF
%token ELSE
%token DO
%token WHILE




%start program // main non-terminal

%left '='
%left AND
%left EQUALS DIFFERENT_FROM
%left MORE_THAN_OR_EQUALS LESS_THAN_OR_EQUALS '<' '>'
%left '+' '-'
%left '%'
%left '*' '/'
%nonassoc UMOINS


%parse-param {AST_prog *rez}

%nonassoc NOT

%%
program : 
    /* */
                {
                    
                    AST_prog p = NULL;
                    AST_comm c = NULL;
                    $$ = new_prog(c, p); 
                    *rez = $$;
                }
    |command program
                {
                    $$ = new_prog($1, $2);
                    *rez = $$;              
                } 
    
    ;


command :
    expression ';'
                {$$ = new_command('c', $1);}
    |Import IDENT ';'
                {   AST_expr e = NULL;
                    $$ = new_command('i', new_ident_expr(strtok($1, "=;+-=*/*<%!>.()")));
                }
    |';'
                {
                    $$ = new_null_command('n');
                }

    |'{' program '}'
                {
                    $$ = new_command_prog('p', $2);
                }
    |program
                {
                    $$ = new_command_prog('p', $1);
                }
    |IF '(' expression ')' '{' command '}' ELSE '{' command '}'
                {
                    $$ = new_if_then_else_command('f', $3, $6, $10);
                }
    |WHILE '(' expression ')' '{' command '}'
                {
                    $$ = new_while_command('w', $3, $6);
                }
    |DO '{' command '}' WHILE '(' expression ')'
                {
                    $$ = new_do_while_command('d', $3, $7);
                }


expression : 
    BOOLEAN
                { $$ = new_boolean_expr($1); }     
    |NUMBER
                { $$ = new_number_expr($1); }
    |IDENT 
                { $$ = new_ident_expr(strtok($1, "=;+-=*/*<%!>.()")); }
    | expression '+' expression
                { $$ = new_binary_expr('+',$1,$3); }
    | expression '-' expression
                { $$ = new_binary_expr('-',$1,$3); }
    | expression '*' expression
                { $$ = new_binary_expr('*',$1,$3); }
    | expression '/' expression
                { $$ = new_binary_expr('/',$1,$3); }
    | expression '%' expression
                { $$ = new_binary_expr('%',$1,$3); }
    | '(' expression ')'
                { $$ = $2; }
    | '-' expression %prec UMOINS
                { $$ = new_unary_expr('M',$2); }
    | expression EQUALS expression
                { $$ = new_binary_expr('E',$1,$3); }
    | expression MORE_THAN_OR_EQUALS expression
                { $$ = new_binary_expr('G',$1,$3); }
    | expression LESS_THAN_OR_EQUALS expression
                { $$ = new_binary_expr('L',$1,$3); }
    | expression DIFFERENT_FROM expression
                { $$ = new_binary_expr('D',$1,$3); }
    | expression '>' expression
                { $$ = new_binary_expr('>',$1,$3); }
    | expression '<' expression
                { $$ = new_binary_expr('<',$1,$3); }
    | IDENT '=' expression
                { $$ = new_binary_expr('=',new_ident_expr(strtok($1, "=;+-=*/*<%!>.()")),$3);}
    | expression AND expression
                { $$ = new_binary_expr('A',$1,$3); }
    | NOT expression %prec NOT
                { $$ = new_unary_expr('!',$2); }
    ;
   // everything after %% is copied at the end of the generated .c
%%

int yyerror(AST_prog *arg, const char *msg){ // called by the parser if the parsing fails
    printf("Parsing:: syntax error %s\n");
    return 1; // to distinguish with the 0 retured by the success
}