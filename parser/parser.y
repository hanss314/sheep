%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(const char* s);
%}


//%parse-param { const char **expression }

%union {
    char* symbol;
}

%defines "include/parser.h"

%token<symbol> T_SYMBOL
%token T_LAMBDA T_DOT T_LEFT T_RIGHT T_SPACE
%left T_DOT
%left T_SPACE

%type<symbol> expr
%type<symbol> binding
%type<symbol> application

%start calc

%%

calc: expr { printf("%s\n", $1); free($1); } ;

expr: T_SYMBOL
    | binding			{ $$ = $1; }
    | application		{ $$ = $1; }
    | T_LEFT expr[E] T_RIGHT	{
    	char* expr = $E;
    	char* str = malloc(sizeof(char)*256);
    	strcpy(str, "(");
	strcat(str, expr);
	strcat(str, ")");
	free(expr);
	$$ = str;
    }
;

binding:
    T_LAMBDA T_SYMBOL T_DOT expr {
    	char* left = $2;
    	char* right = $4;
    	char* str = malloc(sizeof(char)*256);
    	strcpy(str, "λ");
    	strcat(str, left);
    	strcat(str, ".");
    	strcat(str, right);
    	free(left); free(right);
    	$$ = str;
    }
;

application:
    expr[L] T_SPACE expr[R] {
    	char* left = $L;
	char* right = $R;
    	char* str = malloc(sizeof(char)*256);
	strcpy(str, left);
	strcat(str, " ");
	strcat(str, right);
	free(left); free(right);
	$$ = str;
    }
;


%%

void yyerror(const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}