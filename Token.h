#ifndef BASHCOMPILER_TOKEN_H
#define BASHCOMPILER_TOKEN_H

#include <string>

namespace TokenType {
    enum {
        eof,
        NL,
        Number,
        Bool,
        String,
        DeclareId,
        DeclareFunc,
        IfStmt,
        ForLoopStmt, // not implemented
        Id,
        ROUND_BRACKET_START,
        ROUND_BRACKET_END,
        SQUARE_BRACKET_START,
        SQUARE_BRACKET_END,
        CURLY_BRACKET_START,
        CURLY_BRACKET_END,
        LESS,
        GREATER,
        SEMICOLON,
        Assign,
        Add,
        Sub,
        Mul,
        Div,
        Mod, // not implemented
        UnaryMinus,
        Inc, // not implemented
        Dec, // not implemented
        BoolAND,
        BoolOR,
        Equal
    };
}

struct Token {
    int Type;
    std::string Value;
};

#endif //BASHCOMPILER_TOKEN_H
