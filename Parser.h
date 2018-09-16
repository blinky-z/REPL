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

    BlockStmtNode* createBlockStmtNode(const std::vector<ASTNode*> statements);

    IfStmtNode* createIfStmtNode(ASTNode* condition, BlockStmtNode* statement);

    ForLoopNode* createForLoopNode();

    double getNumTokenValue(const Token& numToken);

    bool getBoolTokenValue(const Token& boolToken);

    ASTNode* parseExpression();

    BinOpNode* parseAssign();

    DeclVarNode* parseDeclVar();

    IdentifierNode* parseIdentifier();

    BlockStmtNode* parseBlockStmt();

    IfStmtNode* parseIfStmt();

    ForLoopNode* parseForLoop();

    std::queue<Token> convertToReversePolish();

    bool isOperator(const Token& token);

    bool isBinaryOperator(const Token& token);

    bool isLeftAssociative(const Token& token);

    void matchParseComplete();

    void expect(const std::string& expected);

    void errorExpected(const std::string& expected, const Token& foundTok);

    TokenContainer tokens;

    ASTNode* parseStatement();
public:
    ASTNode* parse(const TokenContainer& tokenizedSourceData);
};

#endif //BASHCOMPILER_PARSER_H