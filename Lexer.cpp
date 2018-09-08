#include <iostream>
#include "Lexer.h"

TokenContainer Lexer::tokenize(const std::string& src) {
    currentChar = src.begin();

    TokenContainer tokens;

    while (true) {
        Token token;

        while (*currentChar == ' ') {
            currentChar++;
        }
        while (*currentChar == '\n') {
            currentChar++;
        }

        if ((*currentChar >= 'a' && *currentChar <= 'z') || (*currentChar >= 'A' && *currentChar <= 'Z') ||
            (*currentChar == '_')) {
            token = tokenizeStringLiteral();
        } else if (*currentChar >= '0' && *currentChar <= '9') {
            token = tokenizeNumber();
        } else if (*currentChar == '=') {
            if (*(currentChar + 1) == '=') {
                currentChar++;

                token.Type = TokenType::Equal;
                token.Value = "==";
            } else {
                token.Type = TokenType::Assign;
                token.Value = "=";
            }
        } else if (*currentChar == '+') {
            if (*(currentChar + 1) == '+') {
                currentChar++;

                token.Type = TokenType::Inc;
                token.Value = "++";
            } else {
                token.Type = TokenType::Add;
                token.Value = "+";
            }
        } else if (*currentChar == '-') {
            if (*(currentChar + 1) == '-') {
                currentChar++;

                token.Type = TokenType::Dec;
                token.Value = "--";
            } else {
                token.Type = TokenType::Sub;
                token.Value = "-";
            }
        } else if (*currentChar == '*') {
            token.Type = TokenType::Mul;
            token.Value = "*";
        } else if (*currentChar == '/') {
            token.Type = TokenType::Div;
            token.Value = "/";
        } else if (*currentChar == '%') {
            token.Type = TokenType::Mod;
            token.Value = "%";
        } else if (*currentChar == '&' && *(currentChar + 1) == '&') {
            token.Type = TokenType::BoolAND;
            token.Value = "&&";

            currentChar++;
        } else if (*currentChar == '|' && *(currentChar + 1) == '|') {
            token.Type = TokenType::BoolOR;
            token.Value = "||";

            currentChar++;
        } else if (*currentChar == '(') {
            token.Type = TokenType::ROUND_BRACKET_START;
            token.Value = "(";
        } else if (*currentChar == ')') {
            token.Type = TokenType::ROUND_BRACKET_END;
            token.Value = ")";
        } else if (*currentChar == '[') {
            token.Type = TokenType::SQUARE_BRACKET_START;
            token.Value = "[";
        } else if (*currentChar == ']') {
            token.Type = TokenType::SQUARE_BRACKET_END;
            token.Value = "]";
        } else if (*currentChar == ';') {
            token.Type = TokenType::SEMICOLON;
            token.Value = ";";
        } else if (*currentChar == '<') {
            token.Type = TokenType::LESS;
            token.Value = "<";
        } else if (*currentChar == '>') {
            token.Type = TokenType::GREATER;
            token.Value = ">";
        } else if (*currentChar == EOF) {
            tokens.addNewToken(Token{TokenType::eof, "EOF"});
            break;
        } else {
            throw std::runtime_error(std::string("Invalid char ") + "'" + *currentChar + "'");
        }

        tokens.addNewToken(token);
        currentChar++;
    }

    return tokens;
}

Token Lexer::tokenizeStringLiteral() {
    Token token;

    std::string id_name;
    id_name += *currentChar;

    while ((*(currentChar + 1) >= 'a' && *(currentChar + 1) <= 'z') ||
           (*(currentChar + 1) >= 'A' && *(currentChar + 1) <= 'Z') ||
           (*(currentChar + 1) >= '0' && *(currentChar + 1) <= '9') || (*(currentChar + 1) == '_')) {
        currentChar++;
        id_name += *currentChar;
    }

    if (id_name == "var") {
        token.Type = TokenType::DeclareId;
        token.Value = "var";
    } else if (id_name == "false" || id_name == "true") {
        token.Type = TokenType::Bool;

        if (id_name == "false") {
            token.Value = "0";
        } else {
            token.Value = "1";
        }
    } else {
        token.Type = TokenType::Id;
        token.Value = id_name;
    }

    return token;
}

Token Lexer::tokenizeNumber() {
    Token token;

    std::string token_val;
    token_val += *currentChar;

    while ((*(currentChar + 1) >= '0' && *(currentChar + 1) <= '9') || *(currentChar + 1) == '.') {
        currentChar++;
        token_val += *currentChar;
    }

    token.Type = TokenType::Num;
    token.Value = token_val;

    return token;
}