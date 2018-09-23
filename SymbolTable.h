#ifndef REPL_SYMBOLTABLE_H
#define REPL_SYMBOLTABLE_H

#include <unordered_map>
#include "ASTNode.h"
#include "Identifier.h"
#include "Function.h"

class SymbolTable {
private:
    std::unordered_map<std::string, Identifier> symbolTable;

    std::unordered_map<std::string, Function> funcSymbolTable;
public:
    bool isIdExist(const std::string& identifierName) const;

    void addNewIdentifier(const std::string& identifierName);

    void setIdValueDouble(const std::string& identifierName, double value);

    double getIdValueDouble(const std::string& identifierName) const;

    void setIdValueBool(const std::string& identifierName, bool value);

    bool getIdValueBool(const std::string& identifierName) const;

    ValueType::Type getIdValueType(const std::string& identifierName) const;

    void addNewFunc(DeclFuncNode* funcDecl);

    Function getFunc(const std::string funcName) const;

    bool isFuncExist(const std::string funcName);
};


#endif //REPL_SYMBOLTABLE_H
