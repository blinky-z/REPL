#ifndef REPL_ANALYSISCONTAINER_H
#define REPL_ANALYSISCONTAINER_H

#include "Token.h"
#include "Identifier.h"
#include <vector>
#include <unordered_map>
#include <string>

class AnalysisContainer {
private:
    std::vector<Token> tokens;

    unsigned long currentTokenNum = 0;

    std::unordered_map<std::string, Identifier> symbol_table;
public:
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

    const std::vector<Token>& GetTokens();

    Token GetNextToken();

    void ReturnToken();

    bool IsIdExist(std::string identifierName);

    void AddNewIdentifier(std::string identifierName, const Identifier& identifier);

    void AddNewToken(Token token);
};

#endif //REPL_ANALYSISCONTAINER_H
