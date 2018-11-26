#include "BashGenerator.h"

std::string BashGenerator::generate(ProgramTranslationNode* root) {
    std::string result;

    for (const auto& currentStatement : root->statements) {
        result += generateStatement(currentStatement);
    }

    return result;
}

std::string BashGenerator::generateStatement(ASTNode* node) {
    std::string result;

    for (int i = 0; i < tabCount; i++) {
        result += "    ";
    }

    if (node->type == NodeType::ConstNumber) {
        ConstNumberNode* constNumber = static_cast<ConstNumberNode*>(node);
        result += "echo" + generateConstNumber(constNumber);
    } else if (node->type == NodeType::ConstBool) {
        ConstBoolNode* constBool = static_cast<ConstBoolNode*>(node);
        result += "echo" + generateConstBool(constBool);
    } else if (node->type == NodeType::Id) {
        IdentifierNode* id = static_cast<IdentifierNode*>(node);
        result += "echo $" + generateId(id);
    } else if (node->type == NodeType::BinOp) {
        BinOpNode* binOp = static_cast<BinOpNode*>(node);
        if (binOp->binOpType == BinOpType::OperatorAssign) {
            result += generateBinaryExprAssign(binOp);
        } else {
            result += "echo $" + generateBinaryExprMath(binOp);
        }
    } else if (node->type == NodeType::DeclVar) {
        DeclVarNode* declVar = static_cast<DeclVarNode*>(node);
        result += generateDeclVar(declVar);
    } else if (node->type == NodeType::FuncCall) {
        FuncCallNode* funcCall = static_cast<FuncCallNode*>(node);
        result += generateFuncCall(funcCall);
    } else if (node->type == NodeType::ReturnStmt) {
        ReturnStmtNode* returnStmt = static_cast<ReturnStmtNode*>(node);
        result += generateReturnStmt(returnStmt);
    } else if (node->type == NodeType::BreakStmt) {
        result += generateBreakStmt();
    } else if (node->type == NodeType::DeclFunc) {
        DeclFuncNode* declFunc = static_cast<DeclFuncNode*>(node);
        result += generateDeclFunc(declFunc);
    } else if (node->type == NodeType::IfStmt) {
        IfStmtNode* ifStmt = static_cast<IfStmtNode*>(node);
        result += generateIfStmt(ifStmt);
    } else if (node->type == NodeType::ForLoop) {
        ForLoopNode* forLoop = static_cast<ForLoopNode*>(node);
        result += generateForLoop(forLoop);
    }

    result += "\n";
    return result;
}

std::string BashGenerator::generateBinaryExprAssign(BinOpNode* node) {
    std::string result;
    IdentifierNode* id = static_cast<IdentifierNode*>(node->left);
    result = generateId(id) + "=";
    result += generateGetValue(node->right);

    return result;
}

std::string BashGenerator::generateId(IdentifierNode* node) {
    return lookTopId(node->name);
}

std::string BashGenerator::generateConstBool(ConstBoolNode* node) {
    return node->value ? "1" : "0";
}

std::string BashGenerator::generateConstNumber(ConstNumberNode* node) {
    std::string stringNum = std::to_string(std::round(node->value));
    while (stringNum.back() == '0') {
        stringNum.pop_back();
    }
    if (stringNum.back() == '.') {
        stringNum.pop_back();
    }

    return stringNum;
}

std::string BashGenerator::generateDeclVar(DeclVarNode* node) {
    std::string result;
    const std::string& idName = node->id->name;

    std::string rhsExpr;
    if (node->expr != nullptr) {
        rhsExpr = generateGetValue(node->expr);
    }

    if (blockScope) {
        std::string uuidName = idName + "_" + getUuid();
        topScope->uuid.emplace(idName, uuidName);
    } else {
        topScope->uuid.emplace(idName, idName);
    }

    if (node->expr != nullptr) {
        result = topScope->uuid[idName] + "=" + rhsExpr;
    }

    return result;
}

