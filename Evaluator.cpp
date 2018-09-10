#include "Evaluator.h"

double Evaluator::EvaluateMathExpr(ASTNode* subtree) {
    if (subtree->type == NodeType::NumberValue) {
        NumberNode* node = dynamic_cast<NumberNode*>(subtree);

        if (node != nullptr) {
            return EvaluateNumberValue(node);
        } else {
            throw std::runtime_error("Invalid number node");
        }
    } else if (subtree->type == NodeType::BoolValue) {
        throw std::runtime_error("Incompatible operands type");
    } else if (subtree->type == NodeType::Id) {
        IdentifierNode* node = dynamic_cast<IdentifierNode*>(subtree);

        if (node != nullptr) {
            if (symbolTable.isIdExist(node->name)) {
                IdentifierValueType::ValueType idType = symbolTable.getIdType(node->name);

                if (idType == IdentifierValueType::Number) {
                    return EvaluateIdDouble(node);
                } else if (idType == IdentifierValueType::Undefined) {
                    throw std::runtime_error("Use of uninitialized identifier '" + node->name + "'");
                } else {
                    throw std::runtime_error("Incompatible operands type");
                }
            } else {
                throw std::runtime_error("Use of undeclared identifier '" + node->name + "'");
            }
        } else {
            throw std::runtime_error("Invalid Id node");
        }
    } else if (subtree->type == NodeType::BinOp) {
        BinOpNode* node = dynamic_cast<BinOpNode*>(subtree);

        if (node != nullptr) {
            double leftValue = EvaluateMathExpr(node->left);
            double rightValue = EvaluateMathExpr(node->right);

            switch (node->binOpType) {
                case BinOpType::OperatorPlus:
                    return leftValue + rightValue;
                case BinOpType::OperatorMinus:
                    return leftValue - rightValue;
                case BinOpType::OperatorMul:
                    return leftValue * rightValue;
                case BinOpType::OperatorDiv:
                    return leftValue / rightValue;
                default:
                    throw std::runtime_error("Incompatible operands type");
            }
        } else {
            throw std::runtime_error("Invalid binary operation node");
        }
    } else {
        throw std::runtime_error("Invalid AST");
    }
}

bool Evaluator::EvaluateBoolExpr(ASTNode* subtree) {
    if (subtree->type == NodeType::BoolValue) {
        BoolNode* node = dynamic_cast<BoolNode*>(subtree);

        if (node != nullptr) {
            return EvaluateBoolValue(node);
        } else {
            throw std::runtime_error("Invalid BoolValue Node");
        }
    } else if (subtree->type == NodeType::NumberValue) {
        throw std::runtime_error("Incompatible operands type");
    } else if (subtree->type == NodeType::Id) {
        IdentifierNode* node = dynamic_cast<IdentifierNode*>(subtree);

        if (node != nullptr) {
            if (symbolTable.isIdExist(node->name)) {
                IdentifierValueType::ValueType idType = symbolTable.getIdType(node->name);

                if (idType == IdentifierValueType::Bool) {
                    return EvaluateIdBool(node);
                } else if (idType == IdentifierValueType::Undefined) {
                    throw std::runtime_error("Use of uninitialized identifier '" + node->name + "'");
                } else {
                    throw std::runtime_error("Incompatible operands type");
                }
            } else {
                throw std::runtime_error("Use of undeclared identifier '" + node->name + "'");
            }
        } else {
            throw std::runtime_error("Invalid Id node");
        }
    } else if (subtree->type == NodeType::BinOp) {
        BinOpNode* node = dynamic_cast<BinOpNode*>(subtree);

        if (node != nullptr) {
            if (node->binOpType == BinOpType::OperatorEqual) {
                return EvaluateEqual(node);
            }

            bool leftValue = EvaluateBoolExpr(node->left);
            bool rightValue = EvaluateBoolExpr(node->right);

            switch (node->binOpType) {
                case BinOpType::OperatorBoolAND:
                    return leftValue && rightValue;
                case BinOpType::OperatorBoolOR:
                    return leftValue || rightValue;
                default:
                    throw std::runtime_error("Incompatible operands type");
            }
        } else {
            throw std::runtime_error("Invalid binary operation node");
        }
    } else {
        throw std::runtime_error("Invalid AST");
    }
}

