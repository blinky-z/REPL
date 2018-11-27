#include "Evaluator.h"

EvalResult Evaluator::EvaluateMathExpr(ASTNode* subtree) {
    EvalResult result;
    if (subtree->type == NodeType::ConstNumber) {
        ConstNumberNode* node = static_cast<ConstNumberNode*>(subtree);

        result.setValueDouble(EvaluateNumberConstant(node));
    } else if (subtree->type == NodeType::Id) {
        IdentifierNode* id = static_cast<IdentifierNode*>(subtree);

        Scope* curScope = lookTopIdScope(id->name);
        result.setValueDouble(EvaluateIdDouble(curScope, id));
    } else if (subtree->type == NodeType::FuncCall) {
        FuncCallNode* node = static_cast<FuncCallNode*>(subtree);

        const EvalResult& funcCallResult = EvaluateFuncCall(node);
        result = funcCallResult;
    } else if (subtree->type == NodeType::BinOp) {
        BinOpNode* node = static_cast<BinOpNode*>(subtree);

        EvalResult leftValue = EvaluateMathExpr(node->left);
        EvalResult rightValue = EvaluateMathExpr(node->right);

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
            default: {
            }
        }
    }

    return result;
}

EvalResult Evaluator::EvaluateBoolExpr(ASTNode* subtree) {
    EvalResult result;

    if (subtree->type == NodeType::ConstBool) {
        ConstBoolNode* node = static_cast<ConstBoolNode*>(subtree);

        result.setValueBool(EvaluateBoolConstant(node));
    } else if (subtree->type == NodeType::Id) {
        IdentifierNode* id = static_cast<IdentifierNode*>(subtree);

        Scope* curScope = lookTopIdScope(id->name);
        result.setValueBool(EvaluateIdBool(curScope, id));
    } else if (subtree->type == NodeType::FuncCall) {
        FuncCallNode* node = static_cast<FuncCallNode*>(subtree);

        const EvalResult& funcCallResult = EvaluateFuncCall(node);
        result = funcCallResult;
    } else if (subtree->type == NodeType::BinOp) {
        BinOpNode* binOp = static_cast<BinOpNode*>(subtree);

        if (binOp->binOpType == BinOpType::OperatorEqual) {
            return EvaluateEqual(binOp);
        } else if (binOp->binOpType == BinOpType::OperatorLess || binOp->binOpType == BinOpType::OperatorGreater) {
            return EvaluateComparison(binOp);
        }

        EvalResult leftValue = EvaluateBoolExpr(binOp->left);
        EvalResult rightValue = EvaluateBoolExpr(binOp->right);

        switch (binOp->binOpType) {
            case BinOpType::OperatorBoolAND:
                result.setValueBool(leftValue.getResultBool() && rightValue.getResultBool());
                break;
            case BinOpType::OperatorBoolOR:
                result.setValueBool(leftValue.getResultBool() || rightValue.getResultBool());
                break;
            default: {
            }
        }
    }

    return result;
}