std::string BashGenerator::generateBinaryExprMath(BinOpNode* node) {
    std::string result = "((";

    std::string leftResult = generateGetValue(node->left);
    if (node->left->type == NodeType::BinOp) {
        BinOpNode* leftBinOp = static_cast<BinOpNode*>(node->left);
        if (getOperatorPrecedence(leftBinOp) < getOperatorPrecedence(node)) {
            leftResult = "(" + leftResult.substr(3, leftResult.size() - 5) + ")";
        } else {
            leftResult = leftResult.substr(3, leftResult.size() - 5);
        }
    }

    std::string rightResult = generateGetValue(node->right);
    if (node->right->type == NodeType::BinOp) {
        BinOpNode* rightBinOp = static_cast<BinOpNode*>(node->right);
        if (getOperatorPrecedence(rightBinOp) < getOperatorPrecedence(node)) {
            rightResult = "(" + rightResult.substr(3, rightResult.size() - 5) + ")";
        } else {
            rightResult = rightResult.substr(3, rightResult.size() - 5);
        }
    }

    switch (node->binOpType) {
        case BinOpType::OperatorPlus: {
            result += leftResult + " + " + rightResult;
            break;
        }
        case BinOpType::OperatorMinus: {
            result += leftResult + " - " + rightResult;
            break;
        }
        case BinOpType::OperatorMul: {
            result += leftResult + " * " + rightResult;
            break;
        }
        case BinOpType::OperatorDiv: {
            result += leftResult + " / " + rightResult;
            break;
        }
        case BinOpType::OperatorBoolAND: {
            result += leftResult + " && " + rightResult;
            break;
        }
        case BinOpType::OperatorBoolOR: {
            result += leftResult + " || " + rightResult;
            break;
        }
        case BinOpType::OperatorEqual: {
            result += leftResult + " == " + rightResult;
            break;
        }
        case BinOpType::OperatorLess: {
            result += leftResult + " < " + rightResult;
            break;
        }
        case BinOpType::OperatorGreater: {
            result += leftResult + " > " + rightResult;
            break;
        }
        default: {

        }
    }

    result += "))";
    return result;
}

std::string BashGenerator::generateGetValue(ASTNode* node) {
    std::string result;

    if (node->type == NodeType::ConstNumber) {
        ConstNumberNode* constNumber = static_cast<ConstNumberNode*>(node);
        result = generateConstNumber(constNumber);
    } else if (node->type == NodeType::ConstBool) {
        ConstBoolNode* constBool = static_cast<ConstBoolNode*>(node);
        result = generateConstBool(constBool);
    } else if (node->type == NodeType::Id) {
        IdentifierNode* id = static_cast<IdentifierNode*>(node);
        result = "$" + generateId(id) + "";
    } else if (node->type == NodeType::BinOp) {
        BinOpNode* binOp = static_cast<BinOpNode*>(node);
        result = "$" + generateBinaryExprMath(binOp);
    } else if (node->type == NodeType::FuncCall) {
        FuncCallNode* funcCall = static_cast<FuncCallNode*>(node);
        result = "$(" + generateFuncCall(funcCall) + ")";
    }

    return result;
}

std::string BashGenerator::generateFuncCall(FuncCallNode* node) {
    std::string result = node->name;

    for (const auto& currentArg : node->args) {
        result.push_back(' ');
        result += generateGetValue(currentArg);
    }

    result += " | tail -n1";
    return result;
}

std::string BashGenerator::generateReturnStmt(ReturnStmtNode* node) {
    std::string result = "echo ";
    result += generateGetValue(node->expression);
    result += "; return";

    return result;
}

std::string BashGenerator::generateDeclFunc(DeclFuncNode* node) {
    std::string result = "function " + node->name + " {\n";

    openScope();
    tabCount++;

    for (unsigned long currentParamNum = 0; currentParamNum < node->args.size(); currentParamNum++) {
        const std::string& idName = node->args[currentParamNum]->name;
        topScope->uuid.emplace(idName, idName);
        for (int i = 0; i < tabCount; i++) {
            result += "    ";
        }
        result += "local " + idName + "=$" + std::to_string(currentParamNum + 1) + "\n";
    }

    bool oldBlockScope = blockScope;
    blockScope = true;
    for (const auto& currentStmt : node->body->stmtList) {
        result += generateStatement(currentStmt);
    }
    blockScope = oldBlockScope;

    closeScope();

    if (node->body->stmtList.empty()) {
        for (int i = 0; i < tabCount; i++) {
            result += "    ";
        }
        result += ":\n";
    }

    tabCount--;

    for (int i = 0; i < tabCount; i++) {
        result += "    ";
    }
    result += "}";
    return result;
}

std::string BashGenerator::generateBreakStmt() {
    return "break";
}

