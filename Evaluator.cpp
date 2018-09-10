#include "Evaluator.h"

EvalResult Evaluator::EvaluateMathExpr(ASTNode* subtree) {
    EvalResult result;
    if (subtree->type == NodeType::NumberValue) {
        NumberNode* node = dynamic_cast<NumberNode*>(subtree);

        if (node != nullptr) {
            result.setValueDouble(EvaluateNumberValue(node));
        } else {
            result.error = newError(EvalErrorCode::INVALID_AST, "Invalid Number Constant Node");
        }
    } else if (subtree->type == NodeType::BoolValue) {
        result.error = newError(EvalErrorCode::INCOMPATIBLE_OPERANDS_TYPE);
    } else if (subtree->type == NodeType::Id) {
        IdentifierNode* node = dynamic_cast<IdentifierNode*>(subtree);

        if (node != nullptr) {
            if (symbolTable.isIdExist(node->name)) {
                IdentifierValueType::ValueType idType = symbolTable.getIdType(node->name);

                if (idType == IdentifierValueType::Number) {
                    result.setValueDouble(EvaluateIdDouble(node));
                } else if (idType == IdentifierValueType::Undefined) {
                    result.error = newError(EvalErrorCode::UNINITIALIZED_VAR,
                                            "Use of uninitialized identifier '" + node->name + "'");
                } else {
                    result.error = newError(EvalErrorCode::INCOMPATIBLE_OPERANDS_TYPE);
                }
            } else {
                result.error = newError(EvalErrorCode::UNDECLARED_VAR,
                                        "Use of undeclared identifier '" + node->name + "'");
            }
        } else {
            result.error = newError(EvalErrorCode::INVALID_AST, "Invalid Identifier Node");
        }
    } else if (subtree->type == NodeType::BinOp) {
        BinOpNode* node = dynamic_cast<BinOpNode*>(subtree);

        if (node != nullptr) {
            EvalResult leftValue = EvaluateMathExpr(node->left);
            if (leftValue.isError()) {
                return leftValue;
            }
            EvalResult rightValue = EvaluateMathExpr(node->right);
            if (leftValue.isError()) {
                return rightValue;
            }

            switch (node->binOpType) {
                case BinOpType::OperatorPlus:
                    result.setValueDouble(leftValue.getResultDouble() + rightValue.getResultDouble());
                    break;
                case BinOpType::OperatorMinus:
                    result.setValueDouble(leftValue.getResultDouble() - rightValue.getResultDouble());
                    break;
                case BinOpType::OperatorMul:
                    result.setValueDouble(leftValue.getResultDouble() * rightValue.getResultDouble());
                    break;
                case BinOpType::OperatorDiv:
                    result.setValueDouble(leftValue.getResultDouble() / rightValue.getResultDouble());
                    break;
                default:
                    result.error = newError(EvalErrorCode::INCOMPATIBLE_OPERANDS_TYPE);
            }
        } else {
            result.error = newError(EvalErrorCode::INVALID_AST, "Invalid Binary Operation Node");
        }
    } else {
        result.error = newError(EvalErrorCode::INVALID_AST);
    }

    return result;
}

