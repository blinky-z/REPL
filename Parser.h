#ifndef BASHCOMPILER_PARSER_H
#define BASHCOMPILER_PARSER_H

#include "Token.h"
#include "Identifier.h"
#include "AnalysisContainer.h"
#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <unordered_map>

class Parser {
private:
    int expression(int lvalue);

    int term(int lvalue);

    int factor();

    int parseMath();

    AnalysisContainer data;

public:
    Parser(AnalysisContainer data) : data(std::move(data)) {};

    int parse();
};


#endif //BASHCOMPILER_PARSER_H
