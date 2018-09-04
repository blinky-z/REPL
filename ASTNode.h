#ifndef REPL_ASTNODE_H
#define REPL_ASTNODE_H

#include <iostream>
#include <unordered_map>
#include <string>

enum ASTNodeType {
    Undefined,
    DeclVar,
    Empty,
    Id,
    BinOp,
    NumberValue,
    BoolValue,
    ForLoop,
    MathExpr,
    BoolExpr
};

enum ASTNodeBinOpType {
    OperatorLess,
    OperatorGreater,
    OperatorBoolAND,
    OperatorBoolOR,
    OperatorAssign,
    OperatorPlus,
    OperatorMinus,
    OperatorMul,
    OperatorDiv,
};

struct TypesStringNames {
    std::unordered_map<int, std::string> nodeTypeStringNames;
    std::unordered_map<int, std::string> binOpTypeStringNames;

    TypesStringNames() {
        nodeTypeStringNames[Undefined] = "Undefined";
        nodeTypeStringNames[DeclVar] = "Var Declaration";
        nodeTypeStringNames[Empty] = "Empty";
        nodeTypeStringNames[Id] = "Identifier";
        nodeTypeStringNames[BinOp] = "Binary Operation";
        nodeTypeStringNames[NumberValue] = "Number";
        nodeTypeStringNames[BoolValue] = "Bool";

        binOpTypeStringNames[OperatorAssign] = "Operator Assign";
        binOpTypeStringNames[OperatorPlus] = "Operator Plus";
        binOpTypeStringNames[OperatorMinus] = "Operator Minus";
        binOpTypeStringNames[OperatorMul] = "Operator Mul";
        binOpTypeStringNames[OperatorDiv] = "Operator Div";
    }
};

struct ASTNode {
    ASTNodeType type;

    ASTNode() {
        type = Undefined;
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
        type = NumberValue;
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
        type = BoolValue;
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
        type = Id;
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
        type = DeclVar;
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

struct ForLoopNode : ASTNode {

    ForLoopNode() {
        type = ForLoop;
    }

    void print() override {

    }
};

struct MathExprNode : ASTNode {
    ASTNode* expr;

    MathExprNode() {
        type = MathExpr;
    }

    void print() override {
        std::cout << std::endl;
        expr->print();
        std::cout << std::endl;
    }
};

struct BoolExprNode : ASTNode {
    ASTNode* expr;

    BoolExprNode() {
        type = BoolExpr;
    }

    void print() override {
        std::cout << std::endl;
        expr->print();
        std::cout << std::endl;
    }
};

#endif //REPL_ASTNODE_H