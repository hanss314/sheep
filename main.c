#include <stdio.h>
#include "parser.h"
#include "lexer.h"

int main() {
    const char *expr = "λx . λy  . \n(x y)";
    YY_BUFFER_STATE state;


    state = yy_scan_string(expr);

    if (yyparse()) {
        /* error parsing */
        return -1;
    }
    yy_delete_buffer(state);
    yylex_destroy();


    return 0;
}