std::string BashGenerator::generateIfStmt(IfStmtNode* node) {
    bool oldBlockScope = blockScope;
    blockScope = true;

    openScope();

    std::string result = "if ";

    std::string condStmt = generateGetValue(node->condition);
    if (node->condition->type == NodeType::BinOp) {
        condStmt = condStmt.substr(3, condStmt.size() - 5); // remove $(( and ))
    }
    result += "((" + condStmt + ")); then ";

    result += generateBlockStmt(node->body);

    if (!node->elseIfStmts.empty()) {
        for (const auto& currentElifStmt : node->elseIfStmts) {
            for (int i = 0; i < tabCount; i++) {
                result += "    ";
            }
            result += "elif ";
            condStmt = generateGetValue(node->condition);
            if (node->condition->type == NodeType::BinOp) {
                condStmt = condStmt.substr(3, condStmt.size() - 5);
            }
            result += "((" + condStmt + ")); then ";

            result += generateBlockStmt(currentElifStmt->body);
        }
    }

    if (node->elseBody != nullptr) {
        for (int i = 0; i < tabCount; i++) {
            result += "    ";
        }
        result += "else ";
        result += generateBlockStmt(node->elseBody);
    }
    for (int i = 0; i < tabCount; i++) {
        result += "    ";
    }
    result += "fi";

    closeScope();
    blockScope = oldBlockScope;

    return result;
}

std::string BashGenerator::generateBlockStmt(BlockStmtNode* node) {
    std::string result = "{\n";

    tabCount++;
    for (const auto& currentStmt : node->stmtList) {
        result += generateStatement(currentStmt);
    }

    if (node->stmtList.empty()) {
        for (int i = 0; i < tabCount; i++) {
            result += "    ";
        }
        result += ":\n";
    }

    tabCount--;

    for (int i = 0; i < tabCount; i++) {
        result += "    ";
    }
    result += "}\n";
    return result;
}

std::string BashGenerator::generateForLoop(ForLoopNode* node) {
    std::string result = "for ((";

    bool oldBlockScope = blockScope;
    blockScope = true;

    openScope();
    if (node->init != nullptr) {
        std::string initStmt;
        if (node->init->type == NodeType::DeclVar) {
            DeclVarNode* declVar = static_cast<DeclVarNode*>(node->init);
            initStmt = generateDeclVar(declVar);
        } else {
            BinOpNode* binOpCondAssign = static_cast<BinOpNode*>(node->init);
            initStmt = generateBinaryExprAssign(binOpCondAssign);
        }
        result += initStmt;
    }
    result += "; ";

    if (node->condition != nullptr) {
        std::string condStmt = generateGetValue(node->condition);
        result += condStmt;
    }
    result += "; ";

    if (node->inc != nullptr) {
        BinOpNode* binOpIncAssign = static_cast<BinOpNode*>(node->inc);
        std::string incStmt = generateBinaryExprAssign(binOpIncAssign);
        result += incStmt;
    }

    result += ")) ";

    result += generateBlockStmt(node->body);

    blockScope = oldBlockScope;

    closeScope();

    return result;
}

int BashGenerator::getOperatorPrecedence(BinOpNode* node) {
    static std::unordered_map<BinOpType::Type, int> opPrecedence = {
            std::pair<BinOpType::Type, int>(BinOpType::OperatorMul, 6),
            std::pair<BinOpType::Type, int>(BinOpType::OperatorDiv, 6),
            std::pair<BinOpType::Type, int>(BinOpType::OperatorMinus, 5),
            std::pair<BinOpType::Type, int>(BinOpType::OperatorPlus, 5),
            std::pair<BinOpType::Type, int>(BinOpType::OperatorGreater, 4),
            std::pair<BinOpType::Type, int>(BinOpType::OperatorLess, 4),
            std::pair<BinOpType::Type, int>(BinOpType::OperatorEqual, 3),
            std::pair<BinOpType::Type, int>(BinOpType::OperatorBoolAND, 2),
            std::pair<BinOpType::Type, int>(BinOpType::OperatorBoolOR, 1),
    };

    return opPrecedence[node->binOpType];
}

void BashGenerator::openScope() {
    topScope = new Scope(topScope);
}

void BashGenerator::closeScope() {
    Scope* oldScope = topScope;
    topScope = topScope->outer;
    delete oldScope;
}

std::string BashGenerator::getUuid() {
    std::string result = sole::uuid4().str();
    result.erase(std::remove(result.begin(), result.end(), '-'), result.end());
    return result;
}

std::string BashGenerator::lookTopId(const std::string& id) {
    // returns id's uuid
    Scope* oldTopScope = topScope;
    std::string idUuid;
    while (topScope != nullptr) {
        if (topScope->uuid.find(id) != topScope->uuid.end()) {
            idUuid = topScope->uuid[id];
            break;
        }
        topScope = topScope->outer;
    }
    topScope = oldTopScope;

    return idUuid;
}
