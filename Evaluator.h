#ifndef REPL_EVALUATOR_H
#define REPL_EVALUATOR_H

#include "ASTNode.h"
#include "SymbolTable.h"
#include "EvalResult.h"
#include <iostream>

class Evaluator {
private:
    SymbolTable symbolTable;

    EvalResult EvaluateMathExpr(ASTNode* subtree);

    EvalResult EvaluateBoolExpr(ASTNode* subtree);

    EvalResult EvaluateAssignValue(IdentifierNode* lvalue, ASTNode* expr);

    EvalResult EvaluateDeclVar(DeclVarNode* subtree);

    EvalResult EvaluateEqual(BinOpNode* subtree);

    EvalResult EvaluateComparison(BinOpNode* subtree);

    double EvaluateIdDouble(IdentifierNode* id);

    bool EvaluateIdBool(IdentifierNode* id);

    double EvaluateNumberConstant(NumberNode* num);

    bool EvaluateBoolConstant(BoolNode* num);

    EvalError newError(EvalError::Error err);

    EvalError newError(EvalError::Error err, const std::string errMessage);

public:
    EvalResult Evaluate(ASTNode* root);
};

#endif //REPL_EVALUATOR_H