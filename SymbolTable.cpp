#include "SymbolTable.h"

bool SymbolTable::isIdExist(const std::string& identifierName) const {
    return symbolTable.find(identifierName) != symbolTable.end();
}

void SymbolTable::addNewIdentifier(const std::string& identifierName) {
    symbolTable.emplace(identifierName, Identifier{});
}

void SymbolTable::setIdValueDouble(const std::string& identifierName, double value) {
    if (symbolTable[identifierName].Type == ValueType::Undefined ||
        symbolTable[identifierName].Type == ValueType::Number) {
        symbolTable[identifierName].Type = ValueType::Number;
        symbolTable[identifierName].NumValue = value;
    } else {
        IdentifierTypeStringNames typeStringNames;
        throw std::runtime_error("Can not assign integer value to the variable '" + identifierName + "' of type " +
                                 typeStringNames.idTypeStringNames[symbolTable[identifierName].Type]);
    }
}

double SymbolTable::getIdValueDouble(const std::string& identifierName) const {
    return symbolTable.at(identifierName).NumValue;
}

void SymbolTable::setIdValueBool(const std::string& identifierName, bool value) {
    if (symbolTable[identifierName].Type == ValueType::Undefined ||
        symbolTable[identifierName].Type == ValueType::Bool) {
        symbolTable[identifierName].Type = ValueType::Bool;
        symbolTable[identifierName].BoolValue = value;
    } else {
        IdentifierTypeStringNames typeStringNames;
        throw std::runtime_error("Can not assign bool value to the variable '" + identifierName + "' of type " +
                                 typeStringNames.idTypeStringNames[symbolTable[identifierName].Type]);
    }
}

bool SymbolTable::getIdValueBool(const std::string& identifierName) const {
    return symbolTable.at(identifierName).BoolValue;
}

ValueType::Type SymbolTable::getIdValueType(const std::string& identifierName) const {
    return symbolTable.at(identifierName).Type;
}