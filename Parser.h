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
    ASTNode* createNode(ASTNodeType type, ASTNode* left, ASTNode* right);

    ASTNode* createNodeNumber(int value);

    ASTNode* createEmptyNode();

    ASTNode* expression();

    ASTNode* expression1();

    ASTNode* term();

    ASTNode* term1();

    ASTNode* factor();

    ASTNode* parseMath();

    AnalysisContainer data;
public:
    Parser(AnalysisContainer data) : data(std::move(data)) {};

    ASTNode* parse();
};


#endif //BASHCOMPILER_PARSER_H
