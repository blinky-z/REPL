#include "SymbolTable.h"

bool SymbolTable::isIdExist(std::string identifierName) const {
    return symbolTable.find(identifierName) != symbolTable.end();
}

void SymbolTable::addNewIdentifier(std::string identifierName) {
    symbolTable.emplace(identifierName, Identifier{});
}

void SymbolTable::setIdValueDouble(std::string idName, double value) {
    if (symbolTable[idName].Type == IdentifierValueType::Undefined ||
        symbolTable[idName].Type == IdentifierValueType::Number) {
        symbolTable[idName].Type = IdentifierValueType::Number;
        symbolTable[idName].NumValue = value;
    } else {
        IdentifierTypeStringNames typeStringNames;
        throw std::runtime_error("Can not assign integer value to the variable '" + idName + "' of type " +
                                 typeStringNames.idTypeStringNames[symbolTable[idName].Type]);
    }
}

double SymbolTable::getIdValueDouble(std::string idName) const {
    return symbolTable.at(idName).NumValue;
}

void SymbolTable::setIdValueBool(std::string idName, bool value) {
    if (symbolTable[idName].Type == IdentifierValueType::Undefined ||
        symbolTable[idName].Type == IdentifierValueType::Bool) {
        symbolTable[idName].Type = IdentifierValueType::Bool;
        symbolTable[idName].BoolValue = value;
    } else {
        IdentifierTypeStringNames typeStringNames;
        throw std::runtime_error("Can not assign bool value to the variable '" + idName + "' of type " +
                                 typeStringNames.idTypeStringNames[symbolTable[idName].Type]);
    }
}

bool SymbolTable::getIdValueBool(std::string idName) const {
    return symbolTable.at(idName).BoolValue;
}

IdentifierValueType::ValueType SymbolTable::getIdType(const std::string& idName) const {
    return symbolTable.at(idName).Type;
}