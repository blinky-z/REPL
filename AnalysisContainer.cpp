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
    return symbolTable.find(identifierName) != symbolTable.end();
}

void AnalysisContainer::AddNewIdentifier(std::string identifierName, const Identifier& identifier) {
    symbolTable.emplace(identifierName, identifier);
}

void AnalysisContainer::AddNewToken(Token token) {
    tokens.emplace_back(token);
}

const std::vector<Token>& AnalysisContainer::GetTokens() {
    return tokens;
}

void AnalysisContainer::ResetData() {
    tokens.clear();
    currentTokenNum = 0;
    symbolTable.clear();
}