EvalResult Evaluator::EvaluateAssignValue(IdentifierNode* id, ASTNode* expr) {
    EvalResult result;

    Scope* lhsIdScope = lookTopIdScope(id->name);

    BinOpNode* binOpExpr = dynamic_cast<BinOpNode*>(expr);
    IdentifierNode* idExpr = dynamic_cast<IdentifierNode*>(expr);
    ConstNumberNode* numberConst = dynamic_cast<ConstNumberNode*>(expr);
    ConstBoolNode* boolConst = dynamic_cast<ConstBoolNode*>(expr);
    FuncCallNode* funcCallExpr = dynamic_cast<FuncCallNode*>(expr);

    if (binOpExpr != nullptr) {
        if (binOpExpr->binOpType == BinOpType::OperatorPlus ||
            binOpExpr->binOpType == BinOpType::OperatorMinus ||
            binOpExpr->binOpType == BinOpType::OperatorMul ||
            binOpExpr->binOpType == BinOpType::OperatorDiv) {
            EvalResult exprResult = EvaluateMathExpr(binOpExpr);

            lhsIdScope->symbolTable.setIdValueDouble(id->name, exprResult.getResultDouble());
            result.setValueString("Assign value");
        } else if (binOpExpr->binOpType == BinOpType::OperatorBoolAND ||
                   binOpExpr->binOpType == BinOpType::OperatorBoolOR ||
                   binOpExpr->binOpType == BinOpType::OperatorEqual ||
                   binOpExpr->binOpType == BinOpType::OperatorLess ||
                   binOpExpr->binOpType == BinOpType::OperatorGreater) {
            EvalResult exprResult = EvaluateBoolExpr(binOpExpr);

            lhsIdScope->symbolTable.setIdValueBool(id->name, exprResult.getResultBool());
            result.setValueString("Assign value");
        }
    } else if (idExpr != nullptr) {
        Scope* rhsIdScope = lookTopIdScope(idExpr->name);

        ValueType::Type rhsIdType = rhsIdScope->symbolTable.getIdValueType(idExpr->name);

        if (rhsIdType == ValueType::Number) {
            lhsIdScope->symbolTable.setIdValueDouble(id->name, EvaluateIdDouble(rhsIdScope, idExpr));
            result.setValueString("Assign value");
        } else if (rhsIdType == ValueType::Bool) {
            lhsIdScope->symbolTable.setIdValueBool(id->name, EvaluateIdBool(rhsIdScope, idExpr));
            result.setValueString("Assign value");
        }
    } else if (funcCallExpr != nullptr) {
        const EvalResult& funcCallResult = EvaluateFuncCall(funcCallExpr);

        switch (funcCallResult.getResultType()) {
            case ValueType::Number: {
                lhsIdScope->symbolTable.setIdValueDouble(id->name, funcCallResult.getResultDouble());
                result.setValueString("Assign value");
                break;
            }
            case ValueType::Bool: {
                lhsIdScope->symbolTable.setIdValueBool(id->name, funcCallResult.getResultBool());
                result.setValueString("Assign value");
                break;
            }
            default: {
            }
        }
    } else if (numberConst != nullptr) {
        lhsIdScope->symbolTable.setIdValueDouble(id->name, EvaluateNumberConstant(numberConst));
        result.setValueString("Assign value");
    } else if (boolConst != nullptr) {
        lhsIdScope->symbolTable.setIdValueBool(id->name, EvaluateBoolConstant(boolConst));
        result.setValueString("Assign value");
    }

    return result;
}

EvalResult Evaluator::EvaluateReturnStmt(ReturnStmtNode* subtree) {
    EvalResult result;

    if (subtree->expression != nullptr) {
        result = Evaluate(subtree->expression);
    } else {
        result.setVoidResult();
    }

    funcReturn = true;
    return result;
}

EvalResult Evaluator::EvaluateFuncCall(FuncCallNode* funcCall) {
    EvalResult result;

    const std::string& funcName = funcCall->name;

    DeclFuncNode* func = functions->symbolTable.getFunc(funcName);

    // evaluate call parameters
    std::vector<EvalResult> callParamsValues;
    for (const auto& currentCallParam : funcCall->args) {
        const EvalResult& currentParamValue = Evaluate(currentCallParam);
        callParamsValues.emplace_back(currentParamValue);
    }

    openScope();

    for (unsigned long currentIdNum = 0; currentIdNum != func->argsSize; currentIdNum++) {
        IdentifierNode* id = func->args[currentIdNum];
        const std::string& declParamName = id->name;

        const EvalResult& callParamValue = callParamsValues[currentIdNum];

        // add param to function scope
        topScope->symbolTable.addNewIdentifier(declParamName);
        switch (callParamValue.getResultType()) {
            case ValueType::Number: {
                topScope->symbolTable.setIdValueDouble(declParamName, callParamValue.getResultDouble());
                break;
            }
            case ValueType::Bool: {
                topScope->symbolTable.setIdValueBool(declParamName, callParamValue.getResultBool());
                break;
            }
            default: {
            }
        }
    }

    Scope* oldOuterScope = topScope->outer;
    topScope->outer = globalScope;

    result = EvaluateBlockStmt(func->body);

    topScope->outer = oldOuterScope;
    closeScope();

    if (result.getResultType() == ValueType::Compound) {
        // function returned without return statement, so we have void function
        result.setVoidResult();
    } else {
        funcReturn = false;
    }

    return result;
}

EvalResult Evaluator::EvaluateDeclFunc(DeclFuncNode* subtree) {
    EvalResult result;
    functions->symbolTable.addNewFunc(subtree);

    result.setValueString("Declare func");
    return result;
}

EvalResult Evaluator::EvaluateDeclVar(DeclVarNode* subtree) {
    EvalResult result;

    std::string idName = subtree->id->name;

    if (subtree->expr != nullptr) {
        const EvalResult& exprResult = Evaluate(subtree->expr);
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
            }
        }
    } else {
        topScope->symbolTable.addNewIdentifier(idName);
    }

    result.setValueString("Declare Variable");

    return result;
}

