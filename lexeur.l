/* file lexeur.l
* compilation: flex lexeur.l
* result: lex.yy.c = lexical analyser in C
*/

%{
    #include <stdio.h> // printf
    #include "parser.tab.h" // token constants defined in parseur.y via #define
    #include <string.h>
    #include <math.h>
%}

%%

[0-9][0-9]*[.][0-9][0-9]*               { printf("lex: création token NUMBER %s\n",yytext);
                                            return NUMBER; }
[0-9]*[.]?[0-9]+[e][-+]?[0-9]+          { printf("lex: création token NUMBER %s\n",yytext);
                                            return NUMBER; }
[N][a][N]                               { printf("lex: création token NUMBER %s\n",yytext);
                                            return NUMBER; }
True                                    { printf("lex: création token BOOLEAN %s\n",yytext);
                                            return BOOLEAN; }
False                                   { printf("lex: création token BOOLEAN %s\n",yytext);
                                            return BOOLEAN; }
[=][=]                                  { printf("lex: création token EQUALS %s\n",yytext);
                                            return EQUALS; }
[&][&]                                  { printf("lex: création token AND %s\n",yytext);
                                            return AND; }
[<][=]                                  { printf("lex: création token LESS_THAN_OR_EQUALS %s\n",yytext);
                                            return LESS_THAN_OR_EQUALS; }
[>][=]                                  { printf("lex: création token MORE_THAN_OR_EQUALS %s\n",yytext);
                                            return MORE_THAN_OR_EQUALS; }
[!][=]                                  { printf("lex: création token DIFFERENT_FROM %s\n",yytext);
                                            return DIFFERENT_FROM; } 
[i][m][p][o][r][t][ ]                   { printf("lex: création token %s\n",yytext);
                                            return Import; }
[a-z,A-Z,$,_][a-z,A-Z,$,_,0-9]*         { printf("lex: création token IDENT %s\n",yytext);
                                            return IDENT; }
[!]                                     { printf("lex: création token NOT %s\n",yytext);
                                            return NOT; }            
[\n\t\r]                                { printf("lex: fin de lecture");
                                            return 0; } // separator
.                                       { printf("lex: création token %s\n",yytext);
                                            return yytext[0]; }

%%
int yywrap(void){ return 1; } // function called at the end of the file