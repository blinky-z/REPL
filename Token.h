#ifndef BASHCOMPILER_TOKEN_H
#define BASHCOMPILER_TOKEN_H

#include <string>

namespace TokenType {
    enum {
        eof,
        Num,
        Bool,
        String,
        DeclareId,
        DeclareFunc,
        IfStmt,
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
