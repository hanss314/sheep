#include <stdio.h>

#include "expression.h"
#include "parser.h"
#include "lexer.h"

Expr* applyBinding(Expr*, Expr*);
Expr* evaluate(Expr* expr);
char* stringify(Expr* expr);

int main(int nargs, char** args) {
    if (nargs < 1){
        fprintf(stderr, "File not specified");
        exit(-1);
    }
    YY_BUFFER_STATE state;
    Expr* output;
    //const char *expr = "(λx.λy.(x y)) ((λx.(x x)) (λx.a))";
    //state = yy_scan_string(expr);
    yyin = fopen(args[1], "r");
    if(yyin==NULL){
        fprintf(stderr, "File %s not found\n", args[1]);
        exit(-1);
    }

    if (yyparse(&output)) {
        /* error parsing */
        return -1;
    }
    yylex_destroy();
    Expr* evaluated = evaluate(output);
    char* o = stringify(evaluated);
    printf("%s\n", o);
    freeExpr(output);
    freeExpr(evaluated);
    free(o);
    return 0;
}

char* stringify(Expr* expr){
    if(expr->type==SYMBOL){
        return strdup(expr->name);
    } else if (expr->type==APPLICATION){
        char* left = stringify(expr->left);
        char* right = stringify(expr->right);
        char* ret = malloc(2*sizeof(char)*(strlen(right)+strlen(left)+5));
        strcpy(ret, "(");
        strcat(ret, left);
        strcat(ret, " ");
        strcat(ret, right);
        strcat(ret, ")");
        free(left); free(right);
        return ret;
    } else if(expr->type==BINDING){
        char* left = stringify(expr->left);
        char* right = stringify(expr->right);
        char* ret = malloc(2*sizeof(char)*(strlen(right)+strlen(left)+5));
        strcpy(ret, "λ");
        strcat(ret, left);
        strcat(ret, ".");
        strcat(ret, right);
        free(left); free(right);
        return ret;
    }
    fprintf(stderr, "A fatal error has occurred");
    exit(-1);
}

Expr* evaluate(Expr* expr){
    Expr* ret;
    if(expr->type != APPLICATION){
        return dupExpr(expr);
    }
    Expr* left = evaluate(expr->left);
    Expr* right = evaluate(expr->right);
    if(left->type != BINDING){
        return createApplication(left, right);
    } else {
        ret = applyBinding(left, right);
        freeExpr(left); freeExpr(right);
        Expr* final = evaluate(ret);
        freeExpr(ret);
        return final;
    }

}

Expr* substituteLambda(char* name, Expr* expr, Expr* content){
    Expr* ret;
    if (expr->type == SYMBOL) {
        if(strcmp(expr->name, name) == 0) {
            ret = dupExpr(content);
        } else {
            ret = dupExpr(expr);
        }
    } else if (expr->type == APPLICATION){
        ret = createApplication(
                substituteLambda(name, expr->left, content),
                substituteLambda(name, expr->right, content)
                );
    } else if (expr->type == BINDING){
        ret = createBinding(
                substituteLambda(name, expr->left, content),
                substituteLambda(name, expr->right, content)
                );
    } else {
        fprintf(stderr, "A fatal error has occurred");
        exit(-1);
    }
    return ret;
}

Expr* applyBinding(Expr* lambda, Expr* content){
    Expr* variable = lambda->left;
    if(variable->type != SYMBOL){
        fprintf(stderr, "Attempted to bind non symbol");
    }
    char* name = variable->name;
    Expr* expression = lambda->right;
    Expr* replaced = substituteLambda(name, expression, content);
    return replaced;
}