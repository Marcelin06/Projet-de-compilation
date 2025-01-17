/* file lexeur.l
* compilation: flex lexeur.l
* result: lex.yy.c = lexical analyser in C
*/

%{
    #include <stdio.h> // printf
    #include "AST.h"
    #include "parser.tab.h" // token constants defined in parseur.y via #define
    #include <string.h>
    #include <math.h>
    #include <stdlib.h>
%}

%%
[0-9][0-9]*[.][0-9][0-9]*           { printf("lex: création token NUMBER %s\n",yytext);
                                        yylval.number = strtod(yytext, NULL);
                                        return NUMBER; }
[0-9]*[.]?[0-9]+[e][-+]?[0-9]+      { printf("lex: création token NUMBER %s\n",yytext);
                                        yylval.number=strtod(yytext, NULL);
                                        return NUMBER; }
[N][a][N]                           { printf("lex: création token NUMBER %s\n",yytext);
                                        yylval.number=strtod("NAN", NULL);
                                        return NUMBER; }
True                                { printf("lex: création token BOOLEAN %s\n",yytext);
                                        yylval.boolean = 1;
                                        return BOOLEAN; }
False                               { printf("lex: création token BOOLEAN %s\n",yytext);
                                        yylval.boolean = 0;
                                        return BOOLEAN; }
[=][=]                              { printf("lex: création token EQUALS %s\n",yytext);
                                        return EQUALS; }
[&][&]                              { printf("lex: création token AND %s\n",yytext);
                                        return AND; }
[<][=]                              { printf("lex: création token LESS_THAN_OR_EQUALS %s\n",yytext);
                                        return LESS_THAN_OR_EQUALS; }
[>][=]                              { printf("lex: création token MORE_THAN_OR_EQUALS %s\n",yytext);
                                        return MORE_THAN_OR_EQUALS; }
[!][=]                              { printf("lex: création token DIFFERENT_FROM %s\n",yytext);
                                        return DIFFERENT_FROM; }
[i][f]                              { printf("lex: création token IF %s\n", yytext);
                                            return IF; }
[d][o]                              { printf("lex: création token DO %s\n", yytext);
                                            return DO; }
[w][h][i][l][e]                     { printf("lex: création token WHILE %s\n", yytext);
                                            return WHILE; }
[e][l][s][e]                        { printf("lex: création token ELSE %s\n", yytext);
                                            return ELSE; }
[i][m][p][o][r][t][ ]               { printf("lex: création token IMPORT %s\n",yytext);
                                        yylval.import = yytext;
                                        return Import;}
[a-z][a-z,A-Z,_,0-9]*               {printf("lex: création token IDENT %s\n",yytext);
                                        yylval.ident = yytext;
                                        return IDENT;}
[!]                                 { printf("lex: création token NOT %s\n",yytext);
                                        return NOT; }
[/][/].*[\n]+                       {/*commentaires sur une ligne*/} 
[/][*]([^*]|\*+[^*/])*[*]+[/]       {/*commentaires sur +sieurs lignes*/}
[\n\t\r ]                           {/*separateurs*/}
.                                   { printf("lex: création token %s\n",yytext);
                                        return yytext[0]; }
%%
int yywrap(void){ return 1; } // function called at the end of the file