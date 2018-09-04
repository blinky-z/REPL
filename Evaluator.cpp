#include "Evaluator.h"

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
                return EvaluateIdDouble(node);
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
                    throw std::runtime_error("Invalid binary math operation");
            }
        } else {
            throw std::runtime_error("Invalid BinOp Math operation Node");
        }
    } else {
        throw std::runtime_error("Invalid AST");
    }
}

bool Evaluator::EvaluateBoolExpr(ASTNode* subtree) {
    if (subtree->type == BoolValue) {
        BoolNode* node = dynamic_cast<BoolNode*>(subtree);

        if (node != nullptr) {
            return EvaluateBoolValue(node);
        } else {
            throw std::runtime_error("Invalid BoolValue Node");
        }
    } else if (subtree->type == Id) {
        IdentifierNode* node = dynamic_cast<IdentifierNode*>(subtree);

        if (node != nullptr) {
            if (symbolTable.isIdExist(node->name)) {
                return EvaluateIdBool(node);
            } else {
                throw std::runtime_error("Use of undeclared identifier '" + node->name + "'");
            }
        } else {
            throw std::runtime_error("Invalid Id Node");
        }
    } else if (subtree->type == BinOp) {
        BinOpNode* node = dynamic_cast<BinOpNode*>(subtree);

        if (node != nullptr) {
            bool leftValue = EvaluateBoolExpr(node->left);
            bool rightValue = EvaluateBoolExpr(node->right);

            switch (node->binOpType) {
                case OperatorBoolAND:
                    return leftValue && rightValue;
                case OperatorBoolOR:
                    return leftValue || rightValue;
                default:
                    throw std::runtime_error("Invalid binary bool operation");
            }
        } else {
            throw std::runtime_error("Invalid BinOp Bool operation Node");
        }
    } else {
        throw std::runtime_error("Invalid AST");
    }
}

void Evaluator::EvaluateAssignValue(BinOpNode* subtree) {
    IdentifierNode* id = dynamic_cast<IdentifierNode*>(subtree->left);

    if (id != nullptr) {
        if (symbolTable.isIdExist(id->name)) {
            MathExprNode* mathExpr = dynamic_cast<MathExprNode*>(subtree->right);
            BoolExprNode* boolExpr = dynamic_cast<BoolExprNode*>(subtree->right);
            IdentifierNode* idExpr = dynamic_cast<IdentifierNode*>(subtree->right);

            if (mathExpr != nullptr) {
                double value = EvaluateMathExpr(mathExpr->expr);

                symbolTable.setIdValueDouble(id->name, value);
            } else if (boolExpr != nullptr) {
                bool value = EvaluateBoolExpr(boolExpr->expr);

                symbolTable.setIdValueBool(id->name, value);
            } else if (idExpr != nullptr) {
                if (symbolTable.isIdExist(idExpr->name)) {
                    int idExprType = symbolTable.getIdType(idExpr->name);

                    if (idExprType == IdentifierValueType::Number) {
                        symbolTable.setIdValueDouble(id->name, EvaluateIdDouble(idExpr));
                    } else if (idExprType == IdentifierValueType::Bool) {
                        symbolTable.setIdValueBool(id->name, EvaluateIdBool(idExpr));
                    } else {
                        throw std::runtime_error("Can not assign: Invalid rvalue type");
                    }
                } else {
                    throw std::runtime_error("Use of undeclared identifier '" + idExpr->name + "'");
                }
            } else {
                throw std::runtime_error("invalid assign expression");
            }
        } else {
            throw std::runtime_error("Can't assign value to undeclared identifier '" + id->name + "'");
        }
    } else {
        throw std::runtime_error("Invalid BinOp Assign Node");
    }
}

void Evaluator::EvaluateDeclVar(DeclVarNode* subtree) {
    std::string idName = subtree->id->name;

    if (!symbolTable.isIdExist(idName)) {
        symbolTable.addNewIdentifier(idName);

        if (subtree->expr != nullptr) {
            MathExprNode* mathExpr = dynamic_cast<MathExprNode*>(subtree->expr);
            BoolExprNode* boolExpr = dynamic_cast<BoolExprNode*>(subtree->expr);
            IdentifierNode* idExpr = dynamic_cast<IdentifierNode*>(subtree->expr);

            if (mathExpr != nullptr) {
                double value = EvaluateMathExpr(mathExpr->expr);

                symbolTable.setIdValueDouble(idName, value);
            } else if (boolExpr != nullptr) {
                bool value = EvaluateBoolExpr(boolExpr->expr);

                symbolTable.setIdValueBool(idName, value);
            } else if (idExpr != nullptr) {
                if (symbolTable.isIdExist(idExpr->name)) {
                    int idExprType = symbolTable.getIdType(idExpr->name);

                    if (idExprType == IdentifierValueType::Number) {
                        symbolTable.setIdValueDouble(idName, EvaluateIdDouble(idExpr));
                    } else if (idExprType == IdentifierValueType::Bool) {
                        symbolTable.setIdValueBool(idName, EvaluateIdBool(idExpr));
                    } else {
                        throw std::runtime_error("Use of uninitialized value");
                    }
                } else {
                    throw std::runtime_error("Use of undeclared identifier '" + idExpr->name + "'");
                }
            } else {
                throw std::runtime_error("invalid assign expression");
            }
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
                throw std::runtime_error("Invalid BinOp Type");
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

    } else if (root->type == Id) {
        IdentifierNode* node = dynamic_cast<IdentifierNode*>(root);

        if (node != nullptr) {
            if (symbolTable.isIdExist(node->name)) {
                int idType = symbolTable.getIdType(node->name);

                switch (idType) {
                    case IdentifierValueType::Number: {
                        return std::to_string(EvaluateIdDouble(node));
                    }
                    case IdentifierValueType::Bool: {
                        bool value = EvaluateIdBool(node);

                        return value ? "true" : "false";
                    }
                    case IdentifierValueType::Undefined: {
                        throw std::runtime_error("Use of uninitialized value");
                    }
                    default: {
                        throw std::runtime_error("invalid Identifier type");
                    }
                }
            } else {
                throw std::runtime_error("Use of undeclared identifier '" + node->name + "'");
            }
        } else {
            throw std::runtime_error("Invalid Id Node");
        }
    } else if (root->type == MathExpr) {
        MathExprNode* node = dynamic_cast<MathExprNode*>(root);

        if (node != nullptr) {
            return std::to_string(EvaluateMathExpr(node->expr));
        } else {
            throw std::runtime_error("Invalid Math Expr Node");
        }
    } else if (root->type == BoolExpr) {
        BoolExprNode* node = dynamic_cast<BoolExprNode*>(root);

        if (node != nullptr) {
            bool value = EvaluateBoolExpr(node->expr);

            return value ? "true" : "false";
        } else {
            throw std::runtime_error("Invalid Bool Expr Node");
        }
    } else {
        throw std::runtime_error("Invalid AST");
    }
}

double Evaluator::EvaluateIdDouble(IdentifierNode* subtree) {
    return symbolTable.getIdValueDouble(subtree->name);
}

bool Evaluator::EvaluateIdBool(IdentifierNode* subtree) {
    return symbolTable.getIdValueBool(subtree->name);
}

double Evaluator::EvaluateNumberValue(NumberNode* subtree) {
    return subtree->value;
}

bool Evaluator::EvaluateBoolValue(BoolNode* subtree) {
    return subtree->value;
}
