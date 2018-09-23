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
    double NumValue;
    bool BoolValue;

    Identifier() {
        Type = ValueType::Undefined;
    }
};

#endif //BASHCOMPILER_IDENTIFIER_H
