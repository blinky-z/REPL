#ifndef REPL_COMPILER_H
#define REPL_COMPILER_H

#include "ASTNode.h"
#include <string>
#include <map>
#include <cmath>
#include <algorithm>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsequence-point"
#include "sole/sole.hpp"
#pragma GCC diagnostic pop

class BashGenerator {
private:
    struct Scope {
        Scope* outer;
        std::unordered_map<std::string, std::string> uuid;

        Scope(Scope* outerScope) {
            outer = outerScope;
        }
    };

    std::string getUuid();

    void openScope();

    void closeScope();

    std::string lookTopId(const std::string& id);

    Scope* topScope;

    int getOperatorPrecedence(BinOpNode* binOp);

    std::string generateConstNumber(ConstNumberNode* node);

    std::string generateConstBool(ConstBoolNode* node);

    std::string generateId(IdentifierNode* node);

    std::string generateReservedFuncCall(FuncCallNode* node);

    std::string generateFuncCall(FuncCallNode* node);

    std::string generateDeclVar(DeclVarNode* node);

    std::string generateDeclFunc(DeclFuncNode* node);

    std::string generateBreakStmt();

    std::string generateIfStmt(IfStmtNode* node);

    std::string generateForLoop(ForLoopNode* node);

    std::string generateBlockStmt(BlockStmtNode* node);

    std::string generateReturnStmt(ReturnStmtNode* node);

    std::string generateBinaryExprMath(BinOpNode* node);

    std::string generateBinaryExprAssign(BinOpNode* node);

    std::string generateGetValue(ASTNode* node);

    std::string generateStatement(ASTNode* node);

    void addTabs(std::string& result);

    int tabCount;

    bool blockScope;

    bool isFuncReserved(const std::string& funcName);
public:
    std::string generate(ProgramTranslationNode* root);

    BashGenerator(): topScope(new Scope(nullptr)), tabCount(0), blockScope(false) {};

    ~BashGenerator() {
        delete topScope;
    }
};


#endif //REPL_COMPILER_H
