#include "Parser.h"

int Parser::expression(int lvalue) {
    const Token& token = getNextToken();
    int rvalue;

    if (token.type != Add && token.type != Sub) {
        returnToken();
        return lvalue;
    }

    rvalue = term(factor());
    if (token.type == Add) {
        return expression(lvalue + rvalue);
    } else {
        return expression(lvalue - rvalue);
    }
}

int Parser::term(int lvalue) {
    const Token& token = getNextToken();
    int rvalue;

    if (token.type != Mul && token.type != Div) {
        returnToken();
        return lvalue;
    }

    rvalue = factor();

    if (token.type == Mul) {
        return term(lvalue * rvalue);
    } else if (token.type == Div) {
        if (rvalue == 0) {
            std::cerr << "Div by zero" << std::endl;
            exit(EXIT_FAILURE);
        }
        return term(lvalue / rvalue);
    }
}

int Parser::factor() {
    const Token& token = getNextToken();

    if (token.type == Num) {
        int num = std::stoi(token.value);
        return num;
    } else if (token.type == ROUND_BRACKET_START) {
        int result = expression(term(factor()));

        if (getNextToken().type == ROUND_BRACKET_END) {
            return result;
        } else {
            std::cerr << "Invalid syntax" << std::endl;
            exit(EXIT_FAILURE);
        }
    } else {
        std::cerr << "Invalid syntax" << std::endl;
        exit(EXIT_FAILURE);
    }
}

int Parser::parseMath() {
    return expression(term(factor()));
}

int Parser::parse() {
    // нужно будет определять по контексту какая операция
    // пока будут только математические формулы

    return parseMath();
}

Token Parser::getNextToken() {
    auto token = *currentToken;
    currentToken++;
    return token;
}

void Parser::returnToken() {
    currentToken--;
}
