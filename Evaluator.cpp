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
        IdentifierNode* id = dynamic_cast<IdentifierNode*>(subtree);

        if (id != nullptr) {
            Scope* curScope;

            if ((curScope = lookTopId(id->name))) {
                ValueType::T idType = curScope->symbolTable.getIdValueType(id->name);

                switch (idType) {
                    case ValueType::Number: {
                        result.setValueDouble(EvaluateIdDouble(curScope, id));
                        break;
                    }
                    case ValueType::Undefined: {
                        result.error = newError(EvalError::UNINITIALIZED_VAR,
                                                "Use of uninitialized identifier '" + id->name + "'");
                        break;
                    }
                    default: {
                        result.error = newError(EvalError::INCOMPATIBLE_OPERAND_TYPES);
                    }
                }
            } else {
                result.error = newError(EvalError::UNDECLARED_VAR,
                                        "Use of undeclared identifier '" + id->name + "'");
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
        IdentifierNode* id = dynamic_cast<IdentifierNode*>(subtree);

        if (id != nullptr) {
            Scope* curScope;

            if ((curScope = lookTopId(id->name))) {
                ValueType::T idType = curScope->symbolTable.getIdValueType(id->name);

                if (idType == ValueType::Bool) {
                    result.setValueBool(EvaluateIdBool(curScope, id));
                } else if (idType == ValueType::Undefined) {
                    result.error = newError(EvalError::UNINITIALIZED_VAR,
                                            "Use of uninitialized identifier '" + id->name + "'");
                } else {
                    result.error = newError(EvalError::INCOMPATIBLE_OPERAND_TYPES);
                }
            } else {
                result.error = newError(EvalError::UNDECLARED_VAR,
                                        "Use of undeclared identifier '" + id->name + "'");
            }
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid Identifier Node");
        }
    } else if (subtree->type == NodeType::BinOp) {
        BinOpNode* binOp = dynamic_cast<BinOpNode*>(subtree);

        if (binOp != nullptr) {
            if (binOp->binOpType == BinOpType::OperatorEqual) {
                return EvaluateEqual(binOp);
            } else if (binOp->binOpType == BinOpType::OperatorLess || binOp->binOpType == BinOpType::OperatorGreater) {
                return EvaluateComparison(binOp);
            }

            EvalResult leftValue = EvaluateBoolExpr(binOp->left);
            if (leftValue.isError()) {
                return leftValue;
            }
            EvalResult rightValue = EvaluateBoolExpr(binOp->right);
            if (rightValue.isError()) {
                return rightValue;
            }

            switch (binOp->binOpType) {
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

EvalResult Evaluator::EvaluateAssignValue(IdentifierNode* id, ASTNode* expr) {
    EvalResult result;

    if (id != nullptr) {
        Scope* curScope;

        if ((curScope = lookTopId(id->name))) {
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
                            curScope->symbolTable.setIdValueDouble(id->name, exprResult.getResultDouble());
                            result.setValueString("Assign value");
                        } catch (std::runtime_error e) {
                            result.error = newError(EvalError::INVALID_VALUE_TYPE, e.what());
                        }
                    } else {
                        return exprResult;
                    }
                } else if (binOpExpr->binOpType == BinOpType::OperatorBoolAND ||
                           binOpExpr->binOpType == BinOpType::OperatorBoolOR ||
                           binOpExpr->binOpType == BinOpType::OperatorEqual ||
                           binOpExpr->binOpType == BinOpType::OperatorLess ||
                           binOpExpr->binOpType == BinOpType::OperatorGreater) {
                    EvalResult exprResult = EvaluateBoolExpr(binOpExpr);

                    if (!exprResult.isError()) {
                        try {
                            curScope->symbolTable.setIdValueBool(id->name, exprResult.getResultBool());
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
                Scope* rhsIdScope;

                if ((rhsIdScope = lookTopId(idExpr->name))) {
                    int idExprType = rhsIdScope->symbolTable.getIdValueType(idExpr->name);

                    if (idExprType == ValueType::Number) {
                        try {
                            rhsIdScope->symbolTable.setIdValueDouble(id->name, EvaluateIdDouble(rhsIdScope, idExpr));
                            result.setValueString("Assign value");
                        } catch (std::runtime_error e) {
                            result.error = newError(EvalError::INVALID_VALUE_TYPE, e.what());
                        }
                    } else if (idExprType == ValueType::Bool) {
                        try {
                            rhsIdScope->symbolTable.setIdValueBool(id->name, EvaluateIdBool(rhsIdScope, idExpr));
                            result.setValueString("Assign value");
                        } catch (std::runtime_error e) {
                            result.error = newError(EvalError::INVALID_VALUE_TYPE, e.what());
                        }
                    } else if (idExprType == ValueType::Undefined) {
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
                    curScope->symbolTable.setIdValueDouble(id->name, EvaluateNumberConstant(numberConst));
                    result.setValueString("Assign value");
                } catch (std::runtime_error e) {
                    result.error = newError(EvalError::INVALID_VALUE_TYPE, e.what());
                }
            } else if (boolConst != nullptr) {
                try {
                    curScope->symbolTable.setIdValueBool(id->name, EvaluateBoolConstant(boolConst));
                    result.setValueString("Assign value");
                } catch (std::runtime_error e) {
                    result.error = newError(EvalError::INVALID_VALUE_TYPE, e.what());
                }
            } else {
                result.error = newError(EvalError::INVALID_AST, "Invalid expression in assignment statement");
            }
        } else {
            result.error = newError(EvalError::UNDECLARED_VAR,
                                    "Assignment to undeclared variable '" + id->name + "'");
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

        if (!topScope->symbolTable.isIdExist(idName)) {
            topScope->symbolTable.addNewIdentifier(idName);

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

    ValueType::T operationValueType = leftValue.getResultType();

    if (operationValueType == ValueType::Number) {
        result.setValueBool(leftValue.getResultDouble() == rightValue.getResultDouble());
    } else {
        result.setValueBool(leftValue.getResultBool() == rightValue.getResultBool());
    }

    return result;
}

EvalResult Evaluator::EvaluateComparison(BinOpNode* subtree) {
    EvalResult result;

    EvalResult leftValue = Evaluate(subtree->left);
    if (leftValue.isError()) {
        return leftValue;
    }

    EvalResult rightValue = Evaluate(subtree->right);
    if (rightValue.isError()) {
        return rightValue;
    }

    if (leftValue.getResultType() != ValueType::Number ||
        rightValue.getResultType() != ValueType::Number) {
        result.error = newError(EvalError::INCOMPATIBLE_OPERAND_TYPES);
        return result;
    }

    if (subtree->binOpType == BinOpType::OperatorLess) {
        result.setValueBool(leftValue.getResultDouble() < rightValue.getResultDouble());
    } else {
        result.setValueBool(leftValue.getResultDouble() > rightValue.getResultDouble());
    }

    return result;
}

EvalResult Evaluator::EvaluateIfStmt(IfStmtNode* subtree) {
    EvalResult result;

    const EvalResult& conditionResult = EvaluateBoolExpr(subtree->condition);
    if (conditionResult.isError()) {
        result = conditionResult;
        return result;
    }

    openScope();

    std::vector<EvalResult> blockStmtResults;
    BlockStmtNode* blockStmt = subtree->body;
    if (conditionResult.getResultBool()) {
        for (auto currentStatement : blockStmt->stmtList) {
            blockStmtResults.emplace_back(Evaluate(currentStatement));
        }
    }
    result.setBlockResult(blockStmtResults);

    closeScope();

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
                       node->binOpType == BinOpType::OperatorEqual ||
                       node->binOpType == BinOpType::OperatorLess ||
                       node->binOpType == BinOpType::OperatorGreater) {
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
        IdentifierNode* id = dynamic_cast<IdentifierNode*>(root);

        if (id != nullptr) {
            Scope* curScope;
            if ((curScope = lookTopId(id->name))) {
                ValueType::T idValueType = curScope->symbolTable.getIdValueType(id->name);

                switch (idValueType) {
                    case ValueType::Number: {
                        result.setValueDouble(EvaluateIdDouble(curScope, id));
                        break;
                    }
                    case ValueType::Bool: {
                        result.setValueBool(EvaluateIdBool(curScope, id));
                        break;
                    }
                    case ValueType::Undefined: {
                        result.error = newError(EvalError::UNINITIALIZED_VAR,
                                                "Use of uninitialized identifier '" + id->name + "'");
                        break;
                    }
                    default: {
                        result.error = newError(EvalError::INVALID_VALUE_TYPE);
                    }
                }
            } else {
                result.error = newError(EvalError::UNDECLARED_VAR,
                                        "Use of undeclared identifier '" + id->name + "'");
            }
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid Identifier Node");
        }
    } else if (root->type == NodeType::IfStmt) {
        IfStmtNode* node = dynamic_cast<IfStmtNode*>(root);

        if (node != nullptr) {
            result = EvaluateIfStmt(node);
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid If Statement Node");
        }
    } else {
        result.error = newError(EvalError::INVALID_AST);
    }

    return result;
}

double Evaluator::EvaluateIdDouble(Scope* scope, IdentifierNode* id) {
    return scope->symbolTable.getIdValueDouble(id->name);
}

bool Evaluator::EvaluateIdBool(Scope* scope, IdentifierNode* id) {
    return scope->symbolTable.getIdValueBool(id->name);
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

void Evaluator::openScope() {
    topScope = new Scope(topScope);
}

void Evaluator::closeScope() {
    topScope = topScope->outer;
}

Evaluator::Scope* Evaluator::lookTopId(const std::string& idName) {
    Scope* currentScope = topScope;

    while (currentScope != nullptr) {
        if (currentScope->symbolTable.isIdExist(idName)) {
            return currentScope;
        }
        currentScope = currentScope->outer;
    }

    return nullptr;
}
