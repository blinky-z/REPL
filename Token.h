#ifndef BASHCOMPILER_TOKEN_H
#define BASHCOMPILER_TOKEN_H

#include <string>

namespace TokenType {
    enum {
        eof,
        NL,
        SEMICOLON,
        Comma,
        Number,
        Bool,
        DeclareId,
        DeclareFunc,
        Return,
        IntType,
        BoolType,
        FuncReturnVoid,
        IfStmt,
        ElseStmt,
        ForLoopStmt,
        Id,
        FuncCall,
        ROUND_BRACKET_START,
        ROUND_BRACKET_END,
        SQUARE_BRACKET_START,
        SQUARE_BRACKET_END,
        CURLY_BRACKET_START,
        CURLY_BRACKET_END,
        LESS,
        GREATER,
        Assign,
        Add,
        Sub,
        Mul,
        Div,
        UnaryMinus,
        BoolAND,
        BoolOR,
        Equal,
        Break
    };
}

struct Token {
    int Type;
    std::string Value;
};

#endif //BASHCOMPILER_TOKEN_H
