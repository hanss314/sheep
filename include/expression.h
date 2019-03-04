#ifndef SHEEP_EXPRESSION_H
#define SHEEP_EXPRESSION_H

typedef enum ExprType {
    BINDING,
    APPLICATION,
    SYMBOL,
//    ASSIGN,
//    LINE
} ExprType;

typedef struct Expression{
    ExprType type;
    struct Expression* left;  // null if symbol
    struct Expression* right; // null if symbol
    char* name; // only valid for symbol
} Expr;

Expr* createSymbol(char*);
Expr* createBinding(char*, Expr*);
Expr* createApplication(Expr*, Expr*);
Expr* dupExpr(Expr* expr);
void freeExpr(Expr* expr);

#endif //SHEEP_EXPRESSION_H
