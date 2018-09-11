#include "Evaluator.h"

EvalResult Evaluator::EvaluateMathExpr(ASTNode* subtree) {
    EvalResult result;
    if (subtree->type == NodeType::NumberValue) {
        NumberNode* node = dynamic_cast<NumberNode*>(subtree);

        if (node != nullptr) {
            result.setValueDouble(EvaluateNumberConstant(node));
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid Number Constant Node");
        }
    } else if (subtree->type == NodeType::BoolValue) {
        result.error = newError(EvalError::INCOMPATIBLE_OPERAND_TYPES);
    } else if (subtree->type == NodeType::Id) {
        IdentifierNode* node = dynamic_cast<IdentifierNode*>(subtree);

        if (node != nullptr) {
            if (symbolTable.isIdExist(node->name)) {
                IdentifierValueType::ValueType idType = symbolTable.getIdType(node->name);

                switch (idType) {
                    case IdentifierValueType::Number: {
                        result.setValueDouble(EvaluateIdDouble(node));
                        break;
                    }
                    case IdentifierValueType::Undefined: {
                        result.error = newError(EvalError::UNINITIALIZED_VAR,
                                                "Use of uninitialized identifier '" + node->name + "'");
                        break;
                    }
                    default: {
                        result.error = newError(EvalError::INCOMPATIBLE_OPERAND_TYPES);
                    }
                }
            } else {
                result.error = newError(EvalError::UNDECLARED_VAR,
                                        "Use of undeclared identifier '" + node->name + "'");
            }
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid Identifier Node");
        }
    } else if (subtree->type == NodeType::BinOp) {
        BinOpNode* node = dynamic_cast<BinOpNode*>(subtree);

        if (node != nullptr) {
            EvalResult leftValue = EvaluateMathExpr(node->left);
            if (leftValue.isError()) {
                return leftValue;
            }
            EvalResult rightValue = EvaluateMathExpr(node->right);
            if (rightValue.isError()) {
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
                    result.error = newError(EvalError::INCOMPATIBLE_OPERAND_TYPES);
            }
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid Binary Operation Node");
        }
    } else {
        result.error = newError(EvalError::INVALID_AST);
    }

    return result;
}

EvalResult Evaluator::EvaluateBoolExpr(ASTNode* subtree) {
    EvalResult result;

    if (subtree->type == NodeType::BoolValue) {
        BoolNode* node = dynamic_cast<BoolNode*>(subtree);

        if (node != nullptr) {
            result.setValueBool(EvaluateBoolConstant(node));
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid Bool Constant Node");
        }
    } else if (subtree->type == NodeType::NumberValue) {
        result.error = newError(EvalError::INCOMPATIBLE_OPERAND_TYPES);
    } else if (subtree->type == NodeType::Id) {
        IdentifierNode* node = dynamic_cast<IdentifierNode*>(subtree);

        if (node != nullptr) {
            if (symbolTable.isIdExist(node->name)) {
                IdentifierValueType::ValueType idType = symbolTable.getIdType(node->name);

                if (idType == IdentifierValueType::Bool) {
                    result.setValueBool(EvaluateIdBool(node));
                } else if (idType == IdentifierValueType::Undefined) {
                    result.error = newError(EvalError::UNINITIALIZED_VAR,
                                            "Use of uninitialized identifier '" + node->name + "'");
                } else {
                    result.error = newError(EvalError::INCOMPATIBLE_OPERAND_TYPES);
                }
            } else {
                result.error = newError(EvalError::UNDECLARED_VAR,
                                        "Use of undeclared identifier '" + node->name + "'");
            }
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid Identifier Node");
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
            if (rightValue.isError()) {
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
                    result.error = newError(EvalError::INCOMPATIBLE_OPERAND_TYPES);
            }
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid Binary Operation Node");
        }
    } else {
        result.error = newError(EvalError::INVALID_AST);
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
                        try {
                            symbolTable.setIdValueDouble(lvalue->name, exprResult.getResultDouble());
                            result.setValueString("Assign value");
                        } catch (std::runtime_error e) {
                            result.error = newError(EvalError::INVALID_VALUE_TYPE, e.what());
                        }
                    } else {
                        return exprResult;
                    }
                } else if (binOpExpr->binOpType == BinOpType::OperatorBoolAND ||
                           binOpExpr->binOpType == BinOpType::OperatorBoolOR ||
                           binOpExpr->binOpType == BinOpType::OperatorEqual) {
                    EvalResult exprResult = EvaluateBoolExpr(binOpExpr);

                    if (!exprResult.isError()) {
                        try {
                            symbolTable.setIdValueBool(lvalue->name, exprResult.getResultBool());
                            result.setValueString("Assign value");
                        } catch (std::runtime_error e) {
                            result.error = newError(EvalError::INVALID_VALUE_TYPE, e.what());
                        }
                    } else {
                        return exprResult;
                    }
                } else {
                    result.error = newError(EvalError::INVALID_OPERATION, "Unknown or not allowed binary operation");
                }
            } else if (idExpr != nullptr) {
                if (symbolTable.isIdExist(idExpr->name)) {

                    int idExprType = symbolTable.getIdType(idExpr->name);

                    if (idExprType == IdentifierValueType::Number) {
                        try {
                            symbolTable.setIdValueDouble(lvalue->name, EvaluateIdDouble(idExpr));
                            result.setValueString("Assign value");
                        } catch (std::runtime_error e) {
                            result.error = newError(EvalError::INVALID_VALUE_TYPE, e.what());
                        }
                    } else if (idExprType == IdentifierValueType::Bool) {
                        try {
                            symbolTable.setIdValueBool(lvalue->name, EvaluateIdBool(idExpr));
                            result.setValueString("Assign value");
                        } catch (std::runtime_error e) {
                            result.error = newError(EvalError::INVALID_VALUE_TYPE, e.what());
                        }
                    } else if (idExprType == IdentifierValueType::Undefined) {
                        result.error = newError(EvalError::UNINITIALIZED_VAR,
                                                "Use of uninitialized identifier '" + idExpr->name + "'");
                    } else {
                        result.error = newError(EvalError::INVALID_VALUE_TYPE);
                    }
                } else {
                    result.error = newError(EvalError::UNDECLARED_VAR,
                                            "Use of undeclared identifier '" + idExpr->name + "'");
                }
            } else if (numberConst != nullptr) {
                try {
                    symbolTable.setIdValueDouble(lvalue->name, EvaluateNumberConstant(numberConst));
                    result.setValueString("Assign value");
                } catch (std::runtime_error e) {
                    result.error = newError(EvalError::INVALID_VALUE_TYPE, e.what());
                }
            } else if (boolConst != nullptr) {
                try {
                    symbolTable.setIdValueBool(lvalue->name, EvaluateBoolConstant(boolConst));
                    result.setValueString("Assign value");
                } catch (std::runtime_error e) {
                    result.error = newError(EvalError::INVALID_VALUE_TYPE, e.what());
                }
            } else {
                result.error = newError(EvalError::INVALID_AST, "Invalid expression in assignment statement");
            }
        } else {
            result.error = newError(EvalError::UNDECLARED_VAR, "Assignment to undeclared variable");
        }
    } else {
        result.error = newError(EvalError::INVALID_LVALUE, "Invalid Lvalue in assignment statement");
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
            result.error = newError(EvalError::VAR_REDEFINITION, "Redefinition of variable '" + idName + "'");
        }
    } else {
        result.error = newError(EvalError::INVALID_AST);
    }

    return result;
}

