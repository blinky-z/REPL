#ifndef BASHCOMPILER_LEXER_H
#define BASHCOMPILER_LEXER_H

#include "Token.h"
#include "Identifier.h"
#include "AnalysisContainer.h"
#include <vector>
#include <string>
#include <unordered_map>

class Lexer {
private:
    int line;

    std::string::const_iterator currentChar;

    AnalysisContainer data;

    void initLexer(const std::string& src);

    Token tokenizeIdentifier();

    Token tokenizeNumber();

public:
    AnalysisContainer tokenize(const std::string& src);
};

#endif //BASHCOMPILER_LEXER_H