EvalResult Evaluator::EvaluateEqual(BinOpNode* subtree) {
    EvalResult result;

    EvalResult leftValue = Evaluate(subtree->left);
    EvalResult rightValue = Evaluate(subtree->right);

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
    EvalResult rightValue = Evaluate(subtree->right);

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
        if (funcReturn) {
            return currentResult;
        }

        if (breakForLoop) {
            if (currentStatement->type != NodeType::BreakStmt) {
                stmtResults.emplace_back(currentResult);
            }
            break;
        }

        stmtResults.emplace_back(currentResult);
    }

    result.setBlockResult(stmtResults);
    return result;
}

EvalResult Evaluator::EvaluateIfStmt(IfStmtNode* subtree) {
    EvalResult result;

    const EvalResult& conditionResult = EvaluateBoolExpr(subtree->condition);

    openScope();

    if (conditionResult.getResultBool()) {
        result = EvaluateBlockStmt(subtree->body);
    } else if (subtree->elseIfStmts.size() != 0) {
        for (const auto& currentElseIfStmt : subtree->elseIfStmts) {
            const EvalResult& elseIfCondResult = EvaluateBoolExpr(currentElseIfStmt->condition);
            if (elseIfCondResult.getResultBool()) {
                result = EvaluateBlockStmt(currentElseIfStmt->body);
                break;
            }
        }
    } else if (subtree->elseBody) {
        result = EvaluateBlockStmt(subtree->elseBody);
    } else {
        result.setVoidResult();
    }

    closeScope();

    return result;
}

EvalResult Evaluator::EvaluateForLoopStmt(ForLoopNode* subtree) {
    EvalResult result;

    openScope();

    if (subtree->init != nullptr) {
        Evaluate(subtree->init);
    }

    std::vector<EvalResult> blockStmtResults;

    while (subtree->condition == nullptr || Evaluate(subtree->condition).getResultBool()) {
        const EvalResult& currentBlockResult = EvaluateBlockStmt(subtree->body);
        if (funcReturn) {
            closeScope();
            return currentBlockResult;
        }
        blockStmtResults.emplace_back(currentBlockResult);

        if (breakForLoop) {
            breakForLoop = false;
            break;
        }

        if (subtree->inc != nullptr) {
            Evaluate(subtree->inc);
        }
    }

    result.setBlockResult(blockStmtResults);

    closeScope();
    return result;
}

EvalResult Evaluator::Evaluate(ASTNode* root) {
    EvalResult result;

    if (root->type == NodeType::BinOp) {
        BinOpNode* node = static_cast<BinOpNode*>(root);

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
        }
    } else if (root->type == NodeType::DeclVar) {
        DeclVarNode* node = static_cast<DeclVarNode*>(root);
        result = EvaluateDeclVar(node);
    } else if (root->type == NodeType::ConstNumber) {
        ConstNumberNode* node = static_cast<ConstNumberNode*>(root);
        result.setValueDouble(EvaluateNumberConstant(node));
    } else if (root->type == NodeType::ConstBool) {
        ConstBoolNode* node = static_cast<ConstBoolNode*>(root);
        result.setValueBool(EvaluateBoolConstant(node));
    } else if (root->type == NodeType::Id) {
        IdentifierNode* id = static_cast<IdentifierNode*>(root);
        Scope* curScope = lookTopIdScope(id->name);
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
            default: {

            }
        }
    } else if (root->type == NodeType::IfStmt) {
        IfStmtNode* node = static_cast<IfStmtNode*>(root);
        result = EvaluateIfStmt(node);
    } else if (root->type == NodeType::ForLoop) {
        ForLoopNode* node = static_cast<ForLoopNode*>(root);
        result = EvaluateForLoopStmt(node);
    } else if (root->type == NodeType::DeclFunc) {
        DeclFuncNode* node = static_cast<DeclFuncNode*>(root);
        result = EvaluateDeclFunc(node);
    } else if (root->type == NodeType::FuncCall) {
        FuncCallNode* node = static_cast<FuncCallNode*>(root);
        result = EvaluateFuncCall(node);
    } else if (root->type == NodeType::ReturnStmt) {
        ReturnStmtNode* node = static_cast<ReturnStmtNode*>(root);
        result = EvaluateReturnStmt(node);
    } else if (root->type == NodeType::BreakStmt) {
        breakForLoop = true;
    }

    return result;
}

double Evaluator::EvaluateIdDouble(Scope* scope, IdentifierNode* id) {
    return scope->symbolTable.getIdValueDouble(id->name);
}

bool Evaluator::EvaluateIdBool(Scope* scope, IdentifierNode* id) {
    return scope->symbolTable.getIdValueBool(id->name);
}

double Evaluator::EvaluateNumberConstant(ConstNumberNode* num) {
    return num->value;
}

bool Evaluator::EvaluateBoolConstant(ConstBoolNode* num) {
    return num->value;
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