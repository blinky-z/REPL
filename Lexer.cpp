#include <iostream>
#include "Lexer.h"

const TokenContainer Lexer::tokenize(const std::string& src) {
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

        if (isalpha(*currentChar) || (*currentChar == '_')) {
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
            } else if (tokens.size() == 0) {
                token.Type = TokenType::UnaryMinus;
                token.Value = "u-";
            } else {
                const Token& lastOp = tokens.getTokens()[tokens.size() - 1];

                if (lastOp.Type == TokenType::Assign || lastOp.Type == TokenType::Add ||
                    lastOp.Type == TokenType::Sub || lastOp.Type == TokenType::Mul ||
                    lastOp.Type == TokenType::Div || lastOp.Type == TokenType::Equal ||
                    lastOp.Type == TokenType::ROUND_BRACKET_START) {
                    token.Type = TokenType::UnaryMinus;
                    token.Value = "u-";
                } else {
                    token.Type = TokenType::Sub;
                    token.Value = "-";
                }
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

const Token Lexer::tokenizeStringLiteral() {
    Token token;

    std::string strLiteral;
    strLiteral += *currentChar;

    while ((*(currentChar + 1) >= 'a' && *(currentChar + 1) <= 'z') ||
           (*(currentChar + 1) >= 'A' && *(currentChar + 1) <= 'Z') ||
           (*(currentChar + 1) >= '0' && *(currentChar + 1) <= '9') || (*(currentChar + 1) == '_')) {
        currentChar++;
        strLiteral += *currentChar;
    }

    if (strLiteral == "var") {
        token.Type = TokenType::DeclareId;
        token.Value = "var";
    } else if (strLiteral == "false" || strLiteral == "true") {
        token.Type = TokenType::Bool;

        if (strLiteral == "false") {
            token.Value = "0";
        } else {
            token.Value = "1";
        }
    } else {
        token.Type = TokenType::Id;
        token.Value = strLiteral;
    }

    return token;
}

const Token Lexer::tokenizeNumber() {
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