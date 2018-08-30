#include "Evaluator.h"

void Evaluator::EvaluateAssignValue(BinOpNode* subtree) {
    IdentifierNode* id = dynamic_cast<IdentifierNode*>(subtree->left);

    if (id != nullptr) {
        if (symbolTable.isIdExist(id->name)) {
            double value = EvaluateMathExpr(subtree->right);

            symbolTable.setIdValueDouble(id->name, value);
        } else {
            throw std::runtime_error("Can't assign value to undeclared identifier '" + id->name + "'");
        }
    } else {
        throw std::runtime_error("Invalid BinOp Assign Node");
    }
}

double Evaluator::EvaluateMathExpr(ASTNode* subtree) {
    if (subtree->type == NumberValue) {
        NumberNode* node = dynamic_cast<NumberNode*>(subtree);

        if (node != nullptr) {
            return EvaluateNumberValue(node);
        } else {
            throw std::runtime_error("Invalid NumberValue Node");
        }
    } else if (subtree->type == Id) {
        IdentifierNode* node = dynamic_cast<IdentifierNode*>(subtree);

        if (node != nullptr) {
            if (symbolTable.isIdExist(node->name)) {
                return EvaluateId(node);
            } else {
                throw std::runtime_error("Use of undeclared identifier '" + node->name + "'");
            }
        } else {
            throw std::runtime_error("Invalid Id Node");
        }
    } else if (subtree->type == BinOp) {
        BinOpNode* node = dynamic_cast<BinOpNode*>(subtree);

        if (node != nullptr) {
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
            throw std::runtime_error("Invalid BinOp Math operation Node");
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
        BinOpNode* node = dynamic_cast<BinOpNode*>(root);

        if (node != nullptr) {
            if (node->binOpType == OperatorAssign) {
                EvaluateAssignValue(node);
                return "Assign Variable";
            } else {
                return std::to_string(EvaluateMathExpr(root));
            }
        } else {
            throw std::runtime_error("Invalid BinOp Node");
        }
    } else if (root->type == DeclVar) {
        DeclVarNode* node = dynamic_cast<DeclVarNode*>(root);

        if (node != nullptr) {
            EvaluateDeclVar(node);
            return "Declare Variable";
        } else {
            throw std::runtime_error("Invalid DeclVar Node");
        }

    } else if (root->type == NumberValue) {
        NumberNode* node = dynamic_cast<NumberNode*>(root);

        if (node != nullptr) {
            return std::to_string(EvaluateNumberValue(node));
        } else {
            throw std::runtime_error("Invalid NumberValue Node");
        }

    } else if (root->type == Id) {
        IdentifierNode* node = dynamic_cast<IdentifierNode*>(root);

        if (node != nullptr) {
            if (symbolTable.isIdExist(node->name)) {
                return std::to_string(EvaluateId(node));
            } else {
                throw std::runtime_error("Use of undeclared identifier '" + node->name + "'");
            }
        } else {
            throw std::runtime_error("Invalid Id Node");
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
