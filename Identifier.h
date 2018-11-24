#ifndef BASHCOMPILER_IDENTIFIER_H
#define BASHCOMPILER_IDENTIFIER_H

#include <string>
#include <unordered_map>

namespace ValueType {
    enum Type {
        Number,
        Bool,
        String,
        Compound,
        Void,
        Undefined
    };
}

struct IdentifierTypeStringNames {
    std::unordered_map<int, std::string> idTypeStringNames;

    IdentifierTypeStringNames() {
        idTypeStringNames[ValueType::Number] = "Number";
        idTypeStringNames[ValueType::Bool] = "Bool";
        idTypeStringNames[ValueType::String] = "String";
        idTypeStringNames[ValueType::Compound] = "Compound Statement";
        idTypeStringNames[ValueType::Undefined] = "Undefined";
    }
};

struct Identifier {
    ValueType::Type Type;
    double numValue;
    bool boolValue;

    Identifier() {
        Type = ValueType::Undefined;
    }
};

#endif //BASHCOMPILER_IDENTIFIER_H
