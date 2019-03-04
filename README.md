## Sheep

Sheep is a minimalistic lambda calculus interpreter.
It supports the basic features of lambda calculus, 
as well as unsafe assignment.

Valid expressions are symbols, 
which are arbitrary length combinations of characters that are not whitespace, `λ`, 
parentheses, `.`, `:` or `=`. 

Applications are expressions, denoted by two expressions seperated by whitespace.

A lambda is `λ`, followed by a symbol, then a `.`, then an expression.

A statement is either an expression followed by a semicolon or an assignment.

An assignment is a symbol, then `:=`, then an expression, then a semicolon.

A statement is always followed by a semicolon.

If a statement is an assignment, 
it will evaluate the expression then assign it to the symbol. 
If a statement is not an assignment, 
it will evaluate the expression then print the expression.

A program is executed statement by statement in order.

A symbol, when assigned to, evaluates to what it has been assigned to.
Otherwise, it evaluates to itself.

A lambda evaluates to itself.

If the left-hand side of an application is not a lambda, 
it will evaluate to itself. Otherwise, it will evaluate 
to the expression of the left hand lambda, 
with all instances of the symbol bound to the lambda 
replaced by the right hand side of the application. 
The only exception to this is if the symbol 
is in the expression of a lambda bound to the same symbol. 
In this case, the symbols would not be replaced.

An example program: ```
a := (λx.λy.(y x)) ((λx.(x x)) (λx.q)) (λx.(x λx.x));
b := (λx.λy.(x y)) ((λx.(x x)) (λx.q)) (λx.(x x x x));
a; b;```
outputs
```
(q λx.x)
(q λx.(((x x) x) x))```
