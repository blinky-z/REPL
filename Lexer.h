#ifndef BASHCOMPILER_LEXER_H
#define BASHCOMPILER_LEXER_H

#include "Token.h"
#include "Identifier.h"
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class Lexer {
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

    int line = 1;
public:
    void tokenize(char* src, vector<Token>& tokens, unordered_map<string, Identifier>& symbol_table);
};

#endif //BASHCOMPILER_LEXER_H
