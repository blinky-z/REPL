#ifndef REPL_FUNCTION_H
#define REPL_FUNCTION_H

#include "ASTNode.h"
#include <map>

struct Function {
    ValueType::Type returnType;
    std::vector<std::pair<std::string, Identifier>> args;
    unsigned long argsSize;
    BlockStmtNode* body;
};

#endif //REPL_FUNCTION_H
