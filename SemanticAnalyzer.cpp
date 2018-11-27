#include "SemanticAnalyzer.h"

void SemanticAnalyzer::openScope() {
    topScope = new Scope(topScope);
}

void SemanticAnalyzer::closeScope() {
    Scope* oldScope = topScope;
    topScope = topScope->outer;
    delete oldScope;
}

SemanticAnalyzer::Scope* SemanticAnalyzer::lookTopIdScope(const std::string& idName) {
    Scope* currentScope = topScope;

    while (currentScope != nullptr) {
        if (currentScope->symbolTable.isIdExist(idName)) {
            return currentScope;
        }
        currentScope = currentScope->outer;
    }

    return nullptr;
}

SemanticAnalysisResult SemanticAnalyzer::newError(SemanticAnalysisResult::Error err) {
    return SemanticAnalysisResult(err);
}

SemanticAnalysisResult SemanticAnalyzer::newError(SemanticAnalysisResult::Error err, const std::string& message) {
    return SemanticAnalysisResult(err, message);
}

SemanticAnalysisResult SemanticAnalyzer::checkVarDecl(DeclVarNode* node) {
    std::string idName = node->id->name;

    if (topScope->symbolTable.isIdExist(idName)) {
        return newError(SemanticAnalysisResult::VAR_REDEFINITION, "Redefinition of variable '" + idName + "'");
    }

    if (node->expr != nullptr) {
        bool oldOperationCheck = operationCheck;
        operationCheck = true;
        SemanticAnalysisResult checkResult = checkStatement(node->expr);
        operationCheck = oldOperationCheck;
        if (checkResult.isError()) {
            return checkResult;
        }

        switch (node->expr->type) {
            case NodeType::ConstNumber: {
                double value = 0;
                topScope->symbolTable.addNewIdentifier(idName, value);
                break;
            }
            case NodeType::ConstBool: {
                bool value = false;
                topScope->symbolTable.addNewIdentifier(idName, value);
                break;
            }
            case NodeType::BinOp: {
                BinOpNode* binExpr = static_cast<BinOpNode*>(node->expr);
                if (binExpr->binOpType == BinOpType::OperatorPlus ||
                    binExpr->binOpType == BinOpType::OperatorMinus ||
                    binExpr->binOpType == BinOpType::OperatorMul ||
                    binExpr->binOpType == BinOpType::OperatorDiv) {
                    double value = 0;
                    topScope->symbolTable.addNewIdentifier(idName, value);
                } else if (binExpr->binOpType == BinOpType::OperatorBoolAND ||
                           binExpr->binOpType == BinOpType::OperatorBoolOR ||
                           binExpr->binOpType == BinOpType::OperatorEqual ||
                           binExpr->binOpType == BinOpType::OperatorLess ||
                           binExpr->binOpType == BinOpType::OperatorGreater) {
                    bool value = false;
                    topScope->symbolTable.addNewIdentifier(idName, value);
                } else {
                    return newError(SemanticAnalysisResult::INVALID_VALUE_TYPE,
                                    "Invalid RHS expression value type");
                }
                break;
            }
            case NodeType::Id: {
                IdentifierNode* rhsId = static_cast<IdentifierNode*>(node->expr);
                Scope* rhsIdScope = lookTopIdScope(rhsId->name);

                if (rhsIdScope->symbolTable.getIdValueType(rhsId->name) == ValueType::Number) {
                    double value = 0;
                    topScope->symbolTable.addNewIdentifier(idName, value);
                } else {
                    bool value = false;
                    topScope->symbolTable.addNewIdentifier(idName, value);
                }
                break;
            }
            case NodeType::FuncCall: {
                FuncCallNode* funcCallNode = static_cast<FuncCallNode*>(node->expr);
                const std::string& funcName = funcCallNode->name;

                switch (functions->symbolTable.getFuncValueType(funcName)) {
                    case ValueType::Number: {
                        double value = 0;
                        topScope->symbolTable.addNewIdentifier(idName, value);
                        break;
                    }
                    case ValueType::Bool: {
                        bool value = false;
                        topScope->symbolTable.addNewIdentifier(idName, value);
                        break;
                    }
                    default: {
                        return newError(SemanticAnalysisResult::INVALID_VALUE_TYPE,
                                        "Invalid RHS expression value type");
                    }
                }
                break;
            }
            default: {
                return newError(SemanticAnalysisResult::INVALID_VALUE_TYPE,
                                "Invalid RHS expression value type");
            }
        }
    } else {
        topScope->symbolTable.addNewIdentifier(idName);
    }

    return SemanticAnalysisResult();
}

