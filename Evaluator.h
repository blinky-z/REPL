#ifndef REPL_EVALUATOR_H
#define REPL_EVALUATOR_H

#include "ASTNode.h"
#include <iostream>

class Evaluator {
private:
    double EvaluateSubtree(ASTNode* subtree);
public:
    double Evaluate(ASTNode* root);
};

#endif //REPL_EVALUATOR_H
