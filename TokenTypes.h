#ifndef REPL_TOKENTYPES_H
#define REPL_TOKENTYPES_H

struct TokenTypes {
    enum types {
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
        BoolOR
    };
};

#endif //REPL_TOKENTYPES_H