SemanticAnalysisResult SemanticAnalyzer::checkFuncDecl(DeclFuncNode* node) {
    if (topScope != globalScope) {
        return newError(SemanticAnalysisResult::FUNC_DEFINITION_IS_NOT_ALLOWED);
    }
    if (functions->symbolTable.isFuncExist(node->name)) {
        return newError(SemanticAnalysisResult::FUNC_REDEFINITION, "Redefinition of function '" + node->name + "'");
    }

    SemanticAnalysisResult checkResult;

    // since function can see variables in its own scope and also in global scope,
    // so make outer scope is global scope,
    openScope();
    Scope* oldOuterScope = topScope->outer;
    topScope->outer = globalScope;

    for (const auto& currentId : node->args) {
        if (currentId->valueType == ValueType::Number) {
            double value = 0;
            topScope->symbolTable.addNewIdentifier(currentId->name, value);
        } else if (currentId->valueType == ValueType::Bool) {
            bool value = 0;
            topScope->symbolTable.addNewIdentifier(currentId->name, value);
        }
    }

    bool oldFunctionBodyCheck = functionBodyCheck;
    functionBodyCheck = true;
    functionReturnType = node->returnType;

    for (ASTNode* currentStatement : node->body->stmtList) {
//         TODO: сделать проверку, что не void функция всегда возвращает значение
//         примерно это можно реализовать так:
//         функция всегда возвращает значение, если инструкция вне if ветвления
//         иначе надо проверить все ответвления и убедиться что во всех присутствует return инструкция
//         также, есть еще for loop который может не выполниться ни разу, не понятно как его проверять, такое должно
//         определиться в run-time скорее всего, и пока самое простое решение - потребовать, чтобы всегда возвращалось
//         какое-то значение из функций

        checkResult = checkStatement(currentStatement);
        if (checkResult.isError()) {
            break;
        }
    }

    functionBodyCheck = oldFunctionBodyCheck;
    topScope->outer = oldOuterScope;
    closeScope();

    if (!checkResult.isError()) {
        functions->symbolTable.addNewFunc(node);
    }

    return checkResult;
}

SemanticAnalysisResult SemanticAnalyzer::checkReservedFuncCall(FuncCallNode* node) {
    if (node->name == "print") {
        const std::string funcName = node->name;
        DeclFuncNode* func = functions->symbolTable.getFunc(funcName);
        if (func->argsSize != node->argsSize) {
            return newError(SemanticAnalysisResult::NO_MATCHING_FUNC);
        }

        for (const auto& callParam : node->args) {
            bool oldOperationCheck = operationCheck;
            operationCheck = true;
            SemanticAnalysisResult checkCallParamResult = checkStatement(callParam);
            operationCheck = oldOperationCheck;
            if (checkCallParamResult.isError()) {
                return checkCallParamResult;
            }

            ValueType::Type callParamType;

            if (callParam->type == NodeType::ConstNumber) {
                callParamType = ValueType::Number;
            } else if (callParam->type == NodeType::ConstBool) {
                callParamType = ValueType::Bool;
            } else if (callParam->type == NodeType::Id) {
                IdentifierNode* id = static_cast<IdentifierNode*>(callParam);
                Scope* idScope = lookTopIdScope(id->name);

                callParamType = idScope->symbolTable.getIdValueType(id->name);
            } else if (callParam->type == NodeType::FuncCall) {
                FuncCallNode* funcCallNode = static_cast<FuncCallNode*>(callParam);

                ValueType::Type funcValueType = functions->symbolTable.getFuncValueType(funcCallNode->name);
                if (funcValueType == ValueType::Void) {
                    return newError(SemanticAnalysisResult::INVALID_VALUE_TYPE,
                                    "Can not use void function call as function parameter");
                }
                callParamType = funcValueType;
            } else if (callParam->type == NodeType::BinOp) {
                BinOpNode* binOpNode = static_cast<BinOpNode*>(callParam);

                if (binOpNode->binOpType == BinOpType::OperatorPlus ||
                    binOpNode->binOpType == BinOpType::OperatorMinus ||
                    binOpNode->binOpType == BinOpType::OperatorMul ||
                    binOpNode->binOpType == BinOpType::OperatorDiv) {
                    callParamType = ValueType::Number;
                } else {
                    callParamType = ValueType::Bool;
                }
            } else {
                return newError(SemanticAnalysisResult::INVALID_VALUE_TYPE, "Invalid function call parameter");
            }

            if (callParamType != ValueType::Number && callParamType != ValueType::Bool) {
                return newError(SemanticAnalysisResult::INVALID_VALUE_TYPE,
                                "Print function can take only number or bool argument");
            }
        }
    }

    return SemanticAnalysisResult();
}

