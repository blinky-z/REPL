#include "SymbolTable.h"
#include "ASTNode.h"

bool SymbolTable::isIdExist(const std::string& identifierName) const {
    return symbolTable.find(identifierName) != symbolTable.end();
}

void SymbolTable::addNewIdentifier(const std::string& name) {
    symbolTable.emplace(name, Identifier{});
}

void SymbolTable::addNewIdentifier(const std::string& name, bool value) {
    Identifier id;
    id.Type = ValueType::Bool;
    id.boolValue = value;

    symbolTable.emplace(name, id);
}

void SymbolTable::addNewIdentifier(const std::string& name, double value) {
    Identifier id;
    id.Type = ValueType::Number;
    id.numValue = value;

    symbolTable.emplace(name, id);
}

void SymbolTable::setIdValueDouble(const std::string& identifierName, double value) {
    symbolTable[identifierName].Type = ValueType::Number;
    symbolTable[identifierName].numValue = value;
}

void SymbolTable::setIdValueBool(const std::string& identifierName, bool value) {
    symbolTable[identifierName].Type = ValueType::Bool;
    symbolTable[identifierName].boolValue = value;
}

double SymbolTable::getIdValueDouble(const std::string& identifierName) const {
    return symbolTable.at(identifierName).numValue;
}

bool SymbolTable::getIdValueBool(const std::string& identifierName) const {
    return symbolTable.at(identifierName).boolValue;
}

ValueType::Type SymbolTable::getIdValueType(const std::string& identifierName) const {
    return symbolTable.at(identifierName).Type;
}

bool SymbolTable::isFuncExist(const std::string& funcName) {
    return funcSymbolTable.find(funcName) != funcSymbolTable.end();
}

void SymbolTable::addNewFunc(DeclFuncNode* funcDecl) {
    Function function;
    function.returnType = funcDecl->returnType;
    function.argsSize = funcDecl->argsSize;

    for (const auto& currentId : funcDecl->args) {
        Identifier id;
        id.Type = currentId->valueType;
        function.args.emplace_back(std::make_pair(currentId->name, id));
    }
    function.body = funcDecl->body; // TODO: сделать копирование контента тела, а не копирование указателей

    funcSymbolTable.emplace(funcDecl->name, function);
}

Function SymbolTable::getFunc(const std::string& funcName) const {
    return funcSymbolTable.at(funcName);
}

ValueType::Type SymbolTable::getFuncValueType(const std::string& funcName) const {
    return funcSymbolTable.at(funcName).returnType;
}