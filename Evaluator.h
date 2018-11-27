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

    EvalResult EvaluateReturnStmt(ReturnStmtNode* subtree);

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

    double EvaluateNumberConstant(ConstNumberNode* num);

    bool EvaluateBoolConstant(ConstBoolNode* num);

    Scope* lookTopIdScope(const std::string& idName);

    Scope* globalScope;

    Scope* topScope;

    Scope* functions;

    void openScope();

    void closeScope();

    bool breakForLoop;

    bool funcReturn;
public:
    Evaluator() : globalScope(new Scope(nullptr)), topScope(globalScope), functions(globalScope),
                  breakForLoop(false), funcReturn(false) {
        DeclFuncNode* funcPrint = new DeclFuncNode;
        funcPrint->name = "print";
        IdentifierNode* idArg = new IdentifierNode;
        idArg->name = "val";
        funcPrint->argsSize = 1;
        funcPrint->args.emplace_back(idArg);
        funcPrint->body = new BlockStmtNode;
        ReturnStmtNode* returnStmt = new ReturnStmtNode;
        IdentifierNode* idReturn = new IdentifierNode;
        idReturn->name = "val";
        returnStmt->expression = idReturn;
        funcPrint->body->stmtList.emplace_back(returnStmt);

        functions->symbolTable.addNewFunc(funcPrint);
    };

    ~Evaluator() {
        delete functions->symbolTable.getFunc("print");
        delete globalScope;
    }

    EvalResult Evaluate(ASTNode* root);
};

#endif //REPL_EVALUATOR_H