SemanticAnalysisResult SemanticAnalyzer::checkFuncCall(FuncCallNode* node) {
    const std::string& funcName = node->name;

    if (!functions->symbolTable.isFuncExist(funcName)) {
        return newError(SemanticAnalysisResult::UNDECLARED_FUNC,
                        "Use of undeclared function '" + funcName + "'");
    }
    if (isFuncReserved(funcName)) {
        return checkReservedFuncCall(node);
    }

    DeclFuncNode* func = functions->symbolTable.getFunc(funcName);
    if (func->argsSize != node->argsSize) {
        return newError(SemanticAnalysisResult::NO_MATCHING_FUNC);
    }

    for (unsigned long currentCallParamNum = 0; currentCallParamNum != node->argsSize; currentCallParamNum++) {
        ASTNode* callParam = node->args[currentCallParamNum];
        bool oldOperationCheck = operationCheck;
        operationCheck = true;
        SemanticAnalysisResult checkCallParamResult = checkStatement(callParam);
        operationCheck = oldOperationCheck;
        if (checkCallParamResult.isError()) {
            return checkCallParamResult;
        }

        ValueType::Type funcParamType = func->args[currentCallParamNum]->valueType;
        ValueType::Type callParamType;

        if (callParam->type == NodeType::ConstNumber) {
            callParamType = ValueType::Number;
        } else if (callParam->type == NodeType::ConstBool) {
            callParamType = ValueType::Bool;
        } else if (callParam->type == NodeType::Id) {
            IdentifierNode* id = static_cast<IdentifierNode*>(callParam);
            Scope* idScope = lookTopIdScope(id->name);

            callParamType = idScope->symbolTable.getIdValueType(id->name);
        } else if (callParam->type == NodeType::FuncCall) {
            FuncCallNode* funcCallNode = static_cast<FuncCallNode*>(callParam);

            ValueType::Type funcValueType = functions->symbolTable.getFuncValueType(funcCallNode->name);
            if (funcValueType == ValueType::Void) {
                return newError(SemanticAnalysisResult::INVALID_VALUE_TYPE,
                                "Can not use void function call as function parameter");
            }
            callParamType = funcValueType;
        } else if (callParam->type == NodeType::BinOp) {
            BinOpNode* binOpNode = static_cast<BinOpNode*>(callParam);

            if (binOpNode->binOpType == BinOpType::OperatorPlus ||
                binOpNode->binOpType == BinOpType::OperatorMinus ||
                binOpNode->binOpType == BinOpType::OperatorMul ||
                binOpNode->binOpType == BinOpType::OperatorDiv) {
                callParamType = ValueType::Number;
            } else {
                callParamType = ValueType::Bool;
            }
        } else {
            return newError(SemanticAnalysisResult::INVALID_VALUE_TYPE, "Invalid function call parameter");
        }

        if (callParamType != funcParamType) {
            return newError(SemanticAnalysisResult::INVALID_VALUE_TYPE,
                            "Value type of " + std::to_string(currentCallParamNum) +
                            "-th argument does not match with function parameter type");
        }
    }

    return SemanticAnalysisResult();
}

SemanticAnalysisResult SemanticAnalyzer::checkId(IdentifierNode* node) {
    const std::string& idName = node->name;

    Scope* idScope = lookTopIdScope(idName);
    if (idScope == nullptr) {
        return newError(SemanticAnalysisResult::UNDECLARED_VAR, "Use of undeclared variable '" + idName + "'");
    }

    ValueType::Type idValueType = idScope->symbolTable.getIdValueType(idName);
    if (idValueType != ValueType::Number && idValueType != ValueType::Bool) {
        if (idValueType == ValueType::Undefined) {
            return newError(SemanticAnalysisResult::UNINITIALIZED_VAR,
                            "Use of uninitialized variable '" + idName + "'");
        } else {
            return newError(SemanticAnalysisResult::INVALID_VALUE_TYPE,
                            "Invalid value type of variable '" + idName + "'");
        }
    }

    return SemanticAnalysisResult();
}

