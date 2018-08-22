#include <iostream>
#include "Lexer.h"

AnalysisContainer Lexer::tokenize(const string& src) {
    currentChar = src.begin();

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
            token.type = data.Assign;
            token.value = "=";
        } else if (*currentChar == '+') {
            if (*(currentChar + 1) == '+') {
                currentChar++;

                token.type = data.Inc;
                token.value = "++";
            } else {
                token.type = data.Add;
                token.value = "+";
            }
        } else if (*currentChar == '-') {
            if (*(currentChar + 1) == '-') {
                currentChar++;

                token.type = data.Dec;
                token.value = "--";
            } else {
                token.type = data.Sub;
                token.value = "-";
            }
        } else if (*currentChar == '*') {
            token.type = data.Mul;
            token.value = "*";
        } else if (*currentChar == '/') {
            token.type = data.Div;
            token.value = "/";
        } else if (*currentChar == '%') {
            token.type = data.Mod;
            token.value = "%";
        } else if (*currentChar == '(') {
            token.type = data.ROUND_BRACKET_START;
            token.value = "(";
        } else if (*currentChar == ')') {
            token.type = data.ROUND_BRACKET_END;
            token.value = ")";
        } else if (*currentChar == '[') {
            token.type = data.SQUARE_BRACKET_START;
            token.value = "[";
        } else if (*currentChar == ']') {
            token.type = data.SQUARE_BRACKET_END;
            token.value = "]";
        } else if (*currentChar == ';') {
            token.type = data.SEMICOLON;
            token.value = ";";
        }

        data.AddNewToken(token);
        currentChar++;
    }

    data.AddNewToken({data.eof, "EOF"});

    return data;
}

Token Lexer::tokenizeIdentifier() {
    Token token;

    string id_name;
    id_name += *currentChar;

    while ((*(currentChar + 1) >= 'a' && *(currentChar + 1) <= 'z') ||
           (*(currentChar + 1) >= 'A' && *(currentChar + 1) <= 'Z') ||
           (*(currentChar + 1) >= '0' && *(currentChar + 1) <= '9') || (*(currentChar + 1) == '_')) {
        currentChar++;
        id_name += *currentChar;
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

Token Lexer::tokenizeNumber() {
    Token token;

    int token_val = *currentChar - '0';

    while (*(currentChar + 1) >= '0' && *(currentChar + 1) <= '9') {
        currentChar++;
        token_val = token_val * 10 + *currentChar - '0';
    }

    token.type = data.Num;
    token.value = to_string(token_val);

    return token;
}
