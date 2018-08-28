#ifndef REPL_ASTNODE_H
#define REPL_ASTNODE_H

#include <string>

enum ASTNodeType {
    Undefined,
    Empty,
    Id,
    NumberValue,
    OperatorPlus,
    OperatorMinus,
    OperatorMul,
    OperatorDiv,
};

struct ASTNode {
    ASTNodeType type;
};

struct BinOpNode : ASTNode {
    ASTNode* left;
    ASTNode* right;

    BinOpNode() {
        left = nullptr;
        right = nullptr;
    }

    ~BinOpNode() {
        delete left;
        delete right;
    }
};

struct NumberNode : ASTNode {
    double value;

    NumberNode() {
        type = NumberValue;
    }
};

struct IdentifierNode : ASTNode {
    std::string name;

    IdentifierNode() {
        type = Id;
    }
};

#endif //REPL_ASTNODE_H
