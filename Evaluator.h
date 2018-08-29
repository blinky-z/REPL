#ifndef REPL_EVALUATOR_H
#define REPL_EVALUATOR_H

#include "ASTNode.h"
#include "SymbolTable.h"
#include "TokenTypes.h"
#include <iostream>

class Evaluator {
private:
    SymbolTable symbolTable;

    double EvaluateMathExpr(ASTNode* subtree);

    void EvaluateAssignValue(BinOpNode* subtree);

    void EvaluateDeclVar(DeclVarNode* subtree);

    double EvaluateId(IdentifierNode* subtree);

    double EvaluateNumberValue(NumberNode* subtree);

public:
    std::string Evaluate(ASTNode* root);
};

#endif //REPL_EVALUATOR_H
