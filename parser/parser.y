%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expression.h"

extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(Expr** expr, const char* s);
%}


%parse-param { Expr** expression }

%union {
    char* symbol;
    Expr* expr;

}

%defines "include/parser.h"

%token<symbol> T_SYMBOL
%token T_LAMBDA T_DOT T_LEFT T_RIGHT T_SPACE
%left T_DOT
%left T_SPACE

%type<expr> expr
%type<expr> symbol
%type<expr> binding
%type<expr> application

%start calc

%%

calc: expr { *expression = $1; } ;

expr: symbol			{ $$ = $1; }
    | binding			{ $$ = $1; }
    | application		{ $$ = $1; }
    | T_LEFT expr[E] T_RIGHT	{ $$ = $E; }
;

symbol: T_SYMBOL { $$ = createSymbol($1); free($1); }

binding:
    T_LAMBDA symbol[S] T_DOT expr[E] {
    	$$ = createBinding($S, $E);
    }
;

application:
    expr[L] T_SPACE expr[R] {
    	$$ = createApplication($L, $R);
    }
;


%%

void yyerror(Expr** expr, const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}
