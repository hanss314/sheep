#include <stdio.h>

#include "expression.h"
#include "parser.h"
#include "lexer.h"
#include "heap.h"

Expr* applyBinding(Expr*, Expr*);
Expr* evaluate(Expr*);
char* stringify(Expr*);
void* maybePrint(Expr*);

int main(int nargs, char** args) {
    if (nargs < 1){
        fprintf(stderr, "File not specified");
        exit(-1);
    }
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
    maybePrint(evaluated);
    freeExpr(output);
    freeExpr(evaluated);
    deleteHeap();
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
        char* left = expr->name;
        char* right = stringify(expr->right);
        char* ret = malloc(2*sizeof(char)*(strlen(right)+strlen(left)+5));
        strcpy(ret, "λ");
        strcat(ret, left);
        strcat(ret, ".");
        strcat(ret, right);
        free(right);
        return ret;
    }
    fprintf(stderr, "A fatal error has occurred");
    exit(-1);
}
void* maybePrint(Expr* expr){
    if(expr->type != ASSIGN && expr->type != LINE){
        char* string = stringify(expr);
        printf("%s\n", string);
        free(string);
    }
}

Expr* evaluate(Expr* expr){
    if(expr->type == BINDING || expr->type == SYMBOL){
        Expr* value = lookup(expr->name);
        if (value != NULL){
            return dupExpr(value);
        } else {
            return dupExpr(expr);
        }
    } else if (expr->type == LINE){
        Expr* left = evaluate(expr->left);
        maybePrint(left);
        Expr* right = evaluate(expr->right);
        maybePrint(right);
        return createLine(left, right);
    } else if (expr->type == ASSIGN){
        char* name = expr->name;
        Expr* right = evaluate(expr->right);
        heap_insert(name, dupExpr(right));
        return createAssignment(strdup(name), right);
    } else if(expr->type == APPLICATION){
        Expr* left = evaluate(expr->left);
        Expr* right = evaluate(expr->right);
        if(left->type != BINDING){
            return createApplication(left, right);
        } else {
            Expr* ret = applyBinding(left, right);
            freeExpr(left); freeExpr(right);
            Expr* final = evaluate(ret);
            freeExpr(ret);
            return final;
        }
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
        if (strcmp(expr->name, name) == 0){
            ret = dupExpr(expr);
        } else {
            ret = createBinding(
                strdup(expr->name),
                substituteLambda(name, expr->right, content)
            );
        }
    } else {
        fprintf(stderr, "A fatal error has occurred");
        exit(-1);
    }
    return ret;
}

Expr* applyBinding(Expr* lambda, Expr* content){
    char* name = lambda->name;
    Expr* expression = lambda->right;
    Expr* replaced = substituteLambda(name, expression, content);
    return replaced;
}