void Evaluator::EvaluateAssignValue(IdentifierNode* lvalue, ASTNode* expr) {
    if (lvalue != nullptr) {
        if (symbolTable.isIdExist(lvalue->name)) {
            BinOpNode* binOpExpr = dynamic_cast<BinOpNode*>(expr);
            IdentifierNode* idExpr = dynamic_cast<IdentifierNode*>(expr);
            NumberNode* numValueExpr = dynamic_cast<NumberNode*>(expr);
            BoolNode* boolValueExpr = dynamic_cast<BoolNode*>(expr);

            if (binOpExpr != nullptr) {
                if (binOpExpr->binOpType == BinOpType::OperatorPlus ||
                    binOpExpr->binOpType == BinOpType::OperatorMinus ||
                    binOpExpr->binOpType == BinOpType::OperatorMul ||
                    binOpExpr->binOpType == BinOpType::OperatorDiv) {
                    double result = EvaluateMathExpr(binOpExpr);

                    symbolTable.setIdValueDouble(lvalue->name, result);
                } else if (binOpExpr->binOpType == BinOpType::OperatorBoolAND ||
                           binOpExpr->binOpType == BinOpType::OperatorBoolOR ||
                           binOpExpr->binOpType == BinOpType::OperatorEqual) {
                    bool result = EvaluateBoolExpr(binOpExpr);

                    symbolTable.setIdValueBool(lvalue->name, result);
                } else {
                    throw std::runtime_error("Invalid Binary Operation");
                }
            } else if (idExpr != nullptr) {
                if (symbolTable.isIdExist(idExpr->name)) {
                    int idExprType = symbolTable.getIdType(idExpr->name);

                    if (idExprType == IdentifierValueType::Number) {
                        symbolTable.setIdValueDouble(lvalue->name, EvaluateIdDouble(idExpr));
                    } else if (idExprType == IdentifierValueType::Bool) {
                        symbolTable.setIdValueBool(lvalue->name, EvaluateIdBool(idExpr));
                    } else if (idExprType == IdentifierValueType::Undefined) {
                        throw std::runtime_error("Use of uninitialized identifier '" + idExpr->name + "'");
                    } else {
                        throw std::runtime_error("Invalid ID value type");
                    }
                } else {
                    throw std::runtime_error("Use of undeclared identifier '" + idExpr->name + "'");
                }
            } else if (numValueExpr != nullptr) {
                symbolTable.setIdValueDouble(lvalue->name, EvaluateNumberValue(numValueExpr));
            } else if (boolValueExpr != nullptr) {
                symbolTable.setIdValueBool(lvalue->name, EvaluateBoolValue(boolValueExpr));
            } else {
                throw std::runtime_error("invalid expression in assign statement");
            }
        } else {
            throw std::runtime_error("Can't assign value to undeclared identifier '" + lvalue->name + "'");
        }
    } else {
        throw std::runtime_error("Invalid lvalue in assign statement");
    }
}

void Evaluator::EvaluateDeclVar(DeclVarNode* subtree) {
    if (subtree->id != nullptr) {
        std::string idName = subtree->id->name;

        if (!symbolTable.isIdExist(idName)) {
            symbolTable.addNewIdentifier(idName);

            if (subtree->expr != nullptr) {
                EvaluateAssignValue(subtree->id, subtree->expr);
            }
        } else {
            throw std::runtime_error("Redefinition of variable '" + idName + "'");
        }
    } else {
        throw std::runtime_error("Invalid Identifier node in variable declaration statement");
    }
}