EvalResult Evaluator::EvaluateBoolExpr(ASTNode* subtree) {
    EvalResult result;

    if (subtree->type == NodeType::BoolValue) {
        BoolNode* node = dynamic_cast<BoolNode*>(subtree);

        if (node != nullptr) {
            result.setValueBool(EvaluateBoolValue(node));
        } else {
            result.error = newError(EvalErrorCode::INVALID_AST, "Invalid Bool Constant Node");
        }
    } else if (subtree->type == NodeType::NumberValue) {
        result.error = newError(EvalErrorCode::INCOMPATIBLE_OPERANDS_TYPE);
    } else if (subtree->type == NodeType::Id) {
        IdentifierNode* node = dynamic_cast<IdentifierNode*>(subtree);

        if (node != nullptr) {
            if (symbolTable.isIdExist(node->name)) {
                IdentifierValueType::ValueType idType = symbolTable.getIdType(node->name);

                if (idType == IdentifierValueType::Bool) {
                    result.setValueBool(EvaluateIdBool(node));
                } else if (idType == IdentifierValueType::Undefined) {
                    result.error = newError(EvalErrorCode::UNINITIALIZED_VAR,
                                            "Use of uninitialized identifier '" + node->name + "'");
                } else {
                    result.error = newError(EvalErrorCode::INCOMPATIBLE_OPERANDS_TYPE);
                }
            } else {
                result.error = newError(EvalErrorCode::UNDECLARED_VAR,
                                        "Use of undeclared identifier '" + node->name + "'");
            }
        } else {
            result.error = newError(EvalErrorCode::INVALID_AST, "Invalid Identifier Node");
        }
    } else if (subtree->type == NodeType::BinOp) {
        BinOpNode* node = dynamic_cast<BinOpNode*>(subtree);

        if (node != nullptr) {
            if (node->binOpType == BinOpType::OperatorEqual) {
                return EvaluateEqual(node);
            }

            EvalResult leftValue = EvaluateBoolExpr(node->left);
            if (leftValue.isError()) {
                return leftValue;
            }
            EvalResult rightValue = EvaluateBoolExpr(node->right);
            if (leftValue.isError()) {
                return rightValue;
            }

            switch (node->binOpType) {
                case BinOpType::OperatorBoolAND:
                    result.setValueBool(leftValue.getResultBool() && rightValue.getResultBool());
                    break;
                case BinOpType::OperatorBoolOR:
                    result.setValueBool(leftValue.getResultBool() || rightValue.getResultBool());
                    break;
                default:
                    result.error = newError(EvalErrorCode::INCOMPATIBLE_OPERANDS_TYPE);
            }
        } else {
            result.error = newError(EvalErrorCode::INVALID_AST, "Invalid Binary Operation Node");
        }
    } else {
        result.error = newError(EvalErrorCode::INVALID_AST);
    }

    return result;
}

EvalResult Evaluator::EvaluateAssignValue(IdentifierNode* lvalue, ASTNode* expr) {
    EvalResult result;

    if (lvalue != nullptr) {
        if (symbolTable.isIdExist(lvalue->name)) {
            BinOpNode* binOpExpr = dynamic_cast<BinOpNode*>(expr);
            IdentifierNode* idExpr = dynamic_cast<IdentifierNode*>(expr);
            NumberNode* numberConst = dynamic_cast<NumberNode*>(expr);
            BoolNode* boolConst = dynamic_cast<BoolNode*>(expr);

            if (binOpExpr != nullptr) {
                if (binOpExpr->binOpType == BinOpType::OperatorPlus ||
                    binOpExpr->binOpType == BinOpType::OperatorMinus ||
                    binOpExpr->binOpType == BinOpType::OperatorMul ||
                    binOpExpr->binOpType == BinOpType::OperatorDiv) {
                    EvalResult exprResult = EvaluateMathExpr(binOpExpr);

                    if (!exprResult.isError()) {
                        symbolTable.setIdValueDouble(lvalue->name, exprResult.getResultDouble());
                    } else {
                        return exprResult;
                    }
                } else if (binOpExpr->binOpType == BinOpType::OperatorBoolAND ||
                           binOpExpr->binOpType == BinOpType::OperatorBoolOR ||
                           binOpExpr->binOpType == BinOpType::OperatorEqual) {
                    EvalResult exprResult = EvaluateBoolExpr(binOpExpr);

                    if (!exprResult.isError()) {
                        symbolTable.setIdValueBool(lvalue->name, exprResult.getResultBool());
                    } else {
                        return exprResult;
                    }
                } else {
                    result.error = newError(EvalErrorCode::INVALID_AST, "Invalid Binary Operation");
                }
            } else if (idExpr != nullptr) {
                if (symbolTable.isIdExist(idExpr->name)) {
                    int idExprType = symbolTable.getIdType(idExpr->name);

                    if (idExprType == IdentifierValueType::Number) {
                        symbolTable.setIdValueDouble(lvalue->name, EvaluateIdDouble(idExpr));
                    } else if (idExprType == IdentifierValueType::Bool) {
                        symbolTable.setIdValueBool(lvalue->name, EvaluateIdBool(idExpr));
                    } else if (idExprType == IdentifierValueType::Undefined) {
                        result.error = newError(EvalErrorCode::UNINITIALIZED_VAR,
                                                "Use of uninitialized identifier '" + idExpr->name + "'");
                    } else {
                        result.error = newError(EvalErrorCode::INVALID_VALUE_TYPE);
                    }
                } else {
                    result.error = newError(EvalErrorCode::UNDECLARED_VAR,
                                            "Use of undeclared identifier '" + idExpr->name + "'");
                }
            } else if (numberConst != nullptr) {
                symbolTable.setIdValueDouble(lvalue->name, EvaluateNumberValue(numberConst));
            } else if (boolConst != nullptr) {
                symbolTable.setIdValueBool(lvalue->name, EvaluateBoolValue(boolConst));
            } else {
                result.error = newError(EvalErrorCode::INVALID_AST, "Invalid expression in assign expression");
            }
        } else {
            result.error = newError(EvalErrorCode::UNDECLARED_VAR_LVALUE);
        }
    } else {
        result.error = newError(EvalErrorCode::INVALID_AST, "Invalid Lvalue in assign expression");
    }

    return result;
}

