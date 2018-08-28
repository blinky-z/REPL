#ifndef BASHCOMPILER_PARSER_H
#define BASHCOMPILER_PARSER_H

#include "Token.h"
#include "Identifier.h"
#include "AnalysisContainer.h"
#include "ASTNode.h"
#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <unordered_map>

class Parser {
private:
    ASTNode* createBinOpNode(ASTNodeBinOpType type, ASTNode* left, ASTNode* right);

    ASTNode* createNumberNode(int value);

    ASTNode* createIdentifierNode(std::string name);

    ASTNode* createEmptyNode();

    int getNumTokenValue(const Token& numToken);

    ASTNode* expression();

    ASTNode* expressionTail(ASTNode* lvalue);

    ASTNode* term();

    ASTNode* termTail(ASTNode* lvalue);

    ASTNode* factor();

    ASTNode* parseMath();

    ASTNode* parseAssign();

    AnalysisContainer data;
public:
    ASTNode* parse(const AnalysisContainer& tokenizingStepData);
};

#endif //BASHCOMPILER_PARSER_H
