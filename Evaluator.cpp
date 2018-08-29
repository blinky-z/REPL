#include "Evaluator.h"

void Evaluator::EvaluateAssignValue(BinOpNode* subtree) {
    IdentifierNode* id = static_cast<IdentifierNode*>(subtree->left);

    if (symbolTable.isIdExist(id->name)) {
        double value = EvaluateMathExpr(subtree->right);

        symbolTable.setIdValueDouble(id->name, value);
    } else {
        throw std::runtime_error("Can't assign value to undeclared identifier '" + id->name + "'");
    }
}

double Evaluator::EvaluateMathExpr(ASTNode* subtree) {
    if (subtree->type == NumberValue) {
        NumberNode* node = static_cast<NumberNode*>(subtree);

        return EvaluateNumberValue(node);
    } else if (subtree->type == Id) {
        IdentifierNode* node = static_cast<IdentifierNode*>(subtree);

        return EvaluateId(node);
    } else if (subtree->type == BinOp) {
        BinOpNode* node = static_cast<BinOpNode*>(subtree);

        double leftValue = EvaluateMathExpr(node->left);
        double rightValue = EvaluateMathExpr(node->right);

        switch (node->binOpType) {
            case OperatorPlus:
                return leftValue + rightValue;
            case OperatorMinus:
                return leftValue - rightValue;
            case OperatorMul:
                return leftValue * rightValue;
            case OperatorDiv:
                return leftValue / rightValue;
            default:
                throw std::runtime_error("Invalid binary operation");
        }
    } else {
        throw std::runtime_error("Invalid AST");
    }
}

void Evaluator::EvaluateDeclVar(DeclVarNode* subtree) {
    std::string idName = subtree->id->name;

    if (!symbolTable.isIdExist(idName)) {
        symbolTable.addNewIdentifier(idName);

        if (subtree->expr != nullptr) {
            double idValue = EvaluateMathExpr(subtree->expr);
            symbolTable.setIdValueDouble(idName, idValue);
        }
    } else {
        throw std::runtime_error("Redefinition of variable '" + idName + "'");
    }
}

std::string Evaluator::Evaluate(ASTNode* root) {
    if (root->type == BinOp) {
        BinOpNode* node = static_cast<BinOpNode*>(root);

        if (node->binOpType == OperatorAssign) {
            EvaluateAssignValue(node);
            return "Assign variable";
        } else {
            return std::to_string(EvaluateMathExpr(root));
        }
    } else if (root->type == DeclVar) {
        DeclVarNode* node = static_cast<DeclVarNode*>(root);

        EvaluateDeclVar(node);
        return "Declare Variable";
    } else if (root->type == NumberValue) {
        NumberNode* node = static_cast<NumberNode*>(root);

        return std::to_string(EvaluateNumberValue(node));
    } else if (root->type == Id) {
        IdentifierNode* node = static_cast<IdentifierNode*>(root);

        if (symbolTable.isIdExist(node->name)) {
            return std::to_string(EvaluateId(node));
        } else {
            throw std::runtime_error("Use of undeclared identifier '" + node->name + "'");
        }

    } else {
        throw std::runtime_error("Invalid AST");
    }
}

double Evaluator::EvaluateId(IdentifierNode* subtree) {
    return symbolTable.getIdValueDouble(subtree->name);
}

double Evaluator::EvaluateNumberValue(NumberNode* subtree) {
    return subtree->value;
}
