a := (λx.λy.(y x)) ((λx.(x x)) (λx.q)) (λx.(x λx.x));
b := (λx.λy.(x y)) ((λx.(x x)) (λx.q)) (λx.(x x x x));
a; b;