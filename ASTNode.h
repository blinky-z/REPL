#ifndef REPL_ASTNODE_H
#define REPL_ASTNODE_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

namespace NodeType {
    enum ASTNodeType {
        Undefined,
        DeclVar,
        Id,
        BinOp,
        NumberValue,
        BoolValue,
        IfStmt,
        ForLoop,
        CompoundStmt
    };
}

namespace BinOpType {
    enum ASTNodeBinOpType {
        OperatorAssign,
        OperatorPlus,
        OperatorMinus,
        OperatorMul,
        OperatorDiv,
        OperatorBoolAND,
        OperatorBoolOR,
        OperatorEqual,
        OperatorLess,
        OperatorGreater
    };
}

struct TypesStringNames {
    std::unordered_map<int, std::string> nodeTypeStringNames;
    std::unordered_map<int, std::string> binOpTypeStringNames;

    TypesStringNames() {
        nodeTypeStringNames[NodeType::Undefined] = "Undefined";
        nodeTypeStringNames[NodeType::DeclVar] = "Var Declaration";
        nodeTypeStringNames[NodeType::Id] = "Identifier";
        nodeTypeStringNames[NodeType::BinOp] = "Binary Operation";
        nodeTypeStringNames[NodeType::NumberValue] = "Number";
        nodeTypeStringNames[NodeType::BoolValue] = "Bool";
        nodeTypeStringNames[NodeType::IfStmt] = "If Statement";

        binOpTypeStringNames[BinOpType::OperatorAssign] = "Operator Assign";
        binOpTypeStringNames[BinOpType::OperatorPlus] = "Operator Plus";
        binOpTypeStringNames[BinOpType::OperatorMinus] = "Operator Minus";
        binOpTypeStringNames[BinOpType::OperatorMul] = "Operator Mul";
        binOpTypeStringNames[BinOpType::OperatorDiv] = "Operator Div";
        binOpTypeStringNames[BinOpType::OperatorBoolAND] = "Logical AND";
        binOpTypeStringNames[BinOpType::OperatorBoolOR] = "Logical OR";
        binOpTypeStringNames[BinOpType::OperatorEqual] = "Operator Equal To";
        binOpTypeStringNames[BinOpType::OperatorLess] = "Operator LESS THAN";
        binOpTypeStringNames[BinOpType::OperatorGreater] = "Operator GREATER THAN";
    }
};

struct ASTNode {
    NodeType::ASTNodeType type;

    ASTNode() {
        type = NodeType::Undefined;
    };

    virtual ~ASTNode() {};

    virtual void print() {
        TypesStringNames typeString;

        std::cout << std::endl;
        std::cout << "[Type]: " << typeString.nodeTypeStringNames[type] << std::endl;
        std::cout << std::endl;
    }
};

struct BinOpNode : ASTNode {
    BinOpType::ASTNodeBinOpType binOpType;
    ASTNode* left;
    ASTNode* right;

    BinOpNode() {
        type = NodeType::BinOp;
        left = nullptr;
        right = nullptr;
    }

    ~BinOpNode() {
        delete left;
        delete right;
    }

    void print() override {
        TypesStringNames typeString;

        std::cout << std::endl;
        std::cout << "[Type]: " << typeString.nodeTypeStringNames[type] << std::endl
                  << "[Binary operation type]: " << typeString.binOpTypeStringNames[binOpType] << std::endl
                  << "[Left value info]: " << std::endl;
        left->print();
        std::cout << "[Right value info]: " << std::endl;
        right->print();
        std::cout << std::endl;
    }
};

struct NumberNode : ASTNode {
    double value;

    NumberNode() {
        type = NodeType::NumberValue;
    }

    void print() override {
        TypesStringNames typeString;

        std::cout << std::endl;
        std::cout << "[Type]: " << typeString.nodeTypeStringNames[type] << std::endl
                  << "[Value]: " << value << std::endl;
        std::cout << std::endl;
    }
};

struct BoolNode : ASTNode {
    bool value;

    BoolNode() {
        type = NodeType::BoolValue;
    }

    void print() override {
        TypesStringNames typeString;

        std::cout << std::endl;
        std::cout << "[Type]: " << typeString.nodeTypeStringNames[type] << std::endl
                  << "[Value]: " << value << std::endl;
        std::cout << std::endl;
    }
};

struct IdentifierNode : ASTNode {
    std::string name;

    IdentifierNode() {
        type = NodeType::Id;
    }

    void print() override {
        TypesStringNames typeString;

        std::cout << std::endl;
        std::cout << "[Type]: " << typeString.nodeTypeStringNames[type] << std::endl
                  << "[Name]: " << name << std::endl;
        std::cout << std::endl;
    }
};

struct DeclVarNode : ASTNode {
    IdentifierNode* id;
    ASTNode* expr;

    DeclVarNode() {
        type = NodeType::DeclVar;
        id = nullptr;
        expr = nullptr;
    }

    ~DeclVarNode() {
        delete id;
        delete expr;
    }

    void print() override {
        TypesStringNames typeString;

        std::cout << std::endl;
        std::cout << "[Type]: " << typeString.nodeTypeStringNames[type] << std::endl
                  << "[Id info]:" << std::endl;
        id->print();
        std::cout << "[Expr info]:" << std::endl;
        if (expr != nullptr) {
            expr->print();
        } else {
            std::cout << "Empty expr" << std::endl;
        }
        std::cout << std::endl;
    }
};

struct BlockStmtNode : ASTNode {
    std::vector<ASTNode*> stmtList;

    BlockStmtNode() {
        type = NodeType::CompoundStmt;
    }

    void print() override {
        TypesStringNames typeString;

        std::cout << std::endl;
        std::cout << "[Type]: " << typeString.nodeTypeStringNames[type] << std::endl;
        std::cout << "Statements:" << std::endl;
        for (const auto& currentStmt : stmtList) {
            currentStmt->print();
        }
        std::cout << std::endl;
    }
};

struct IfStmtNode : ASTNode {
    ASTNode* condition;
    BlockStmtNode* body;
    ASTNode* elseStmt;

    IfStmtNode() {
        type = NodeType::IfStmt;
        elseStmt = nullptr;
    }

    void print() override {
        TypesStringNames typeString;

        std::cout << std::endl;
        std::cout << "[Type]: " << typeString.nodeTypeStringNames[type] << std::endl;
        std::cout << "Condition: " << std::endl;
        condition->print();
        std::cout << std::endl;
    }
};

struct ForLoopNode : ASTNode {

    ForLoopNode() {
        type = NodeType::ForLoop;
    }

    void print() override {

    }
};

#endif //REPL_ASTNODE_H