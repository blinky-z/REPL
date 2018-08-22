#include "Parser.h"

int Parser::expression(int lvalue) {
    const Token& token = data.GetNextToken();
    int rvalue;

    if (token.type != data.Add && token.type != data.Sub) {
        data.ReturnToken();
        return lvalue;
    }

    rvalue = term(factor());
    if (token.type == data.Add) {
        return expression(lvalue + rvalue);
    } else {
        return expression(lvalue - rvalue);
    }
}

int Parser::term(int lvalue) {
    const Token& token = data.GetNextToken();
    int rvalue;

    if (token.type != data.Mul && token.type != data.Div) {
        data.ReturnToken();
        return lvalue;
    }

    rvalue = factor();

    if (token.type == data.Mul) {
        return term(lvalue * rvalue);
    } else {
        if (rvalue == 0) {
            std::cerr << "Div by zero" << std::endl;
            exit(EXIT_FAILURE);
        }
        return term(lvalue / rvalue);
    }
}

int Parser::factor() {
    const Token& token = data.GetNextToken();

    if (token.type == data.Num) {
        int num = std::stoi(token.value);
        return num;
    } else if (token.type == data.ROUND_BRACKET_START) {
        int result = expression(term(factor()));

        if (data.GetNextToken().type == data.ROUND_BRACKET_END) {
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