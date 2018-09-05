#ifndef BASHCOMPILER_PARSER_H
#define BASHCOMPILER_PARSER_H

#include "Token.h"
#include "Identifier.h"
#include "TokenTypes.h"
#include "TokenContainer.h"
#include "ASTNode.h"
#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <unordered_map>

class Parser {
private:
    BinOpNode* createBinOpNode(ASTNodeBinOpType type, ASTNode* left, ASTNode* right);

    NumberNode* createNumberNode(double value);

    BoolNode* createBoolNode(bool value);

    IdentifierNode* createIdentifierNode(std::string name);

    DeclVarNode* createDeclVarNode(IdentifierNode* id, ASTNode* expr);

    MathExprNode* createMathExprNode(ASTNode* expr);

    BoolExprNode* createBoolExprNode(ASTNode* expr);

    ForLoopNode* createForLoopNode();

    ASTNode* createEmptyNode();

    double getNumTokenValue(const Token& numToken);

    bool getBoolTokenValue(const Token& boolToken);

    ASTNode* expression();

    ASTNode* expressionTail(ASTNode* lvalue);

    ASTNode* term();

    ASTNode* termTail(ASTNode* lvalue);

    ASTNode* factor();

    ASTNode* parseMathExpr();

    ASTNode* expressionBool();

    ASTNode* expressionTailBool(ASTNode* lvalue);

    ASTNode* termBool();

    ASTNode* termTailBool(ASTNode* lvalue);

    ASTNode* factorBool();

    ASTNode* parseBoolExpr();

    ASTNode* parseExpr();

    ASTNode* parseAssign();

    ASTNode* parseDeclVar();

    ASTNode* parseEqual();

    ASTNode* parseId();

    ASTNode* parseForLoop();

    bool isStmtEquality();

    bool matchParseComplete();

    TokenContainer tokens;
public:
    ASTNode* parse(const TokenContainer& tokenizedSourceData);
};

#endif //BASHCOMPILER_PARSER_H