SemanticAnalysisResult SemanticAnalyzer::checkAssignExpr(BinOpNode* node) {
    IdentifierNode* id = dynamic_cast<IdentifierNode*>(node->left);
    if (id == nullptr) {
        return newError(SemanticAnalysisResult::INVALID_LVALUE);
    }

    const std::string& idName = id->name;
    Scope* idScope = lookTopIdScope(idName);
    if (idScope == nullptr) {
        return newError(SemanticAnalysisResult::UNDECLARED_VAR, "Use of undeclared variable '" + idName + "'");
    }

    bool oldOperationCheck = operationCheck;
    operationCheck = true;
    SemanticAnalysisResult exprCheckResult = checkStatement(node->right);
    operationCheck = oldOperationCheck;
    if (exprCheckResult.isError()) {
        return exprCheckResult;
    }

    BinOpNode* binOpExpr = dynamic_cast<BinOpNode*>(node->right);
    IdentifierNode* idExpr = dynamic_cast<IdentifierNode*>(node->right);
    ConstNumberNode* numberConst = dynamic_cast<ConstNumberNode*>(node->right);
    ConstBoolNode* boolConst = dynamic_cast<ConstBoolNode*>(node->right);
    FuncCallNode* funcCallExpr = dynamic_cast<FuncCallNode*>(node->right);

    ValueType::Type idValueType = idScope->symbolTable.getIdValueType(idName);
    ValueType::Type exprValueType;

    if (binOpExpr != nullptr) {
        if (binOpExpr->binOpType == BinOpType::OperatorPlus ||
            binOpExpr->binOpType == BinOpType::OperatorMinus ||
            binOpExpr->binOpType == BinOpType::OperatorMul ||
            binOpExpr->binOpType == BinOpType::OperatorDiv) {
            exprValueType = ValueType::Number;
        } else {
            exprValueType = ValueType::Bool;
        }
    } else if (idExpr != nullptr) {
        const std::string& rhsIdName = idExpr->name;
        Scope* rhsIdScope = lookTopIdScope(rhsIdName);

        exprValueType = rhsIdScope->symbolTable.getIdValueType(rhsIdName);
    } else if (numberConst != nullptr) {
        exprValueType = ValueType::Number;
    } else if (boolConst != nullptr) {
        exprValueType = ValueType::Bool;
    } else if (funcCallExpr != nullptr) {
        exprValueType = functions->symbolTable.getFuncValueType(funcCallExpr->name);
    } else {
        return newError(SemanticAnalysisResult::INVALID_AST, "Invalid RHS expression");
    }

    if (idValueType == ValueType::Undefined) {
        if (exprValueType == ValueType::Number) {
            idScope->symbolTable.setIdValueDouble(idName, 0);
        } else {
            idScope->symbolTable.setIdValueBool(idName, false);
        }
    } else {
        if (exprValueType != idValueType) {
            return newError(SemanticAnalysisResult::INVALID_VALUE_TYPE, "Invalid RHS expression value type");
        }
    }

    return SemanticAnalysisResult();
}

