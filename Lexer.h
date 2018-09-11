#ifndef BASHCOMPILER_LEXER_H
#define BASHCOMPILER_LEXER_H

#include "Token.h"
#include "Identifier.h"
#include "TokenContainer.h"
#include <vector>
#include <string>
#include <unordered_map>

class Lexer {
private:
    std::string::const_iterator currentChar;

    const Token tokenizeStringLiteral();

    const Token tokenizeNumber();

public:
    const TokenContainer tokenize(const std::string& src);
};

#endif //BASHCOMPILER_LEXER_H
