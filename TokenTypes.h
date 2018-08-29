#ifndef REPL_TOKENTYPES_H
#define REPL_TOKENTYPES_H

struct TokenTypes {
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
};

#endif //REPL_TOKENTYPES_H