SemanticAnalysisResult SemanticAnalyzer::checkNumberExpr(ASTNode* node) {
    bool oldOperationCheck = operationCheck;
    operationCheck = true;
    SemanticAnalysisResult checkResult;

    if (node->type == NodeType::ConstNumber) {
        checkResult = checkStatement(node);
    } else if (node->type == NodeType::ConstBool) {
        checkResult = checkStatement(node);

        if (!checkResult.isError()) {
            checkResult = newError(SemanticAnalysisResult::INCOMPATIBLE_OPERAND_TYPES);
        }
    } else if (node->type == NodeType::Id) {
        checkResult = checkStatement(node);

        if (!checkResult.isError()) {
            IdentifierNode* id = static_cast<IdentifierNode*>(node);
            Scope* idScope = lookTopIdScope(id->name);
            if (idScope->symbolTable.getIdValueType(id->name) != ValueType::Number) {
                checkResult = newError(SemanticAnalysisResult::INCOMPATIBLE_OPERAND_TYPES);
            }
        }
    } else if (node->type == NodeType::FuncCall) {
        checkResult = checkStatement(node);

        if (!checkResult.isError()) {
            FuncCallNode* funcCall = static_cast<FuncCallNode*>(node);

            if (functions->symbolTable.getFuncValueType(funcCall->name) == ValueType::Void) {
                checkResult = newError(SemanticAnalysisResult::INVALID_VALUE_TYPE);
            } else if (functions->symbolTable.getFuncValueType(funcCall->name) != ValueType::Number) {
                checkResult = newError(SemanticAnalysisResult::INCOMPATIBLE_OPERAND_TYPES);
            }
        }
    } else if (node->type == NodeType::BinOp) {
        BinOpNode* binOp = dynamic_cast<BinOpNode*>(node);

        if (binOp != nullptr) {
            if (binOp->binOpType != BinOpType::OperatorPlus && binOp->binOpType != BinOpType::OperatorMinus &&
                binOp->binOpType != BinOpType::OperatorMul && binOp->binOpType != BinOpType::OperatorDiv) {
                checkResult = newError(SemanticAnalysisResult::INCOMPATIBLE_OPERAND_TYPES);
            } else {
                SemanticAnalysisResult leftCheckResult = checkNumberExpr(binOp->left);
                if (leftCheckResult.isError()) {
                    return leftCheckResult;
                }

                SemanticAnalysisResult rightCheckResult = checkNumberExpr(binOp->right);
                if (rightCheckResult.isError()) {
                    return rightCheckResult;
                }
            }
        } else {
            checkResult = newError(SemanticAnalysisResult::INVALID_AST, "Invalid Binary Operation Node");
        }
    } else {
        return newError(SemanticAnalysisResult::INVALID_AST);
    }

    operationCheck = oldOperationCheck;
    return checkResult;
}

SemanticAnalysisResult SemanticAnalyzer::checkBoolExprComparison(BinOpNode* node) {
    SemanticAnalysisResult checkResult;
    bool oldOperationCheck = operationCheck;
    operationCheck = true;

    const SemanticAnalysisResult& leftOperandCheck = checkStatement(node->left);
    if (leftOperandCheck.isError()) {
        operationCheck = oldOperationCheck;
        return leftOperandCheck;
    }

    const SemanticAnalysisResult& rightOperandCheck = checkStatement(node->right);
    if (rightOperandCheck.isError()) {
        operationCheck = oldOperationCheck;
        return rightOperandCheck;
    }

    if (node->binOpType == BinOpType::OperatorEqual) {
        // operands should be the same types, both int or bool
        if (!checkNumberExpr(node->left).isError()) {
            if (checkNumberExpr(node->right).isError()) {
                checkResult = newError(SemanticAnalysisResult::INCOMPATIBLE_OPERAND_TYPES);
            }
        } else if (!checkBoolExpr(node->left).isError()) {
            if (checkBoolExpr(node->right).isError()) {
                checkResult = newError(SemanticAnalysisResult::INCOMPATIBLE_OPERAND_TYPES);
            }
        }
    } else {
        // check less than and greater than operators
        if (checkNumberExpr(node->left).isError() || checkNumberExpr(node->right).isError()) {
            checkResult = newError(SemanticAnalysisResult::INCOMPATIBLE_OPERAND_TYPES);
        }
    }

    operationCheck = oldOperationCheck;
    return checkResult;
}

