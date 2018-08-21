#include "Parser.h"

// Все формулы можно представить в виде T1 + T2 + ... + Tn
// Где Ti - это формула вида F1i * F2i * ... * Fmi
// Где Fji - это простейшая формула: единственное число или произвольная формула, заключенная в круглые скобки
// Таким образом, все операции, которые имеют приоритет выше, будут выполняться первыми
// И только тогда, когда формула T1 будет вычислена, далее идет операция сложения/вычитания

// Таким образом, все формулы можно разделить на 3 вида;
// 1) Простейшие формулы: числа и формулы, заключенные в круглые скобки | Например: 354, (17 + 3 - 18)
// 2) Формулы, содержащие операции умножения/деления | Например: 18 * 2 * (13 + 5) * 7
// 3) Формулы, содержащие операции сложения/вычитания | Например: 18 * 2 * (13 + 5) * 7 + 354 + (17 + 3 - 18) * (12 - 7)

// Данные формулы в коде:
// expression() - функция вычисления значения формулы, содержащей операции сложения/вычитания
// term() - функция вычисления значения формулы, содержащей операции умножения/деления
// factor() - функция вычисления числа или произвольной формулы, заключенной в скобки

// Тогда вся формула может быть посчитана так: expression(term(factor()))

// описание грамматики формул в BNF:
// <expr> ::= <expr> + <term>
//          | <expr> - <term>
//          | <term>
// данная конструкция описывает рекурсию: expr будет заменяться на одно из 2-х первых выражений, пока не останется
// только один множитель, и тогда выражение будет выглядеть так: <term> +/- <term +/- ... +/- <term>
//
// <term> ::= <term> * <factor>
//          | <term> / <factor>
//          | <factor>
// та же самая ситуация, что и с expr: <term> будет заменяться, пока не останется всего один имножитель или делитель,
// или же <expr> в скобках, которое будет посчитано по аналогии

// <factor> ::= ( <expr> ) - произвольная формула (expr)
//          | Num            или число

// Terminator и Non-Terminator в BNF
// Non-Terminator - это выражение, заключенное в <>. Они могут заменяться на альтернативное выражение с правой стороны
// от ::=
// Terminator - не могут заменяться на альтернативные выражения. Обычно это токены, полученные от лексера

// При парсинге будет использоваться Top-Down метод парсинга

// Top-down parsing - это значит, что парсинг происходит сверху до дна
// это можно видеть на примере ниже:
// кажде выражение заменялось на альтернативное, продвигаясь каждый раз все ниже, пока не встретится terminator
// Поэтому top-down parsing хорошо сочетаются с BNF грамматикой

// При реализации придется использовать рекурсию с бэктрекингом, ведь, дойдя до терминатора, нам надо будет вернуться
// назад с вычисленным значением

// Пример использования BNF и top-down parsing
// Пусть есть выражение: 3 * (4 + 2)
// тогда parse tree будет выглядеть так:
// <expr> => <expr>
//          => <term> *     <factor>
//             => <factor>   |
//              => Num (3)   |
//                           => (<expr>)
//                              => <expr>       + <term>
//                                => <term>         |
//                                  => factor       |
//                                    => Num (4)    |
//                                                  => <factor>
//                                                    => Num (2)

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
