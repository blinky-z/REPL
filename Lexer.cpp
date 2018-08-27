#include <iostream>
#include "Lexer.h"

AnalysisContainer Lexer::tokenize(const std::string& src) {
    initLexer(src);

    while (*currentChar != EOF) {
        Token token;

        while (*currentChar == ' ') {
            currentChar++;
        }
        while (*currentChar == '\n') {
            line++;
            currentChar++;
        }

        if ((*currentChar >= 'a' && *currentChar <= 'z') || (*currentChar >= 'A' && *currentChar <= 'Z') ||
            (*currentChar == '_')) {
            token = tokenizeIdentifier();
        } else if (*currentChar >= '0' && *currentChar <= '9') {
            token = tokenizeNumber();
        } else if (*currentChar == '=') {
            token.Type = data.Assign;
            token.Value = "=";
        } else if (*currentChar == '+') {
            if (*(currentChar + 1) == '+') {
                currentChar++;

                token.Type = data.Inc;
                token.Value = "++";
            } else {
                token.Type = data.Add;
                token.Value = "+";
            }
        } else if (*currentChar == '-') {
            if (*(currentChar + 1) == '-') {
                currentChar++;

                token.Type = data.Dec;
                token.Value = "--";
            } else {
                token.Type = data.Sub;
                token.Value = "-";
            }
        } else if (*currentChar == '*') {
            token.Type = data.Mul;
            token.Value = "*";
        } else if (*currentChar == '/') {
            token.Type = data.Div;
            token.Value = "/";
        } else if (*currentChar == '%') {
            token.Type = data.Mod;
            token.Value = "%";
        } else if (*currentChar == '(') {
            token.Type = data.ROUND_BRACKET_START;
            token.Value = "(";
        } else if (*currentChar == ')') {
            token.Type = data.ROUND_BRACKET_END;
            token.Value = ")";
        } else if (*currentChar == '[') {
            token.Type = data.SQUARE_BRACKET_START;
            token.Value = "[";
        } else if (*currentChar == ']') {
            token.Type = data.SQUARE_BRACKET_END;
            token.Value = "]";
        } else if (*currentChar == ';') {
            token.Type = data.SEMICOLON;
            token.Value = ";";
        }

        data.AddNewToken(token);
        currentChar++;
    }
    data.AddNewToken({data.eof, "EOF"});

    return data;
}

Token Lexer::tokenizeIdentifier() {
    Token token;

    std::string id_name;
    id_name += *currentChar;

    while ((*(currentChar + 1) >= 'a' && *(currentChar + 1) <= 'z') ||
           (*(currentChar + 1) >= 'A' && *(currentChar + 1) <= 'Z') ||
           (*(currentChar + 1) >= '0' && *(currentChar + 1) <= '9') || (*(currentChar + 1) == '_')) {
        currentChar++;
        id_name += *currentChar;
    }

    // объявление переменной
    if (id_name == "var") {
        token.Type = data.DeclareId;
        token.Value = "var";
    } else {
        token.Type = data.Id;
        token.Value = id_name;

        // если такого Identifier в таблице символов ещё нет, внести данные о нем
        if (!data.IsIdExist(id_name)) {
            data.AddNewIdentifier(id_name, Identifier{});
        }
    }

    return token;
}

Token Lexer::tokenizeNumber() {
    Token token;

    int token_val = *currentChar - '0';

    while (*(currentChar + 1) >= '0' && *(currentChar + 1) <= '9') {
        currentChar++;
        token_val = token_val * 10 + *currentChar - '0';
    }

    token.Type = data.Num;
    token.Value = std::to_string(token_val);

    return token;
}

void Lexer::initLexer(const std::string& src) {
    line = 1;
    currentChar = src.begin();
    data.ResetData();
}