EvalResult Evaluator::EvaluateDeclVar(DeclVarNode* subtree) {
    EvalResult result;

    if (subtree->id != nullptr) {
        std::string idName = subtree->id->name;

        if (!symbolTable.isIdExist(idName)) {
            symbolTable.addNewIdentifier(idName);

            if (subtree->expr != nullptr) {
                result = EvaluateAssignValue(subtree->id, subtree->expr);
            }
            result.setValueString("Declare Variable");
        } else {
            result.error = newError(EvalErrorCode::VAR_DEREFINITION);
        }
    } else {
        result.error = newError(EvalErrorCode::INVALID_AST);
    }

    return result;
}

EvalResult Evaluator::EvaluateEqual(BinOpNode* subtree) {
    EvalResult result;

    IdentifierValueType::ValueType leftOperandValueType = getNodeValueType(subtree->left);
    IdentifierValueType::ValueType rightOperandValueType = getNodeValueType(subtree->right);

    if (leftOperandValueType != rightOperandValueType) {
        result.error = newError(EvalErrorCode::INCOMPATIBLE_OPERANDS_TYPE);
        return result;
    }

    NodeType::ASTNodeType leftOperandType = subtree->left->type;
    NodeType::ASTNodeType rightOperandType = subtree->right->type;

    if (leftOperandValueType == IdentifierValueType::Number) {
        double leftValue;
        double rightValue;

        if (leftOperandType == NodeType::NumberValue) {
            leftValue = EvaluateNumberValue(static_cast<NumberNode*>(subtree->left));
        } else if (leftOperandType == NodeType::BinOp) {
            const EvalResult& exprResult = EvaluateMathExpr(subtree->left);

            if (exprResult.isError()) {
                return exprResult;

            }

            leftValue = exprResult.getResultDouble();
        } else if (leftOperandType == NodeType::Id) {
            leftValue = EvaluateIdDouble(static_cast<IdentifierNode*>(subtree->left));
        } else {
            result.error = newError(EvalErrorCode::INVALID_VALUE_TYPE);
            return result;
        }

        if (rightOperandType == NodeType::NumberValue) {
            rightValue = EvaluateNumberValue(static_cast<NumberNode*>(subtree->right));
        } else if (rightOperandType == NodeType::BinOp) {
            const EvalResult& exprResult = EvaluateMathExpr(subtree->right);

            if (exprResult.isError()) {
                return exprResult;
            }

            rightValue = exprResult.getResultDouble();
        } else if (rightOperandType == NodeType::Id) {
            rightValue = EvaluateIdDouble(static_cast<IdentifierNode*>(subtree->right));
        } else {
            result.error = newError(EvalErrorCode::INVALID_VALUE_TYPE);
            return result;
        }

        result.setValueBool(leftValue == rightValue);
    } else {
        bool leftValue;
        bool rightValue;

        if (leftOperandType == NodeType::BoolValue) {
            leftValue = EvaluateBoolValue(static_cast<BoolNode*>(subtree->left));
        } else if (leftOperandType == NodeType::BinOp) {
            const EvalResult& exprResult = EvaluateBoolExpr(subtree->left);

            if (exprResult.isError()) {
                return exprResult;
            }

            leftValue = EvaluateBoolExpr(subtree->left).getResultBool();
        } else if (leftOperandType == NodeType::Id) {
            leftValue = EvaluateIdBool(static_cast<IdentifierNode*>(subtree->left));
        } else {
            result.error = newError(EvalErrorCode::INVALID_VALUE_TYPE);
            return result;
        }


        if (rightOperandType == NodeType::BoolValue) {
            rightValue = EvaluateBoolValue(static_cast<BoolNode*>(subtree->right));
        } else if (rightOperandType == NodeType::BinOp) {
            const EvalResult& exprResult = EvaluateBoolExpr(subtree->right);

            if (exprResult.isError()) {
                return exprResult;
            }

            rightValue = exprResult.getResultBool();
        } else if (rightOperandType == NodeType::Id) {
            rightValue = EvaluateIdBool(static_cast<IdentifierNode*>(subtree->right));
        } else {
            result.error = newError(EvalErrorCode::INVALID_VALUE_TYPE);
            return result;
        }

        result.setValueBool(leftValue == rightValue);
    }

    return result;
}