bool Evaluator::EvaluateEqual(BinOpNode* subtree) {
    IdentifierValueType::ValueType leftOperandValueType = getNodeValueType(subtree->left);
    IdentifierValueType::ValueType rightOperandValueType = getNodeValueType(subtree->right);

    if (leftOperandValueType != rightOperandValueType) {
        throw std::runtime_error("Incompatible operands type");
    }

    NodeType::ASTNodeType leftOperandType = subtree->left->type;
    NodeType::ASTNodeType rightOperandType = subtree->right->type;

    if (leftOperandValueType == IdentifierValueType::Number) {
        double leftValue;
        double rightValue;

        if (leftOperandType == NodeType::NumberValue) {
            leftValue = EvaluateNumberValue(static_cast<NumberNode*>(subtree->left));
        } else if (leftOperandType == NodeType::BinOp) {
            leftValue = EvaluateMathExpr(subtree->left);
        } else if (leftOperandType == NodeType::Id) {
            leftValue = EvaluateIdDouble(static_cast<IdentifierNode*>(subtree->left));
        } else {
            throw std::runtime_error("Invalid left operand type");
        }

        if (rightOperandType == NodeType::NumberValue) {
            rightValue = EvaluateNumberValue(static_cast<NumberNode*>(subtree->right));
        } else if (rightOperandType == NodeType::BinOp) {
            rightValue = EvaluateMathExpr(subtree->right);
        } else if (rightOperandType == NodeType::Id) {
            rightValue = EvaluateIdDouble(static_cast<IdentifierNode*>(subtree->right));
        } else {
            throw std::runtime_error("Invalid right operand type");
        }

        return leftValue == rightValue;
    } else {
        bool leftValue;
        bool rightValue;

        if (leftOperandType == NodeType::BoolValue) {
            leftValue = EvaluateBoolValue(static_cast<BoolNode*>(subtree->left));
        } else if (leftOperandType == NodeType::BinOp) {
            leftValue = EvaluateBoolExpr(subtree->left);
        } else if (leftOperandType == NodeType::Id) {
            leftValue = EvaluateIdBool(static_cast<IdentifierNode*>(subtree->left));
        } else {
            throw std::runtime_error("Invalid left operand type");
        }

        if (rightOperandType == NodeType::BoolValue) {
            rightValue = EvaluateBoolValue(static_cast<BoolNode*>(subtree->right));
        } else if (rightOperandType == NodeType::BinOp) {
            rightValue = EvaluateBoolExpr(subtree->right);
        } else if (rightOperandType == NodeType::Id) {
            rightValue = EvaluateIdBool(static_cast<IdentifierNode*>(subtree->right));
        } else {
            throw std::runtime_error("Invalid right operand type");
        }

        return leftValue == rightValue;
    }
}

