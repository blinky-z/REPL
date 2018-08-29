#include <iostream>
#include "Lexer.h"

TokenContainer Lexer::tokenize(const std::string& src) {
    line = 1;
    currentChar = src.begin();

    TokenContainer tokens;

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
            token.Type = TokenTypes::Assign;
            token.Value = "=";
        } else if (*currentChar == '+') {
            if (*(currentChar + 1) == '+') {
                currentChar++;

                token.Type = TokenTypes::Inc;
                token.Value = "++";
            } else {
                token.Type = TokenTypes::Add;
                token.Value = "+";
            }
        } else if (*currentChar == '-') {
            if (*(currentChar + 1) == '-') {
                currentChar++;

                token.Type = TokenTypes::Dec;
                token.Value = "--";
            } else {
                token.Type = TokenTypes::Sub;
                token.Value = "-";
            }
        } else if (*currentChar == '*') {
            token.Type = TokenTypes::Mul;
            token.Value = "*";
        } else if (*currentChar == '/') {
            token.Type = TokenTypes::Div;
            token.Value = "/";
        } else if (*currentChar == '%') {
            token.Type = TokenTypes::Mod;
            token.Value = "%";
        } else if (*currentChar == '(') {
            token.Type = TokenTypes::ROUND_BRACKET_START;
            token.Value = "(";
        } else if (*currentChar == ')') {
            token.Type = TokenTypes::ROUND_BRACKET_END;
            token.Value = ")";
        } else if (*currentChar == '[') {
            token.Type = TokenTypes::SQUARE_BRACKET_START;
            token.Value = "[";
        } else if (*currentChar == ']') {
            token.Type = TokenTypes::SQUARE_BRACKET_END;
            token.Value = "]";
        } else if (*currentChar == ';') {
            token.Type = TokenTypes::SEMICOLON;
            token.Value = ";";
        }

        tokens.addNewToken(token);
        currentChar++;
    }
    tokens.addNewToken(Token{TokenTypes::eof, "EOF"});

    return tokens;
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
        token.Type = TokenTypes::DeclareId;
        token.Value = "var";
    } else {
        token.Type = TokenTypes::Id;
        token.Value = id_name;
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

    token.Type = TokenTypes::Num;
    token.Value = std::to_string(token_val);

    return token;
}