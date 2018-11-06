#ifndef REPL_ASTNODE_H
#define REPL_ASTNODE_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include "Identifier.h"

namespace NodeType {
    enum ASTNodeType {
        Undefined,
        DeclVar,
        Id,
        DeclFunc,
        FuncCall,
        BinOp,
        NumberValue,
        BoolValue,
        IfStmt,
        ForLoop,
        CompoundStmt,
        ReturnStmt,
        BreakStmt
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
        nodeTypeStringNames[NodeType::ForLoop] = "For loop Statement";
        nodeTypeStringNames[NodeType::CompoundStmt] = "Compound Statement";

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
                  << "[Binary operator]: " << typeString.binOpTypeStringNames[binOpType] << std::endl
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

    ~BlockStmtNode() {
        for (const auto& currentStmt : stmtList) {
            delete currentStmt;
        }
    }

    void print() override {
        TypesStringNames typeString;

        std::cout << std::endl;
        std::cout << "[Type]: " << typeString.nodeTypeStringNames[type] << std::endl;
        std::cout << "Statements:" << std::endl;
        if (!stmtList.empty()) {
            for (const auto& currentStmt : stmtList) {
                currentStmt->print();
            }
        } else {
            std::cout << "Empty body" << std::endl;
        }
        std::cout << std::endl;
    }
};

struct IfStmtNode : ASTNode {
    ASTNode* condition;
    BlockStmtNode* body;
    BlockStmtNode* elseBody;

    IfStmtNode() {
        type = NodeType::IfStmt;
    }

    ~IfStmtNode() {
        delete condition;
        delete body;
        delete elseBody;
    }

    void print() override {
        TypesStringNames typeString;

        std::cout << std::endl;
        std::cout << "[Type]: " << typeString.nodeTypeStringNames[type] << std::endl;
        std::cout << "Condition: " << std::endl;
        condition->print();
        std::cout << "Body: " << std::endl;
        body->print();
        std::cout << "Else Body: " << std::endl;
        body->print();
        std::cout << std::endl;
    }
};

struct ForLoopNode : ASTNode {
    ASTNode* init;
    ASTNode* condition;
    ASTNode* inc;
    BlockStmtNode* body;

    ForLoopNode() {
        type = NodeType::ForLoop;
    }

    ~ForLoopNode() {
        delete init;
        delete condition;
        delete inc;
        delete body;
    }

    void print() override {
        TypesStringNames typeString;

        std::cout << std::endl;
        std::cout << "[Type]: " << typeString.nodeTypeStringNames[type] << std::endl;
        std::cout << "Initialization expression: " << std::endl;
        if (init != nullptr) {
            init->print();
        } else {
            std::cout << "Empty expr" << std::endl;
        }
        std::cout << "Condition: " << std::endl;
        if (condition != nullptr) {
            condition->print();
        } else {
            std::cout << "Empty expr" << std::endl;
        }
        std::cout << "Body: " << std::endl;
        body->print();
        std::cout << std::endl;
    }
};

struct ReturnStmtNode : ASTNode {
    ASTNode* expression;

    ReturnStmtNode() {
        type = NodeType::ReturnStmt;
    }

    ~ReturnStmtNode() {
        delete expression;
    }
};

struct BreakStmtNode : ASTNode {
    BreakStmtNode() {
        type = NodeType::BreakStmt;
    }
};

struct DeclFuncNode : ASTNode {
    std::string name;
    ValueType::Type returnType;
    std::vector<IdentifierNode*> args;
    unsigned long argsSize;
    BlockStmtNode* body;

    DeclFuncNode() {
        type = NodeType::DeclFunc;
    }

    ~DeclFuncNode() {
        for (const auto currentId : args) {
            delete currentId;
        }
//        delete body;
    }

    void print() override {

    }
};

struct FuncCallNode : ASTNode {
    std::string name;
    std::vector<ASTNode*> args;
    unsigned long argsSize;

    FuncCallNode() {
        type = NodeType::FuncCall;
    }

    ~FuncCallNode() {
        for (const auto currentId : args) {
            delete currentId;
        }
    }

    void print() override {

    }
};

#endif //REPL_ASTNODE_H