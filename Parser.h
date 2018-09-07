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

class Parser {
private:
    BinOpNode* createBinOpNode(BinOpType::ASTNodeBinOpType type, ASTNode* left, ASTNode* right);

    NumberNode* createNumberNode(double value);

    BoolNode* createBoolNode(bool value);

    IdentifierNode* createIdentifierNode(std::string name);

    DeclVarNode* createDeclVarNode(IdentifierNode* id, ASTNode* expr);

    ForLoopNode* createForLoopNode();

    ASTNode* createEmptyNode();

    double getNumTokenValue(const Token& numToken);

    bool getBoolTokenValue(const Token& boolToken);

    ASTNode* expression();

    ASTNode* expressionTail(ASTNode* lvalue);

    ASTNode* term();

    ASTNode* termTail(ASTNode* lvalue);

    ASTNode* factor();

    ASTNode* parseAssign();

    ASTNode* parseDeclVar();

    ASTNode* parseId();

    ASTNode* parseForLoop();

    bool matchParseComplete();

    TokenContainer tokens;
public:
    ASTNode* parse(const TokenContainer& tokenizedSourceData);
};

#endif //BASHCOMPILER_PARSER_H
