#ifndef BASHCOMPILER_IDENTIFIER_H
#define BASHCOMPILER_IDENTIFIER_H

#include <string>

struct Identifier {
    int Type; // тип контента (string, int в bash)
    std::string StringValue; // в bash есть только int и string
    double NumValue;
};

#endif //BASHCOMPILER_IDENTIFIER_H