EvalResult Evaluator::EvaluateEqual(BinOpNode* subtree) {
    EvalResult result;

    EvalResult leftValue = Evaluate(subtree->left);
    if (leftValue.isError()) {
        return leftValue;
    }

    EvalResult rightValue = Evaluate(subtree->right);
    if (rightValue.isError()) {
        return rightValue;
    }

    if (leftValue.getResultType() != rightValue.getResultType()) {
        result.error = newError(EvalError::INCOMPATIBLE_OPERAND_TYPES);
        return result;
    }

    IdentifierValueType::ValueType operationValueType = leftValue.getResultType();

    if (operationValueType == IdentifierValueType::Number) {
        result.setValueBool(leftValue.getResultDouble() == rightValue.getResultDouble());
    } else {
        result.setValueBool(leftValue.getResultBool() == rightValue.getResultBool());
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
                result.error = newError(EvalError::INVALID_OPERATION, "Unknown or not allowed binary operation");
            }
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid Binary Operation Node");
        }
    } else if (root->type == NodeType::DeclVar) {
        DeclVarNode* node = dynamic_cast<DeclVarNode*>(root);

        if (node != nullptr) {
            result = EvaluateDeclVar(node);
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid Variable Declaration Node");
        }
    } else if (root->type == NodeType::NumberValue) {
        NumberNode* node = dynamic_cast<NumberNode*>(root);

        if (node != nullptr) {
            result.setValueDouble(EvaluateNumberConstant(node));
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid Number Constant Node");
        }
    } else if (root->type == NodeType::BoolValue) {
        BoolNode* node = dynamic_cast<BoolNode*>(root);

        if (node != nullptr) {
            result.setValueBool(EvaluateBoolConstant(node));
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid Bool Constant Node");
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
                        result.error = newError(EvalError::UNINITIALIZED_VAR,
                                                "Use of uninitialized identifier '" + node->name + "'");
                        break;
                    }
                    default: {
                        result.error = newError(EvalError::INVALID_VALUE_TYPE);
                    }
                }
            } else {
                result.error = newError(EvalError::UNDECLARED_VAR,
                                        "Use of undeclared identifier '" + node->name + "'");
            }
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid Identifier Node");
        }
    } else {
        result.error = newError(EvalError::INVALID_AST);
    }

    return result;
}

double Evaluator::EvaluateIdDouble(IdentifierNode* id) {
    return symbolTable.getIdValueDouble(id->name);
}

bool Evaluator::EvaluateIdBool(IdentifierNode* id) {
    return symbolTable.getIdValueBool(id->name);
}

double Evaluator::EvaluateNumberConstant(NumberNode* num) {
    return num->value;
}

bool Evaluator::EvaluateBoolConstant(BoolNode* num) {
    return num->value;
}

EvalError Evaluator::newError(EvalError::Error err) {
    return EvalError{err};
}

EvalError Evaluator::newError(EvalError::Error err, const std::string errMessage) {
    return EvalError{err, errMessage};
}