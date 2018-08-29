#ifndef BASHCOMPILER_LEXER_H
#define BASHCOMPILER_LEXER_H

#include "Token.h"
#include "Identifier.h"
#include "TokenContainer.h"
#include "TokenTypes.h"
#include <vector>
#include <string>
#include <unordered_map>

class Lexer {
private:
    int line;

    std::string::const_iterator currentChar;

    Token tokenizeIdentifier();

    Token tokenizeNumber();

public:
    TokenContainer tokenize(const std::string& src);
};

#endif //BASHCOMPILER_LEXER_H