SemanticAnalysisResult SemanticAnalyzer::checkBoolExpr(ASTNode* node) {
    bool oldOperationCheck = operationCheck;
    operationCheck = true;

    SemanticAnalysisResult checkResult;

    if (node->type == NodeType::ConstBool) {
        checkResult = checkStatement(node);
    } else if (node->type == NodeType::ConstNumber) {
        checkResult = checkStatement(node);

        if (!checkResult.isError()) {
            checkResult = newError(SemanticAnalysisResult::INCOMPATIBLE_OPERAND_TYPES);

        }
    } else if (node->type == NodeType::Id) {
        checkResult = checkStatement(node);

        if (!checkResult.isError()) {
            IdentifierNode* id = static_cast<IdentifierNode*>(node);
            Scope* idScope = lookTopIdScope(id->name);
            if (idScope->symbolTable.getIdValueType(id->name) != ValueType::Bool) {
                checkResult = newError(SemanticAnalysisResult::INCOMPATIBLE_OPERAND_TYPES);
            }
        }
    } else if (node->type == NodeType::FuncCall) {
        checkResult = checkStatement(node);

        if (!checkResult.isError()) {
            FuncCallNode* funcCall = static_cast<FuncCallNode*>(node);

            if (functions->symbolTable.getFuncValueType(funcCall->name) == ValueType::Void) {
                checkResult = newError(SemanticAnalysisResult::INVALID_VALUE_TYPE);
            } else if (functions->symbolTable.getFuncValueType(funcCall->name) != ValueType::Bool) {
                checkResult = newError(SemanticAnalysisResult::INCOMPATIBLE_OPERAND_TYPES);
            }
        }
    } else if (node->type == NodeType::BinOp) {
        BinOpNode* binOp = dynamic_cast<BinOpNode*>(node);

        if (binOp != nullptr) {
            if (binOp->binOpType == BinOpType::OperatorEqual || binOp->binOpType == BinOpType::OperatorGreater ||
                binOp->binOpType == BinOpType::OperatorLess) {
                checkResult = checkBoolExprComparison(binOp);
            } else if (binOp->binOpType == BinOpType::OperatorBoolOR ||
                       binOp->binOpType == BinOpType::OperatorBoolAND) {
                SemanticAnalysisResult leftCheckResult = checkBoolExpr(binOp->left);
                if (leftCheckResult.isError()) {
                    return leftCheckResult;
                }

                SemanticAnalysisResult rightCheckResult = checkBoolExpr(binOp->right);
                if (rightCheckResult.isError()) {
                    return rightCheckResult;
                }
            } else {
                checkResult = newError(SemanticAnalysisResult::INCOMPATIBLE_OPERAND_TYPES);
            }
        } else {
            return newError(SemanticAnalysisResult::INVALID_AST, "Invalid Binary Operation Node");
        }
    } else {
        return newError(SemanticAnalysisResult::INVALID_AST);
    }

    operationCheck = oldOperationCheck;
    return checkResult;
}

SemanticAnalysisResult SemanticAnalyzer::checkBreakStmt() {
    if (!forLoopCheck) {
        return newError(SemanticAnalysisResult::INVALID_OPERATION, "Break statement is allowed only in for loops");
    }

    return SemanticAnalysisResult();
}

SemanticAnalysisResult SemanticAnalyzer::checkReturnStmt(ReturnStmtNode* node) {
    SemanticAnalysisResult checkResult;

    bool oldOperationCheck = operationCheck;
    operationCheck = true;

    if (functionBodyCheck) {
        if (functionReturnType == ValueType::Void) {
            if (node->expression != nullptr) {
                checkResult = newError(SemanticAnalysisResult::RETURN_TYPE_MISMATCH,
                                       "Void functions can not return any value");
            }
        } else {
            if (functionReturnType == ValueType::Number) {
                checkResult = checkNumberExpr(node->expression);
            } else if (functionReturnType == ValueType::Bool) {
                checkResult = checkBoolExpr(node->expression);
            }
        }
    } else {
        checkResult = newError(SemanticAnalysisResult::INVALID_OPERATION,
                               "Using of Return stmt is allowed only in functions");
    }

    operationCheck = oldOperationCheck;
    return checkResult;
}

SemanticAnalysisResult SemanticAnalyzer::checkBlockStmt(BlockStmtNode* node) {
    for (const auto& currentStatement : node->stmtList) {
        SemanticAnalysisResult checkResult = checkStatement(currentStatement);
        if (checkResult.isError()) {
            return checkResult;
        }
    }

    return SemanticAnalysisResult();
}

