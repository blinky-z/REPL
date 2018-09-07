#include "SymbolTable.h"

bool SymbolTable::isIdExist(std::string identifierName) {
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

        // дублирущийся код
        throw std::runtime_error("Can not assign variable '" + idName + "' of type " +
                                 typeStringNames.idTypeStringNames[symbolTable[idName].Type] +
                                 " to the value of type Number");
    }
}

double SymbolTable::getIdValueDouble(std::string idName) {
    return symbolTable[idName].NumValue;
}

void SymbolTable::setIdValueBool(std::string idName, bool value) {
    if (symbolTable[idName].Type == IdentifierValueType::Undefined ||
        symbolTable[idName].Type == IdentifierValueType::Bool) {
        symbolTable[idName].Type = IdentifierValueType::Bool;
        symbolTable[idName].BoolValue = value;
    } else {
        IdentifierTypeStringNames typeStringNames;

        // TODO: убрать дублирующийся код ошибки (с setIdValueDouble func)
        throw std::runtime_error("Can not assign variable '" + idName + "' of type " +
                                 typeStringNames.idTypeStringNames[symbolTable[idName].Type] +
                                 " to the value of type Bool");
    }
}

bool SymbolTable::getIdValueBool(std::string idName) {
    return symbolTable[idName].BoolValue;
}

IdentifierValueType::ValueType SymbolTable::getIdType(const std::string& idName) {
    return symbolTable[idName].Type;
}