#include <iostream>
#include "Lexer.h"

AnalysisContainer Lexer::tokenize(char* src) {
    while (*src != EOF) {
        Token token;

        while (*src == ' ') {
            src++;
        }
        while (*src == '\n') {
            line++;
            src++;
        }

        if ((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src == '_')) {
            token = tokenizeIdentifier(&src);
        } else if (*src >= '0' && *src <= '9') {
            token = tokenizeNumber(&src);
        } else if (*src == '=') {
            token.type = data.Assign;
            token.value = "=";
        } else if (*src == '+') {
            if (*(src + 1) == '+') {
                src++;

                token.type = data.Inc;
                token.value = "++";
            } else {
                token.type = data.Add;
                token.value = "+";
            }
        } else if (*src == '-') {
            if (*(src + 1) == '-') {
                src++;

                token.type = data.Dec;
                token.value = "--";
            } else {
                token.type = data.Sub;
                token.value = "-";
            }
        } else if (*src == '*') {
            token.type = data.Mul;
            token.value = "*";
        } else if (*src == '/') {
            token.type = data.Div;
            token.value = "/";
        } else if (*src == '%') {
            token.type = data.Mod;
            token.value = "%";
        } else if (*src == '(') {
            token.type = data.ROUND_BRACKET_START;
            token.value = "(";
        } else if (*src == ')') {
            token.type = data.ROUND_BRACKET_END;
            token.value = ")";
        } else if (*src == '[') {
            token.type = data.SQUARE_BRACKET_START;
            token.value = "[";
        } else if (*src == ']') {
            token.type = data.SQUARE_BRACKET_END;
            token.value = "]";
        } else if (*src == ';') {
            token.type = data.SEMICOLON;
            token.value = ";";
        }

        data.AddNewToken(token);
        src++;
    }

    data.AddNewToken({data.eof, "EOF"});

    return data;
}

Token Lexer::tokenizeIdentifier(char** src) {
    Token token;

    string id_name;
    id_name += *src;

    while ((**(src + 1) >= 'a' && **(src + 1) <= 'z') || (**(src + 1) >= 'A' && **(src + 1) <= 'Z') ||
           (**(src + 1) >= '0' && **(src + 1) <= '9') || (**(src + 1) == '_')) {
        src++;
        id_name += *src;
    }

    // объявление переменной
    if (id_name == "var") {
        token.type = data.DeclareId;
        token.value = "var";
    } else {
        token.type = data.Id;
        token.value = id_name;

        // если такого Identifier в таблице символов ещё нет, внести данные о нем
        if (!data.IsIdExist(id_name)) {
            data.AddNewIdentifier(id_name, Identifier{});
        }
    }

    return token;
}

Token Lexer::tokenizeNumber(char** src) {
    Token token;

    int token_val = **src - '0';

    while (*(*src + 1) >= '0' && *(*src + 1) <= '9') {
        (*src)++;
        token_val = token_val * 10 + **src - '0';
    }

    token.type = data.Num;
    token.value = to_string(token_val);

    return token;
}
