#include "TokenContainer.h"

const std::vector<Token>& TokenContainer::getTokens() const {
    return tokens;
}

Token TokenContainer::getNextToken() {
    auto token = tokens[currentTokenNum];
    currentTokenNum++;
    return token;
}

void TokenContainer::returnToken() {
    currentTokenNum--;
}

void TokenContainer::addNewToken(const Token& token) {
    tokens.emplace_back(token);
}

Token TokenContainer::lookNextToken() {
    return tokens[currentTokenNum];
}
