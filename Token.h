#ifndef BASHCOMPILER_TOKEN_H
#define BASHCOMPILER_TOKEN_H

#include <string>

struct Token {
    int Type;
    std::string Value;
};

#endif //BASHCOMPILER_TOKEN_H
