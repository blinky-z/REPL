#ifndef REPL_EVALUATOR_H
#define REPL_EVALUATOR_H

#include "ASTNode.h"
#include "SymbolTable.h"
#include "EvalResult.h"
#include <iostream>

class Evaluator {
private:
    struct Scope {
        Scope* outer;
        SymbolTable symbolTable;

        Scope(Scope* outerScope) {
            outer = outerScope;
        }
    };

    EvalResult EvaluateMathExpr(ASTNode* subtree);

    EvalResult EvaluateBoolExpr(ASTNode* subtree);

    EvalResult EvaluateAssignValue(IdentifierNode* id, ASTNode* expr);

    EvalResult EvaluateFuncCall(FuncCallNode* funcCall);

    EvalResult EvaluateDeclFunc(DeclFuncNode* subtree);

    EvalResult EvaluateDeclVar(DeclVarNode* subtree);

    EvalResult EvaluateEqual(BinOpNode* subtree);

    EvalResult EvaluateComparison(BinOpNode* subtree);

    EvalResult EvaluateBlockStmt(BlockStmtNode* subtree);

    EvalResult EvaluateIfStmt(IfStmtNode* subtree);

    EvalResult EvaluateForLoopStmt(ForLoopNode* subtree);

    double EvaluateIdDouble(Scope* scope, IdentifierNode* id);

    bool EvaluateIdBool(Scope* scope, IdentifierNode* id);

    double EvaluateNumberConstant(NumberNode* num);

    bool EvaluateBoolConstant(BoolNode* num);

    EvalError newError(EvalError::Error err);

    EvalError newError(EvalError::Error err, const std::string errMessage);

    Scope* lookTopIdScope(const std::string& idName);

    Scope* topScope;

    Scope* globalScope;

    Scope* functions;

    void openScope();

    void closeScope();
public:
    Evaluator() {
        globalScope = new Scope(nullptr);
        topScope = globalScope;
        functions = new Scope(nullptr);
    }

    ~Evaluator() {
        delete functions;

        while (topScope != nullptr) {
            Scope* oldScope = topScope;
            topScope = topScope->outer;
            delete oldScope;
        }
    }

    EvalResult Evaluate(ASTNode* root);
};

#endif //REPL_EVALUATOR_H