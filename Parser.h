#ifndef BASHCOMPILER_PARSER_H
#define BASHCOMPILER_PARSER_H

#include "Token.h"
#include "Identifier.h"
#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <unordered_map>

class Parser {
private:
    int expression(int lvalue);

    int term(int lvalue);

    int factor();

    enum types {
        eof,
        Num,
        String,
        Keyword,
        DeclareId,
        DeclareFunc,
        Id,
        ROUND_BRACKET_START,
        ROUND_BRACKET_END,
        SQUARE_BRACKET_START,
        SQUARE_BRACKET_END,
        SEMICOLON,
        Assign,
        Add,
        Sub,
        Mul,
        Div,
        Mod,
        Inc,
        Dec
    };

    std::vector<Token> tokens;

    std::vector<Token>::iterator currentToken;

    std::unordered_map<std::string, Identifier> symbol_table;

    Token getNextToken();

    void returnToken();

    int parseMath();

public:
    Parser(std::vector<Token> tokens_list, std::unordered_map<std::string, Identifier> symbol_table) :
            tokens(std::move(tokens_list)), symbol_table(std::move(symbol_table)) {
        currentToken = tokens.begin();
    };

    int parse();
};


#endif //BASHCOMPILER_PARSER_H
