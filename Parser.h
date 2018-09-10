#ifndef BASHCOMPILER_PARSER_H
#define BASHCOMPILER_PARSER_H

#include "Token.h"
#include "Identifier.h"
#include "TokenContainer.h"
#include "ASTNode.h"
#include "Lexer.h"
#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>

class Parser {
private:
    BinOpNode* createBinOpNode(BinOpType::ASTNodeBinOpType type, ASTNode* left, ASTNode* right);

    NumberNode* createNumberNode(double value);

    BoolNode* createBoolNode(bool value);

    IdentifierNode* createIdentifierNode(std::string name);

    DeclVarNode* createDeclVarNode(IdentifierNode* id, ASTNode* expr);

    ForLoopNode* createForLoopNode();

    double getNumTokenValue(const Token& numToken);

    bool getBoolTokenValue(const Token& boolToken);

    ASTNode* parseExpression();

    ASTNode* parseAssign();

    ASTNode* parseDeclVar();

    ASTNode* parseId();

    ASTNode* parseForLoop();

    std::queue<Token> convertExpr();

    bool isOperator(const Token& token);

    bool isBinaryOperator(const Token& token);

    bool isLeftAssociative(const Token& token);

    bool matchParseComplete();

    TokenContainer tokens;
public:
    ASTNode* parse(const TokenContainer& tokenizedSourceData);
};

#endif //BASHCOMPILER_PARSER_H
