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

    bool EvaluateBoolExpr(ASTNode* subtree);

    void EvaluateAssignValue(BinOpNode* subtree);

    void EvaluateDeclVar(DeclVarNode* subtree);

    bool EvaluateEqual(BinOpNode* subtree);

    double EvaluateIdDouble(IdentifierNode* id);

    bool EvaluateIdBool(IdentifierNode* id);

    double EvaluateNumberValue(NumberNode* num);

    bool EvaluateBoolValue(BoolNode* num);
public:
    std::string Evaluate(ASTNode* root);
};

#endif //REPL_EVALUATOR_H
