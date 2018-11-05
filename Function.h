#ifndef REPL_FUNCTION_H
#define REPL_FUNCTION_H

#include "ASTNode.h"

struct Function {
    ValueType::Type returnType;
    std::vector<std::string> args;
    unsigned long argsSize;
    BlockStmtNode* body;
};

#endif //REPL_FUNCTION_H
