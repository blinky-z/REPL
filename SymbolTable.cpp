#include "SymbolTable.h"

bool SymbolTable::isIdExist(std::string identifierName) {
    return symbolTable.find(identifierName) != symbolTable.end();
}

void SymbolTable::addNewIdentifier(std::string identifierName) {
    symbolTable.emplace(identifierName, Identifier{});
}

void SymbolTable::setIdValueDouble(std::string idName, double value) {
    symbolTable[idName].NumValue = value;
}

double SymbolTable::getIdValueDouble(std::string idName) {
    return symbolTable[idName].NumValue;
}