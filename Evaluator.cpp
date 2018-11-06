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

            if ((curScope = lookTopIdScope(id->name))) {
                ValueType::Type idType = curScope->symbolTable.getIdValueType(id->name);

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
                        break;
                    }
                }
            } else {
                result.error = newError(EvalError::UNDECLARED_VAR,
                                        "Use of undeclared identifier '" + id->name + "'");
            }
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid Identifier Node");
        }
    } else if (subtree->type == NodeType::FuncCall) {
        FuncCallNode* node = dynamic_cast<FuncCallNode*>(subtree);

        if (node != nullptr) {
            const EvalResult& funcCallResult = EvaluateFuncCall(node);

            if (!funcCallResult.isError()) {
                if (funcCallResult.getResultType() == ValueType::Number) {
                    result = funcCallResult;
                } else {
                    result.error = newError(EvalError::INCOMPATIBLE_OPERAND_TYPES);
                }
            } else {
                return funcCallResult;
            }
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid Function Call Node");
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
                    result.error = newError(EvalError::INVALID_OPERATION);
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

            if ((curScope = lookTopIdScope(id->name))) {
                ValueType::Type idType = curScope->symbolTable.getIdValueType(id->name);

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
    } else if (subtree->type == NodeType::FuncCall) {
        FuncCallNode* node = dynamic_cast<FuncCallNode*>(subtree);

        if (node != nullptr) {
            const EvalResult& funcCallResult = EvaluateFuncCall(node);

            if (!funcCallResult.isError()) {
                if (funcCallResult.getResultType() == ValueType::Bool) {
                    result = funcCallResult;
                } else {
                    result.error = newError(EvalError::INCOMPATIBLE_OPERAND_TYPES);
                }
            } else {
                return funcCallResult;
            }
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid Function Call Node");
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

    Scope* lhsIdScope;

    if ((lhsIdScope = lookTopIdScope(id->name))) {
        BinOpNode* binOpExpr = dynamic_cast<BinOpNode*>(expr);
        IdentifierNode* idExpr = dynamic_cast<IdentifierNode*>(expr);
        NumberNode* numberConst = dynamic_cast<NumberNode*>(expr);
        BoolNode* boolConst = dynamic_cast<BoolNode*>(expr);
        FuncCallNode* funcCallExpr = dynamic_cast<FuncCallNode*>(expr);

        ValueType::Type lhsIdType = lhsIdScope->symbolTable.getIdValueType(id->name);

        if (binOpExpr != nullptr) {
            if (binOpExpr->binOpType == BinOpType::OperatorPlus ||
                binOpExpr->binOpType == BinOpType::OperatorMinus ||
                binOpExpr->binOpType == BinOpType::OperatorMul ||
                binOpExpr->binOpType == BinOpType::OperatorDiv) {
                EvalResult exprResult = EvaluateMathExpr(binOpExpr);

                if (!exprResult.isError()) {
                    if (lhsIdType == ValueType::Number || lhsIdType == ValueType::Undefined) {
                        lhsIdScope->symbolTable.setIdValueDouble(id->name, exprResult.getResultDouble());
                        result.setValueString("Assign value");
                    } else {
                        result.error = newError(EvalError::INVALID_VALUE_TYPE, "Invalid RHS Value type");
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
                    if (lhsIdType == ValueType::Bool || lhsIdType == ValueType::Undefined) {
                        lhsIdScope->symbolTable.setIdValueBool(id->name, exprResult.getResultBool());
                        result.setValueString("Assign value");
                    } else {
                        result.error = newError(EvalError::INVALID_VALUE_TYPE, "Invalid RHS Value type");
                    }
                } else {
                    return exprResult;
                }
            } else {
                result.error = newError(EvalError::INVALID_OPERATION, "Invalid binary operation");
            }
        } else if (idExpr != nullptr) {
            Scope* rhsIdScope;

            if ((rhsIdScope = lookTopIdScope(idExpr->name))) { // check if id on right hand side is exist
                ValueType::Type rhsIdType = rhsIdScope->symbolTable.getIdValueType(idExpr->name);

                if (rhsIdType == ValueType::Number) {
                    if (lhsIdType == ValueType::Number || lhsIdType == ValueType::Undefined) {
                        lhsIdScope->symbolTable.setIdValueDouble(id->name, EvaluateIdDouble(rhsIdScope, idExpr));
                        result.setValueString("Assign value");
                    } else {
                        result.error = newError(EvalError::INVALID_VALUE_TYPE, "Invalid RHS Value type");
                    }
                } else if (rhsIdType == ValueType::Bool) {
                    if (lhsIdType == ValueType::Bool || lhsIdType == ValueType::Undefined) {
                        lhsIdScope->symbolTable.setIdValueBool(id->name, EvaluateIdBool(rhsIdScope, idExpr));
                        result.setValueString("Assign value");
                    } else {
                        result.error = newError(EvalError::INVALID_VALUE_TYPE, "Invalid RHS Value type");
                    }
                } else if (rhsIdType == ValueType::Undefined) {
                    result.error = newError(EvalError::UNINITIALIZED_VAR,
                                            "Use of uninitialized identifier '" + idExpr->name + "'");
                } else {
                    result.error = newError(EvalError::INVALID_VALUE_TYPE, "Invalid RHS Value type");
                }
            } else {
                result.error = newError(EvalError::UNDECLARED_VAR,
                                        "Use of undeclared identifier '" + idExpr->name + "'");
            }
        } else if (funcCallExpr != nullptr) {
            const EvalResult& funcCallResult = EvaluateFuncCall(funcCallExpr);

            if (!funcCallResult.isError()) {
                switch (funcCallResult.getResultType()) {
                    case ValueType::Number: {
                        if (lhsIdType == ValueType::Number || lhsIdType == ValueType::Undefined) {
                            lhsIdScope->symbolTable.setIdValueDouble(id->name, funcCallResult.getResultDouble());
                            result.setValueString("Assign value");
                        } else {
                            result.error = newError(EvalError::INVALID_VALUE_TYPE, "Invalid RHS Value type");
                        }
                        break;
                    }
                    case ValueType::Bool: {
                        if (lhsIdType == ValueType::Bool || lhsIdType == ValueType::Undefined) {
                            lhsIdScope->symbolTable.setIdValueBool(id->name, funcCallResult.getResultBool());
                            result.setValueString("Assign value");
                        } else {
                            result.error = newError(EvalError::INVALID_VALUE_TYPE, "Invalid RHS Value type");
                        }
                        break;
                    }
                    default: {
                        result.error = newError(EvalError::INVALID_VALUE_TYPE, "Invalid RHS Value type");
                        return result;
                    }
                }
            } else {
                return funcCallResult;
            }
        } else if (numberConst != nullptr) {
            if (lhsIdType == ValueType::Number || lhsIdType == ValueType::Undefined) {
                lhsIdScope->symbolTable.setIdValueDouble(id->name, EvaluateNumberConstant(numberConst));
                result.setValueString("Assign value");
            } else {
                result.error = newError(EvalError::INVALID_VALUE_TYPE, "Invalid RHS Value type");
            }
        } else if (boolConst != nullptr) {
            if (lhsIdType == ValueType::Bool || lhsIdType == ValueType::Undefined) {
                lhsIdScope->symbolTable.setIdValueBool(id->name, EvaluateBoolConstant(boolConst));
                result.setValueString("Assign value");
            } else {
                result.error = newError(EvalError::INVALID_VALUE_TYPE, "Invalid RHS Value type");
            }
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid expression on RHS");
        }
    } else {
        result.error = newError(EvalError::UNDECLARED_VAR,
                                "Can't assign to undeclared variable '" + id->name + "'");
    }

    return result;
}

EvalResult Evaluator::EvaluateReturnStmt(ReturnValueNode* subtree) {
    EvalResult result;

    if (subtree->expression != nullptr) {
        result = Evaluate(subtree->expression);
    } else {
        result.setVoidResult();
    }
    return result;
}

EvalResult Evaluator::EvaluateFuncCall(FuncCallNode* funcCall) {
    EvalResult result;

    const std::string& funcName = funcCall->name;

    if (functions->symbolTable.isFuncExist(funcName)) {
        const Function& func = functions->symbolTable.getFunc(funcName);

        // evaluate parameters
        std::vector<EvalResult> callParamsValues;
        for (const auto& currentParam : funcCall->args) {
            const EvalResult& currentParamValue = Evaluate(currentParam);
            if (currentParamValue.isError()) {
                return currentParamValue;
            }
            callParamsValues.emplace_back(currentParamValue);
        }

        openScope();

        if (funcCall->argsSize == func.argsSize) {
            for (unsigned long currentId = 0; currentId != func.argsSize; currentId++) {
                const std::string declVar = func.args[currentId];
                const EvalResult& callParamValue = callParamsValues[currentId];

                // add param to function scope
                topScope->symbolTable.addNewIdentifier(declVar);
                switch (callParamValue.getResultType()) {
                    case ValueType::Number: {
                        topScope->symbolTable.setIdValueDouble(declVar, callParamValue.getResultDouble());
                        break;
                    }
                    case ValueType::Bool: {
                        topScope->symbolTable.setIdValueBool(declVar, callParamValue.getResultBool());
                        break;
                    }
                    default: {
                        result.error = newError(EvalError::INVALID_VALUE_TYPE, "Invalid RHS Value type");
                        closeScope();
                        return result;
                    }
                }
            }
        } else {
            result.error = newError(EvalError::NO_MATCHING_FUNC);
            closeScope();
            return result;
        }

        // since function can see variables in its own scope and also in global scope,
        // make outer scope is global scope,
        // so the func lookTopIdScope() will look for variables only in function scope and in global scope
        Scope* oldOuterScope = topScope->outer;
        topScope->outer = globalScope;

        bool oldFuncBodyEval = funcBodyEval;
        funcBodyEval = true;
        result = EvaluateBlockStmt(func.body);
        funcBodyEval = oldFuncBodyEval;

        if (!result.isError() && result.getResultType() != func.returnType) {
            result.error = newError(EvalError::INVALID_RETURN);
        }

        topScope->outer = oldOuterScope;
        closeScope();
    } else {
        result.error = newError(EvalError::UNDECLARED_FUNC, "Use of undeclared function '" + funcName + "'");
    }

    return result;
}

EvalResult Evaluator::EvaluateDeclFunc(DeclFuncNode* subtree) {
    EvalResult result;

    const std::string& funcName = subtree->name;

    if (topScope == globalScope) { // func can be declared only in global scope
        if (!functions->symbolTable.isFuncExist(funcName)) {
            // check for not allowed statements in function body
            for (const auto& currentStatement : subtree->body->stmtList) {
                if (currentStatement->type == NodeType::DeclFunc) {

                    DeclFuncNode* declFuncNode = static_cast<DeclFuncNode*>(currentStatement);
                    result.error = newError(EvalError::FUNC_DEFINITION_IS_NOT_ALLOWED,
                                            "Definition of function '" + declFuncNode->name +
                                            "' is not allowed here");
                    return result;
                } else if (currentStatement->type == NodeType::ReturnValue) {
                    ReturnValueNode* returnValueNode = static_cast<ReturnValueNode*>(currentStatement);
                    if (subtree->returnType == ValueType::Void && returnValueNode->expression != nullptr) {
                        result.error = newError(EvalError::INVALID_RETURN, "Void function can't return values");
                        return result;
                    }
                }
            }

            functions->symbolTable.addNewFunc(subtree);
        } else {
            result.error = newError(EvalError::FUNC_REDEFINITION, "Redefinition of function '" + funcName + "'");
        }
    } else {
        result.error = newError(EvalError::FUNC_DEFINITION_IS_NOT_ALLOWED,
                                "Definition of function '" + funcName + "' is not allowed here");
        return result;
    }

    result.setValueString("Declare func");
    return result;
}

EvalResult Evaluator::EvaluateDeclVar(DeclVarNode* subtree) {
    EvalResult result;

    if (subtree->id != nullptr) {
        std::string idName = subtree->id->name;

        if (!topScope->symbolTable.isIdExist(idName)) {
            if (subtree->expr != nullptr) {
                const EvalResult& exprResult = Evaluate(subtree->expr);
                if (exprResult.isError()) {
                    return exprResult;
                }

                switch (exprResult.getResultType()) {
                    case ValueType::Number: {
                        topScope->symbolTable.addNewIdentifier(idName);
                        topScope->symbolTable.setIdValueDouble(idName, exprResult.getResultDouble());
                        break;
                    }
                    case ValueType::Bool: {
                        topScope->symbolTable.addNewIdentifier(idName);
                        topScope->symbolTable.setIdValueBool(idName, exprResult.getResultBool());
                        break;
                    }
                    default: {
                        result.error = newError(EvalError::INVALID_VALUE_TYPE, "Invalid RHS Value type");
                        return result;
                    }
                }
            } else {
                topScope->symbolTable.addNewIdentifier(idName);
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

    ValueType::Type operationValueType = leftValue.getResultType();

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

EvalResult Evaluator::EvaluateBlockStmt(BlockStmtNode* subtree) {
    EvalResult result;
    EvalResult returnStmt;

    std::vector<EvalResult> stmtResults;
    for (auto currentStatement : subtree->stmtList) {
        const EvalResult& currentResult = Evaluate(currentStatement);
        if (currentResult.isError()) {
            return currentResult;
        }

        if (currentStatement->type == NodeType::ReturnValue) {
            if (funcBodyEval) {
                return currentResult;
            } else {
                result.error = newError(EvalError::INVALID_OPERATION, "Return statement allowed only in functions");
                return result;
            }
        }

        stmtResults.emplace_back(currentResult);
    }

    if (funcBodyEval) {
        result.setVoidResult();
    } else {
        result.setBlockResult(stmtResults);
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

    if (conditionResult.getResultBool()) {
        result = EvaluateBlockStmt(subtree->body);
    } else {
        if (subtree->elseBody) {
            result = EvaluateBlockStmt(subtree->elseBody);
        } else {
            result.setBlockResult(std::vector<EvalResult>{});
        }
    }

    closeScope();

    return result;
}

EvalResult Evaluator::EvaluateForLoopStmt(ForLoopNode* subtree) {
    EvalResult result;

    openScope();

    if (subtree->init != nullptr) {
        const EvalResult& initResult = Evaluate(subtree->init);
        if (initResult.isError()) {
            closeScope();
            return initResult;
        }
    }

    std::vector<EvalResult> blockStmtResults;

    EvalResult conditionResult;
    while (subtree->condition == nullptr ||
           (!(conditionResult = EvaluateBoolExpr(subtree->condition)).isError() &&
            conditionResult.getResultBool())) {
        EvalResult currentBlockResult = EvaluateBlockStmt(subtree->body);
        blockStmtResults.emplace_back(currentBlockResult);

        if (subtree->inc != nullptr) {
            const EvalResult& increaseResult = Evaluate(subtree->inc);
            if (increaseResult.isError()) {
                closeScope();
                return increaseResult;
            }
        }
    }
    if (subtree->condition != nullptr && conditionResult.isError()) {
        closeScope();
        return conditionResult;
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
                if (id != nullptr) {
                    result = EvaluateAssignValue(id, node->right);
                } else {
                    result.error = newError(EvalError::INVALID_LVALUE);
                }
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
            if ((curScope = lookTopIdScope(id->name))) {
                ValueType::Type idValueType = curScope->symbolTable.getIdValueType(id->name);

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
    } else if (root->type == NodeType::ForLoop) {
        ForLoopNode* node = dynamic_cast<ForLoopNode*>(root);

        if (node != nullptr) {
            result = EvaluateForLoopStmt(node);
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid For Loop Statement Node");
        }
    } else if (root->type == NodeType::DeclFunc) {
        DeclFuncNode* node = dynamic_cast<DeclFuncNode*>(root);

        if (node != nullptr) {
            result = EvaluateDeclFunc(node);
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid Function Declaration Node");
        }
    } else if (root->type == NodeType::FuncCall) {
        FuncCallNode* node = dynamic_cast<FuncCallNode*>(root);

        if (node != nullptr) {
            result = EvaluateFuncCall(node);
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid Function Call Node");
        }
    } else if (root->type == NodeType::ReturnValue) {
        ReturnValueNode* node = dynamic_cast<ReturnValueNode*>(root);

        if (node != nullptr) {
            result = EvaluateReturnStmt(node);
        } else {
            result.error = newError(EvalError::INVALID_AST, "Invalid Return Value Node");
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
    Scope* oldScope = topScope;
    topScope = topScope->outer;
    delete oldScope;
}

Evaluator::Scope* Evaluator::lookTopIdScope(const std::string& idName) {
    Scope* currentScope = topScope;

    while (currentScope != nullptr) {
        if (currentScope->symbolTable.isIdExist(idName)) {
            return currentScope;
        }
        currentScope = currentScope->outer;
    }

    return nullptr;
}