std::string Evaluator::Evaluate(ASTNode* root) {
    if (root->type == NodeType::BinOp) {
        BinOpNode* node = dynamic_cast<BinOpNode*>(root);

        if (node != nullptr) {
            if (node->binOpType == BinOpType::OperatorAssign) {
                IdentifierNode* id = dynamic_cast<IdentifierNode*>(node->left);

                EvaluateAssignValue(id, node->right);
                return "Assign Variable";
            } else if (node->binOpType == BinOpType::OperatorPlus ||
                       node->binOpType == BinOpType::OperatorMinus ||
                       node->binOpType == BinOpType::OperatorMul ||
                       node->binOpType == BinOpType::OperatorDiv) {
                return std::to_string(EvaluateMathExpr(node));
            } else if (node->binOpType == BinOpType::OperatorBoolAND ||
                       node->binOpType == BinOpType::OperatorBoolOR ||
                       node->binOpType == BinOpType::OperatorEqual) {
                bool result = EvaluateBoolExpr(node);

                return result ? "true" : "false";
            } else {
                throw std::runtime_error("Invalid Binary Operation");
            }
        } else {
            throw std::runtime_error("Invalid Binary Operation Node");
        }
    } else if (root->type == NodeType::DeclVar) {
        DeclVarNode* node = dynamic_cast<DeclVarNode*>(root);

        if (node != nullptr) {
            EvaluateDeclVar(node);
            return "Declare Variable";
        } else {
            throw std::runtime_error("Invalid Variable Declaration Node");
        }
    } else if (root->type == NodeType::NumberValue) {
        NumberNode* node = dynamic_cast<NumberNode*>(root);

        if (node != nullptr) {
            return std::to_string(EvaluateNumberValue(node));
        } else {
            throw std::runtime_error("Invalid Number Value Node");
        }
    } else if (root->type == NodeType::BoolValue) {
        BoolNode* node = dynamic_cast<BoolNode*>(root);

        if (node != nullptr) {
            return EvaluateBoolValue(node) ? "true" : "false";
        } else {
            throw std::runtime_error("Invalid Bool Value Node");
        }
    } else if (root->type == NodeType::Id) {
        IdentifierNode* node = dynamic_cast<IdentifierNode*>(root);

        if (node != nullptr) {
            if (symbolTable.isIdExist(node->name)) {
                int idType = symbolTable.getIdType(node->name);

                switch (idType) {
                    case IdentifierValueType::Number: {
                        return std::to_string(EvaluateIdDouble(node));
                    }
                    case IdentifierValueType::Bool: {
                        return EvaluateIdBool(node) ? "true" : "false";
                    }
                    case IdentifierValueType::Undefined: {
                        throw std::runtime_error("Use of uninitialized value");
                    }
                    default: {
                        throw std::runtime_error("invalid value type of identifier '" + node->name + "'");
                    }
                }
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

double Evaluator::EvaluateIdDouble(IdentifierNode* id) {
    return symbolTable.getIdValueDouble(id->name);
}

bool Evaluator::EvaluateIdBool(IdentifierNode* id) {
    return symbolTable.getIdValueBool(id->name);
}

double Evaluator::EvaluateNumberValue(NumberNode* num) {
    return num->value;
}

bool Evaluator::EvaluateBoolValue(BoolNode* num) {
    return num->value;
}

IdentifierValueType::ValueType Evaluator::getNodeValueType(ASTNode* node) {
    NodeType::ASTNodeType nodeType = node->type;
    IdentifierValueType::ValueType nodeValueType;

    if (nodeType == NodeType::BinOp) {
        BinOpNode* binOpNode = dynamic_cast<BinOpNode*>(node);

        if (binOpNode != nullptr) {
            if (binOpNode->binOpType == BinOpType::OperatorEqual ||
                binOpNode->binOpType == BinOpType::OperatorBoolOR ||
                binOpNode->binOpType == BinOpType::OperatorBoolAND) {
                nodeValueType = IdentifierValueType::Bool;
            } else if (binOpNode->binOpType == BinOpType::OperatorPlus ||
                       binOpNode->binOpType == BinOpType::OperatorMinus ||
                       binOpNode->binOpType == BinOpType::OperatorMul ||
                       binOpNode->binOpType == BinOpType::OperatorDiv) {
                nodeValueType = IdentifierValueType::Number;
            } else {
                throw std::runtime_error("Invalid binary operation type");
            }
        } else {
            throw std::runtime_error("Invalid Binary operation node");
        }
    } else if (nodeType == NodeType::NumberValue) {
        NumberNode* numNode = dynamic_cast<NumberNode*>(node);

        if (numNode != nullptr) {
            nodeValueType = IdentifierValueType::Number;
        } else {
            throw std::runtime_error("Invalid Number Value node");
        }
    } else if (nodeType == NodeType::BoolValue) {
        BoolNode* boolNode = dynamic_cast<BoolNode*>(node);

        if (boolNode != nullptr) {
            nodeValueType = IdentifierValueType::Bool;
        } else {
            throw std::runtime_error("Invalid Bool Value node");
        }
    } else if (nodeType == NodeType::Id) {
        IdentifierNode* id = dynamic_cast<IdentifierNode*>(node);

        if (id != nullptr) {
            if (symbolTable.isIdExist(id->name)) {
                IdentifierValueType::ValueType idValueType = symbolTable.getIdType(id->name);

                if (idValueType != IdentifierValueType::Undefined) {
                    nodeValueType = idValueType;
                } else {
                    throw std::runtime_error("Use of uninitialized identifier '" + id->name + "'");
                }
            } else {
                throw std::runtime_error("Use of undeclared identifier '" + id->name + "'");
            }
        } else {
            throw std::runtime_error("Invalid Identifier node");
        }
    } else {
        throw std::runtime_error("Invalid node");
    }

    return nodeValueType;
}
