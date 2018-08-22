#ifndef BASHCOMPILER_LEXER_H
#define BASHCOMPILER_LEXER_H

#include "Token.h"
#include "Identifier.h"
#include "AnalysisContainer.h"
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class Lexer {
private:
    int line = 1;

    AnalysisContainer data;

    Token tokenizeIdentifier(char** src);

    Token tokenizeNumber(char** src);
public:
    AnalysisContainer tokenize(char* src);
};

#endif //BASHCOMPILER_LEXER_H
