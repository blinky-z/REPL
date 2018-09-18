#ifndef REPL_TOKENCONTAINER_H
#define REPL_TOKENCONTAINER_H

#include <vector>
#include "Token.h"

class TokenContainer {
private:
    std::vector<Token> tokens;

    unsigned long currentTokenNum = 0;
public:
    const std::vector<Token>& getTokens() const;

    Token getNextToken();

    Token lookNextToken();

    void returnToken();

    void addNewToken(const Token& token);

    unsigned long size() {
        return tokens.size();
    }
};


#endif //REPL_TOKENCONTAINER_H
