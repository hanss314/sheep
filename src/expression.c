#include <stdlib.h>
#include <string.h>

#include "expression.h"

Expr* createExpr(ExprType type){
    Expr* expr = (Expr*)malloc(sizeof(Expr));
    expr->type = type;
    return expr;

}

Expr* createSymbol(char* name){
    Expr* expr = createExpr(SYMBOL);
    expr->name = strdup(name);
    return expr;
}

Expr* createBinding(Expr* symbol, Expr* output){
    Expr* lambda = createExpr(BINDING);
    lambda->left = symbol;
    lambda->right = output;
    return lambda;
}

Expr* createApplication(Expr* left, Expr* right){
    Expr* application = createExpr(APPLICATION);
    application->left = left;
    application->right = right;
    return application;
}

void freeExpr(Expr* expr){
    switch (expr->type){
        case SYMBOL: free(expr->name); break;
        default:
            freeExpr(expr->left);
            freeExpr(expr->right);
            break;
    }
    free(expr);
}

Expr* dupExpr(Expr* expr){
    Expr* dup = createExpr(expr->type);
    switch (expr->type){
        case SYMBOL: dup->name = strdup(expr->name); break;
        default:
            dup->left = dupExpr(expr->left);
            dup->right = dupExpr(expr->right);
            break;
    }
    return dup;
}