SemanticAnalysisResult SemanticAnalyzer::checkIfStmt(IfStmtNode* node) {
    bool oldOperationCheck = operationCheck;
    operationCheck = true;
    SemanticAnalysisResult condCheckResult = checkBoolExpr(node->condition);
    operationCheck = oldOperationCheck;
    if (condCheckResult.isError()) {
        return condCheckResult;
    }

    openScope();

    SemanticAnalysisResult bodyCheckResult = checkBlockStmt(node->body);
    if (bodyCheckResult.isError()) {
        closeScope();
        return bodyCheckResult;
    }

    if (!node->elseIfStmts.empty()) {
        for (const auto& currentElseIfStmt : node->elseIfStmts) {
            oldOperationCheck = operationCheck;
            operationCheck = true;
            condCheckResult = checkBoolExpr(currentElseIfStmt->condition);
            operationCheck = oldOperationCheck;
            if (condCheckResult.isError()) {
                closeScope();
                return condCheckResult;
            }

            bodyCheckResult = checkBlockStmt(currentElseIfStmt->body);
            if (bodyCheckResult.isError()) {
                closeScope();
                return bodyCheckResult;
            }
        }
    }

    if (node->elseBody != nullptr) {
        bodyCheckResult = checkBlockStmt(node->elseBody);
        if (bodyCheckResult.isError()) {
            closeScope();
            return bodyCheckResult;
        }
    }

    closeScope();

    return SemanticAnalysisResult();
}

SemanticAnalysisResult SemanticAnalyzer::checkForLoop(ForLoopNode* node) {
    openScope();

    if (node->init != nullptr) {
        SemanticAnalysisResult initCheckResult = checkStatement(node->init);
        if (initCheckResult.isError()) {
            closeScope();
            return initCheckResult;
        }
    }

    if (node->condition != nullptr) {
        bool oldOperationCheck = operationCheck;
        operationCheck = true;
        SemanticAnalysisResult condCheckResult = checkBoolExpr(node->condition);
        operationCheck = oldOperationCheck;
        if (condCheckResult.isError()) {
            closeScope();
            return condCheckResult;
        }
    }

    if (node->inc != nullptr) {
        SemanticAnalysisResult incStmtCheckResult = checkAssignExpr(node->inc);
        if (incStmtCheckResult.isError()) {
            closeScope();
            return incStmtCheckResult;
        }
    }

    bool oldForLoopCheck = forLoopCheck;
    forLoopCheck = true;

    SemanticAnalysisResult checkResult = checkBlockStmt(node->body);

    closeScope();
    forLoopCheck = oldForLoopCheck;

    return checkResult;
}

