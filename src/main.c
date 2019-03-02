#include <stdio.h>

#include "expression.h"
#include "parser.h"
#include "lexer.h"


int main() {
    const char *expr = "λx . λy  . \n(x y)";
    YY_BUFFER_STATE state;
    Expr* output;


    state = yy_scan_string(expr);

    if (yyparse(&output)) {
        /* error parsing */
        return -1;
    }
    freeExpr(output);
    yy_delete_buffer(state);
    yylex_destroy();


    return 0;
}