#ifndef REPL_SEMATICANALYZER_H
#define REPL_SEMATICANALYZER_H

#include "SemanticAnalysisResult.h"
#include "SymbolTable.h"
#include "ASTNode.h"

class SemanticAnalyzer {
private:
    struct Scope {
        Scope* outer;
        SymbolTable symbolTable;

        Scope(Scope* outerScope) {
            outer = outerScope;
        }
    };

    SemanticAnalysisResult checkAssignExpr(BinOpNode* node);

    SemanticAnalysisResult checkNumberExpr(ASTNode* node);

    SemanticAnalysisResult checkBoolExpr(ASTNode* node);

    SemanticAnalysisResult checkBoolExprComparison(BinOpNode* node);

    SemanticAnalysisResult checkVarDecl(DeclVarNode* node);

    SemanticAnalysisResult checkId(IdentifierNode* node);

    SemanticAnalysisResult checkFuncDecl(DeclFuncNode* node);

    SemanticAnalysisResult checkFuncCall(FuncCallNode* node);

    SemanticAnalysisResult checkReservedFuncCall(FuncCallNode* node);

    SemanticAnalysisResult checkBreakStmt();

    SemanticAnalysisResult checkReturnStmt(ReturnStmtNode* node);

    SemanticAnalysisResult checkBlockStmt(BlockStmtNode* node);

    SemanticAnalysisResult checkIfStmt(IfStmtNode* node);

    SemanticAnalysisResult checkForLoop(ForLoopNode* node);

    SemanticAnalysisResult checkStatement(ASTNode* node);

    SemanticAnalysisResult newError(SemanticAnalysisResult::Error err);

    SemanticAnalysisResult newError(SemanticAnalysisResult::Error err, const std::string& message);

    Scope* lookTopIdScope(const std::string& idName);

    Scope* globalScope;

    Scope* topScope;

    Scope* functions;

    void openScope();

    void closeScope();

    bool forLoopCheck;

    bool functionBodyCheck;

    bool operationCheck;

    bool isFuncReserved(const std::string& funcName);

    ValueType::Type functionReturnType;
public:
    SemanticAnalyzer(int checkMode) : globalScope(new Scope(nullptr)), topScope(globalScope), functions(globalScope),
                                      forLoopCheck(false), functionBodyCheck(false) {
        operationCheck = checkMode == 0;

        DeclFuncNode* printFunc = new DeclFuncNode;
        printFunc->name = "print";
        printFunc->returnType = ValueType::Void;
        printFunc->argsSize = 1;
        printFunc->body = nullptr;

        functions->symbolTable.addNewFunc(printFunc);
    };

    ~SemanticAnalyzer() {
        delete functions->symbolTable.getFunc("print");
        delete globalScope;
    }

    SemanticAnalysisResult checkProgram(ProgramTranslationNode* root);
};


#endif //REPL_SEMATICANALYZER_H