SemanticAnalysisResult SemanticAnalyzer::checkStatement(ASTNode* node) {
    SemanticAnalysisResult checkResult;

    if (node->type == NodeType::BinOp) {
        BinOpNode* binOpNode = dynamic_cast<BinOpNode*>(node);

        if (binOpNode != nullptr) {
            if (binOpNode->binOpType == BinOpType::OperatorPlus ||
                binOpNode->binOpType == BinOpType::OperatorMinus ||
                binOpNode->binOpType == BinOpType::OperatorMul ||
                binOpNode->binOpType == BinOpType::OperatorDiv) {
                checkResult = checkNumberExpr(binOpNode);
                if (!operationCheck) {
                    checkResult = newError(SemanticAnalysisResult::INVALID_OPERATION,
                                           "Number expression evaluated but not used");
                }
            } else if (binOpNode->binOpType == BinOpType::OperatorBoolAND ||
                       binOpNode->binOpType == BinOpType::OperatorBoolOR ||
                       binOpNode->binOpType == BinOpType::OperatorEqual ||
                       binOpNode->binOpType == BinOpType::OperatorLess ||
                       binOpNode->binOpType == BinOpType::OperatorGreater) {
                checkResult = checkBoolExpr(binOpNode);
                if (!operationCheck) {
                    checkResult = newError(SemanticAnalysisResult::INVALID_OPERATION,
                                           "Bool expression evaluated but not used");
                }
            } else if (binOpNode->binOpType == BinOpType::OperatorAssign) {
                checkResult = checkAssignExpr(binOpNode);
            }
        } else {
            checkResult = newError(SemanticAnalysisResult::INVALID_AST, "Invalid Binary Operation Node");
        }
    } else if (node->type == NodeType::DeclVar) {
        DeclVarNode* declVar = dynamic_cast<DeclVarNode*>(node);

        if (declVar != nullptr) {
            checkResult = checkVarDecl(declVar);
        } else {
            checkResult = newError(SemanticAnalysisResult::INVALID_AST, "Invalid Variable Declaration Node");
        }
    } else if (node->type == NodeType::ConstNumber) {
        ConstNumberNode* numberValue = dynamic_cast<ConstNumberNode*>(node);

        if (numberValue == nullptr) {
            checkResult = newError(SemanticAnalysisResult::INVALID_AST, "Invalid Number Const Node");
        } else if (!operationCheck) {
            checkResult = newError(SemanticAnalysisResult::INVALID_OPERATION, "Const number evaluated but not used");
        }
    } else if (node->type == NodeType::ConstBool) {
        ConstBoolNode* boolValue = dynamic_cast<ConstBoolNode*>(node);

        if (boolValue == nullptr) {
            checkResult = newError(SemanticAnalysisResult::INVALID_AST, "Invalid Bool Const Node");
        } else if (!operationCheck) {
            checkResult = newError(SemanticAnalysisResult::INVALID_OPERATION, "Const bool evaluated but not used");
        }
    } else if (node->type == NodeType::Id) {
        IdentifierNode* id = dynamic_cast<IdentifierNode*>(node);

        if (id != nullptr) {
            checkResult = checkId(id);
            if (!checkResult.isError() && !operationCheck) {
                checkResult = newError(SemanticAnalysisResult::INVALID_OPERATION, "Identifier evaluated but not used");
            }
        } else {
            checkResult = newError(SemanticAnalysisResult::INVALID_AST, "Invalid Identifier Node");
        }
    } else if (node->type == NodeType::DeclFunc) {
        DeclFuncNode* funcDecl = dynamic_cast<DeclFuncNode*>(node);

        if (funcDecl != nullptr) {
            checkResult = checkFuncDecl(funcDecl);
        } else {
            checkResult = newError(SemanticAnalysisResult::INVALID_AST, "Invalid Function Declaration Node");
        }
    } else if (node->type == NodeType::FuncCall) {
        FuncCallNode* funcCall = dynamic_cast<FuncCallNode*>(node);

        if (funcCall != nullptr) {
            checkResult = checkFuncCall(funcCall);
            if (!checkResult.isError() && !operationCheck && !isFuncReserved(funcCall->name)) {
                checkResult = newError(SemanticAnalysisResult::INVALID_OPERATION,
                                       "Function call evaluated but not used");
            }
        } else {
            checkResult = newError(SemanticAnalysisResult::INVALID_AST, "Invalid Function Call Node");
        }
    } else if (node->type == NodeType::ReturnStmt) {
        ReturnStmtNode* returnStmt = dynamic_cast<ReturnStmtNode*>(node);

        if (returnStmt != nullptr) {
            checkResult = checkReturnStmt(returnStmt);
        } else {
            checkResult = newError(SemanticAnalysisResult::INVALID_AST, "Invalid Return Statement Node");
        }
    } else if (node->type == NodeType::ForLoop) {
        ForLoopNode* forLoop = dynamic_cast<ForLoopNode*>(node);

        if (forLoop != nullptr) {
            checkResult = checkForLoop(forLoop);
        } else {
            checkResult = newError(SemanticAnalysisResult::INVALID_AST, "Invalid For Loop Node");
        }
    } else if (node->type == NodeType::IfStmt) {
        IfStmtNode* ifStmt = dynamic_cast<IfStmtNode*>(node);

        if (ifStmt != nullptr) {
            checkResult = checkIfStmt(ifStmt);
        } else {
            checkResult = newError(SemanticAnalysisResult::INVALID_AST, "Invalid If Statement Node");
        }
    } else if (node->type == NodeType::BreakStmt) {
        BreakStmtNode* breakStmt = dynamic_cast<BreakStmtNode*>(node);

        if (breakStmt != nullptr) {
            checkResult = checkBreakStmt();
        } else {
            checkResult = newError(SemanticAnalysisResult::INVALID_AST, "Invalid Break Statement Node");
        }
    } else {
        checkResult = newError(SemanticAnalysisResult::INVALID_AST);
    }

    return checkResult;
}

SemanticAnalysisResult SemanticAnalyzer::checkProgram(ProgramTranslationNode* root) {
    for (const auto& currentStatement : root->statements) {
        const SemanticAnalysisResult& checkResult = checkStatement(currentStatement);
        if (checkResult.isError()) {
            return checkResult;
        }
    }

    return SemanticAnalysisResult();
}

bool SemanticAnalyzer::isFuncReserved(const std::string& funcName) {
    return funcName == "print";
}
