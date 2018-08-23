#ifndef REPL_ASTNODE_H
#define REPL_ASTNODE_H

enum ASTNodeType {
    Undefined,
    OperatorPlus,
    OperatorMinus,
    OperatorMul,
    OperatorDiv,
    UnaryMinus,
    NumberValue
};

struct ASTNode {
    ASTNodeType Type;
    int Value;
    ASTNode* Left;
    ASTNode* Right;

    ASTNode() {
        Type = Undefined;
        Value = 0;
        Left = nullptr;
        Right = nullptr;
    }

    ~ASTNode() {
        delete Left;
        delete Right;
    }
};


#endif //REPL_ASTNODE_H
