#ifndef BASHCOMPILER_IDENTIFIER_H
#define BASHCOMPILER_IDENTIFIER_H

#include <string>
#include <unordered_map>

namespace IdentifierValueType {
    enum ValueType {
        Number,
        Bool,
        Undefined
    };
}

struct IdentifierTypeStringNames {
    std::unordered_map<int, std::string> idTypeStringNames;

    IdentifierTypeStringNames() {
        idTypeStringNames[IdentifierValueType::Number] = "Number";
        idTypeStringNames[IdentifierValueType::Bool] = "Bool";
        idTypeStringNames[IdentifierValueType::Undefined] = "Undefined";
    }
};

struct Identifier {
    IdentifierValueType::ValueType Type; // тип контента (string, int в bash)
    std::string StringValue; // в bash есть только int и string
    double NumValue;
    bool BoolValue;

    Identifier() {
        Type = IdentifierValueType::Undefined;
    }
};

#endif //BASHCOMPILER_IDENTIFIER_H
