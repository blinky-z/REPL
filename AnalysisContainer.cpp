#include "AnalysisContainer.h"

Token AnalysisContainer::GetNextToken() {
    auto token = tokens[currentTokenNum];
    currentTokenNum++;
    return token;
}

void AnalysisContainer::ReturnToken() {
    currentTokenNum--;
}

bool AnalysisContainer::IsIdExist(std::string identifierName) {
    return symbol_table.find(identifierName) != symbol_table.end();
}

void AnalysisContainer::AddNewIdentifier(std::string identifierName, const Identifier& identifier) {
    symbol_table.emplace(identifierName, identifier);
}

void AnalysisContainer::AddNewToken(Token token) {
    tokens.emplace_back(token);
}

const std::vector<Token>& AnalysisContainer::GetTokens() {
    return tokens;
}
