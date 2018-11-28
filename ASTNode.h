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
        ConstNumber,
        ConstBool,
        IfStmt,
        ForLoop,
        CompoundStmt,
        ReturnStmt,
        BreakStmt,
        ProgramTranslation
    };
}

namespace BinOpType {
    enum Type {
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

struct ASTNode {
    NodeType::ASTNodeType type;

    ASTNode() {
        type = NodeType::Undefined;
    };

    virtual ~ASTNode() {};
};

struct ProgramTranslationNode : ASTNode {
    std::vector<ASTNode*> statements;

    ProgramTranslationNode() {
        type = NodeType::ProgramTranslation;
    }

    ~ProgramTranslationNode() {
        for (const auto& currentStmt : statements) {
            delete currentStmt;
        }
    }
};

struct BinOpNode : ASTNode {
    BinOpType::Type binOpType;
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
};

struct ConstNumberNode : ASTNode {
    double value;

    ConstNumberNode() {
        type = NodeType::ConstNumber;
    }
};

struct ConstBoolNode : ASTNode {
    bool value;

    ConstBoolNode() {
        type = NodeType::ConstBool;
    }
};

struct IdentifierNode : ASTNode {
    std::string name;
    ValueType::Type valueType;

    IdentifierNode() {
        type = NodeType::Id;
        valueType = ValueType::Undefined;
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
};

struct IfStmtNode : ASTNode {
    ASTNode* condition;
    BlockStmtNode* body;
    std::vector<IfStmtNode*> elseIfStmts;
    BlockStmtNode* elseBody;

    IfStmtNode() {
        type = NodeType::IfStmt;
    }

    ~IfStmtNode() {
        delete condition;
        delete body;
        for (const auto& currentElseIfStmt : elseIfStmts) {
            delete currentElseIfStmt;
        }
        delete elseBody;
    }
};

struct ForLoopNode : ASTNode {
    ASTNode* init;
    ASTNode* condition;
    BinOpNode* inc;
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

struct FuncCallNode : ASTNode {
    std::string name;
    std::vector<ASTNode*> args;
    unsigned long argsSize;

    FuncCallNode() {
        type = NodeType::FuncCall;
    }

    ~FuncCallNode() {
        for (const auto& currentArg : args) {
            delete currentArg;
        }
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
        delete body;
    }
};

#endif //REPL_ASTNODE_H