%option noyywrap

%{
#include <stdio.h>
#include <string.h>

#define YY_DECL int yylex()

#include "expression.h"
#include "parser.h"

%}

%option header-file="include/lexer.h"

%%

"λ"		                { return T_LAMBDA; }
[ \t\n]*"\."[ \t\n]*	{ return T_DOT; }
[ \t\n]*";"[ \t\n]*	    { return T_SEMICOLON; }
[ \t\n]*":="[ \t\n]*	{ return T_ASSIGN; }
[ \t\n]+	            { return T_SPACE; }
"("		                { return T_LEFT;}
")"		                { return T_RIGHT;}
[^λ \t\n\(\)\.;=]+      { yylval.symbol = strdup(yytext); return T_SYMBOL;}
%%
