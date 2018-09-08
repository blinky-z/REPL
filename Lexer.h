#ifndef BASHCOMPILER_LEXER_H
#define BASHCOMPILER_LEXER_H

#include "Token.h"
#include "Identifier.h"
#include "TokenContainer.h"
#include <vector>
#include <string>
#include <unordered_map>

namespace TokenType {
    enum {
        eof,
        Num,
        Bool,
        String,
        DeclareId,
        DeclareFunc,
        DeclareForLoop, // not implemented
        Id,
        ROUND_BRACKET_START,
        ROUND_BRACKET_END,
        SQUARE_BRACKET_START,
        SQUARE_BRACKET_END,
        LESS,
        GREATER,
        SEMICOLON,
        Assign,
        Add,
        Sub,
        Mul,
        Div,
        Mod, // not implemented
        Inc,
        Dec,
        BoolAND,
        BoolOR,
        Equal
    };
}

class Lexer {
private:
    std::string::const_iterator currentChar;

    const Token tokenizeStringLiteral();

    const Token tokenizeNumber();

public:
    const TokenContainer tokenize(const std::string& src);
};

#endif //BASHCOMPILER_LEXER_H
