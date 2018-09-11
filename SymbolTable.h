#ifndef REPL_SYMBOLTABLE_H
#define REPL_SYMBOLTABLE_H

#include <unordered_map>
#include "Identifier.h"

class SymbolTable {
private:
    std::unordered_map<std::string, Identifier> symbolTable;
public:
    bool isIdExist(const std::string& identifierName) const;

    void addNewIdentifier(const std::string& identifierName);

    void setIdValueDouble(const std::string& identifierName, double value);

    double getIdValueDouble(const std::string& identifierName) const;

    void setIdValueBool(const std::string& identifierName, bool value);

    bool getIdValueBool(const std::string& identifierName) const;

    IdentifierValueType::ValueType getIdType(const std::string& identifierName) const;
};


#endif //REPL_SYMBOLTABLE_H
