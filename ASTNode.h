#ifndef REPL_ASTNODE_H
#define REPL_ASTNODE_H

#include <string>

enum ASTNodeType {
    Undefined,
    DeclVar,
    Empty,
    Id,
    BinOp,
    NumberValue,
};

enum ASTNodeBinOpType {
    OperatorAssign,
    OperatorPlus,
    OperatorMinus,
    OperatorMul,
    OperatorDiv,
};

struct ASTNode {
    ASTNodeType type;

    ASTNode() {
        type = Undefined;
    }
};

struct BinOpNode : ASTNode {
    ASTNodeBinOpType binOpType;
    ASTNode* left;
    ASTNode* right;

    BinOpNode() {
        type = BinOp;
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

struct DeclVarNode : ASTNode {
    IdentifierNode* id;
    ASTNode* expr;

    DeclVarNode() {
        type = DeclVar;
        expr = nullptr;
    }

    ~DeclVarNode() {
        delete expr;
    }

};

#endif //REPL_ASTNODE_H
