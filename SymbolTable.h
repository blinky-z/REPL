#ifndef REPL_SYMBOLTABLE_H
#define REPL_SYMBOLTABLE_H

#include <unordered_map>
#include "ASTNode.h"
#include "Identifier.h"

class SymbolTable {
private:
    std::unordered_map<std::string, Identifier> symbolTable;

    std::unordered_map<std::string, DeclFuncNode*> funcSymbolTable;
public:
    bool isIdExist(const std::string& identifierName) const;

    void addNewIdentifier(const std::string& name);

    void addNewIdentifier(const std::string& name, bool value);

    void addNewIdentifier(const std::string& name, double value);

    void setIdValueDouble(const std::string& identifierName, double value);

    void setIdValueBool(const std::string& identifierName, bool value);

    double getIdValueDouble(const std::string& identifierName) const;

    bool getIdValueBool(const std::string& identifierName) const;

    ValueType::Type getIdValueType(const std::string& identifierName) const;

    bool isFuncExist(const std::string& funcName);

    void addNewFunc(DeclFuncNode* funcDecl);

    DeclFuncNode* getFunc(const std::string& funcName) const;

    ValueType::Type getFuncValueType(const std::string& funcName) const;
};


#endif //REPL_SYMBOLTABLE_H
