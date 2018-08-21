#ifndef BASHCOMPILER_IDENTIFIER_H
#define BASHCOMPILER_IDENTIFIER_H

#include <string>

struct Identifier {
    int type; // тип контента (string, int в bash)
    std::string valueString; // в bash есть только int и string
    int valueInt;
};

#endif //BASHCOMPILER_IDENTIFIER_H
