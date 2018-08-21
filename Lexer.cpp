#include "Lexer.h"

void Lexer::tokenize(char* src, vector<Token>& tokens, unordered_map<string, Identifier>& symbol_table) {
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
            string id_name;
            id_name += *src;

            while ((*(src + 1) >= 'a' && *(src + 1) <= 'z') || (*(src + 1) >= 'A' && *(src + 1) <= 'Z') ||
                   (*(src + 1) >= '0' && *(src + 1) <= '9') || (*(src + 1) == '_')) {
                src++;
                id_name += *src;
            }

            // объявление переменной
            if (id_name == "var") {
                token.type = DeclareId;
                token.value = "var";
            } else {
                token.type = Id;
                token.value = id_name;

                // если такого Identifier в таблице символов ещё нет, внести данные о нем
                if (symbol_table.find(id_name) == symbol_table.end()) {
                    symbol_table.emplace(id_name, Identifier{});
                }
            }
        } else if (*src >= '0' && *src <= '9') {
            int token_val = *src - '0';

            while (*(src + 1) >= '0' && *(src + 1) <= '9') {
                src++;
                token_val = token_val * 10 + *src - '0';
            }

            token.type = Num;
            token.value = to_string(token_val);
        } else if (*src == '=') {
            token.type = Assign;
            token.value = "=";
        } else if (*src == '+') {
            if (*(src + 1) == '+') {
                src++;

                token.type = Inc;
                token.value = "++";
            } else {
                token.type = Add;
                token.value = "+";
            }
        } else if (*src == '-') {
            if (*(src + 1) == '-') {
                src++;

                token.type = Dec;
                token.value = "--";
            } else {
                token.type = Sub;
                token.value = "-";
            }
        } else if (*src == '*') {
            token.type = Mul;
            token.value = "*";
        } else if (*src == '/') {
            token.type = Div;
            token.value = "/";
        } else if (*src == '%') {
            token.type = Mod;
            token.value = "%";
        } else if (*src == '(') {
            token.type = ROUND_BRACKET_START;
            token.value = "(";
        } else if (*src == ')') {
            token.type = ROUND_BRACKET_END;
            token.value = ")";
        } else if (*src == '[') {
            token.type = SQUARE_BRACKET_START;
            token.value = "[";
        } else if (*src == ']') {
            token.type = SQUARE_BRACKET_END;
            token.value = "]";
        } else if (*src == ';') {
            token.type = SEMICOLON;
            token.value = ";";
        }

        tokens.emplace_back(token);
        src++;
    }

    tokens.emplace_back(Token{eof, "EOF"});
}