EvalResult Evaluator::Evaluate(ASTNode* root) {
    EvalResult result;

    if (root->type == NodeType::BinOp) {
        BinOpNode* node = dynamic_cast<BinOpNode*>(root);

        if (node != nullptr) {
            if (node->binOpType == BinOpType::OperatorAssign) {
                IdentifierNode* id = dynamic_cast<IdentifierNode*>(node->left);

                result = EvaluateAssignValue(id, node->right);
            } else if (node->binOpType == BinOpType::OperatorPlus ||
                       node->binOpType == BinOpType::OperatorMinus ||
                       node->binOpType == BinOpType::OperatorMul ||
                       node->binOpType == BinOpType::OperatorDiv) {
                result = EvaluateMathExpr(node);
            } else if (node->binOpType == BinOpType::OperatorBoolAND ||
                       node->binOpType == BinOpType::OperatorBoolOR ||
                       node->binOpType == BinOpType::OperatorEqual) {
                result = EvaluateBoolExpr(node);
            } else {
                result.error = newError(EvalErrorCode::INVALiD_BIN_OPERATION);
            }
        } else {
            result.error = newError(EvalErrorCode::INVALID_AST, "Invalid Binary Operation Node");
        }
    } else if (root->type == NodeType::DeclVar) {
        DeclVarNode* node = dynamic_cast<DeclVarNode*>(root);

        if (node != nullptr) {
            result = EvaluateDeclVar(node);
        } else {
            result.error = newError(EvalErrorCode::INVALID_AST, "Invalid Variable Declaration Node");
        }
    } else if (root->type == NodeType::NumberValue) {
        NumberNode* node = dynamic_cast<NumberNode*>(root);

        if (node != nullptr) {
            result.setValueDouble(EvaluateNumberValue(node));
        } else {
            result.error = newError(EvalErrorCode::INVALID_AST, "Invalid Number Constant Node");
        }
    } else if (root->type == NodeType::BoolValue) {
        BoolNode* node = dynamic_cast<BoolNode*>(root);

        if (node != nullptr) {
            result.setValueBool(EvaluateBoolValue(node));
        } else {
            result.error = newError(EvalErrorCode::INVALID_AST, "Invalid Bool Constant Node");
        }
    } else if (root->type == NodeType::Id) {
        IdentifierNode* node = dynamic_cast<IdentifierNode*>(root);

        if (node != nullptr) {
            if (symbolTable.isIdExist(node->name)) {
                int idType = symbolTable.getIdType(node->name);

                switch (idType) {
                    case IdentifierValueType::Number: {
                        result.setValueDouble(EvaluateIdDouble(node));
                        break;
                    }
                    case IdentifierValueType::Bool: {
                        result.setValueBool(EvaluateIdBool(node));
                        break;
                    }
                    case IdentifierValueType::Undefined: {
                        result.error = newError(EvalErrorCode::UNINITIALIZED_VAR,
                                                "Use of uninitialized identifier '" + node->name + "'");
                        break;
                    }
                    default: {
                        result.error = newError(EvalErrorCode::INVALID_VALUE_TYPE);
                    }
                }
            } else {
                result.error = newError(EvalErrorCode::UNDECLARED_VAR,
                                        "Use of undeclared identifier '" + node->name + "'");
            }
        } else {
            result.error = newError(EvalErrorCode::INVALID_AST, "Invalid Identifier Node");
        }
    } else {
        result.error = newError(EvalErrorCode::INVALID_AST);
    }

    return result;
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

EvalError Evaluator::newError(EvalErrorCode::Error err) {
    return EvalError{err};
}

EvalError Evaluator::newError(EvalErrorCode::Error err, const std::string errMessage) {
    return EvalError{err, errMessage};
}
