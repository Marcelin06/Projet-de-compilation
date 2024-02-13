/* file lexeur.l
* compilation: flex lexeur.l
* result: lex.yy.c = lexical analyser in C
*/

%{
    #include <stdio.h> // printf
    #include "parser.tab.h" // token constants defined in parseur.y via #define
%}

%%

[0-9][0-9]*[.][0-9][0-9]*  { printf("lex::NUMBER %s\n",yytext);
                yylval.number = atof(yytext);
                return NUMBER; }
[ \t\r]        { ; } // separator
\n            { printf("lex: fin de lecture");
                return 0; }
.            { printf("lex: création token %s\n",yytext);
                return yytext[0]; }

%%
int yywrap(void){ return 1; } // function called at the end of the file