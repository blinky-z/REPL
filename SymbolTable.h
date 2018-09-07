#ifndef REPL_SYMBOLTABLE_H
#define REPL_SYMBOLTABLE_H

#include <unordered_map>
#include "Identifier.h"

class SymbolTable {
private:
    std::unordered_map<std::string, Identifier> symbolTable;
public:
    bool isIdExist(std::string identifierName);

    void addNewIdentifier(std::string identifierName);

    void setIdValueDouble(std::string idName, double value);

    double getIdValueDouble(std::string idName);

    void setIdValueBool(std::string idName, bool value);

    bool getIdValueBool(std::string idName);

    IdentifierValueType::ValueType getIdType(const std::string& idName);
};


#endif //REPL_SYMBOLTABLE_H
