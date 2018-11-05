#include "SymbolTable.h"
#include "ASTNode.h"

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

void SymbolTable::addNewFunc(DeclFuncNode* funcDecl) {
    Function function;
    function.returnType = funcDecl->returnType;
    function.argsSize = funcDecl->argsSize;

    for (const auto& currentId : funcDecl->args) {
        function.args.emplace_back(currentId->name);
    }
    function.body = funcDecl->body; // TODO: сделать копирование тела

    funcSymbolTable.emplace(funcDecl->name, function);
}

Function SymbolTable::getFunc(const std::string funcName) const {
    return funcSymbolTable.at(funcName);
}

bool SymbolTable::isFuncExist(const std::string funcName) {
    return funcSymbolTable.find(funcName) != funcSymbolTable.end();
}