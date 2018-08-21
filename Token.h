#ifndef BASHCOMPILER_TOKEN_H
#define BASHCOMPILER_TOKEN_H

#include <string>

struct Token {
    int type;
    std::string value;
};

#endif //BASHCOMPILER_TOKEN_H
