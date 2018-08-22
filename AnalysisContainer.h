#ifndef REPL_ANALYSISCONTAINER_H
#define REPL_ANALYSISCONTAINER_H

#include <vector>
#include <unordered_map>
#include "Token.h"
#include "Identifier.h"

class AnalysisContainer {
private:
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
public:
    Token getNextToken();

    void returnToken();
};

#endif //REPL_ANALYSISCONTAINER_H
