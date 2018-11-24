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

    if (node->type == NodeType::DeclVar) {
        DeclVarNode* declVar = static_cast<DeclVarNode*>(node);
        result += generateDeclVar(declVar);
    }

    return result;
}

std::string BashGenerator::generateBinaryExpr(BinOpNode* node) {
    std::string result;

    if (node->binOpType == BinOpType::OperatorAssign) {
        // TODO: написать реализацию операции assign
//        result = generateAssignExpr(node);
    } else {
        result = generateMathExpr(node);
    }

    return result;
}

std::string BashGenerator::generateId(IdentifierNode* node) {
    return "$" + node->name;
}

std::string BashGenerator::generateConstBool(ConstBoolNode* node) {
    return node->value ? "true" : "false";
}

std::string BashGenerator::generateConstNumber(ConstNumberNode* node) {
    std::string stringNum = std::to_string(node->value);
    while (stringNum.back() == '0') {
        stringNum.pop_back();
    }
    if (stringNum.back() == '.') {
        stringNum.pop_back();
    }

    return stringNum;
}

std::string BashGenerator::generateDeclVar(DeclVarNode* node) {
    std::string result = "declare " + node->id->name;
    if (node->expr != nullptr) {
        if (node->expr->type == NodeType::ConstNumber) {
            ConstNumberNode* numberValue = static_cast<ConstNumberNode*>(node->expr);
            result += "=" + generateConstNumber(numberValue);
        } else if (node->expr->type == NodeType::ConstBool) {
            ConstBoolNode* boolValue = static_cast<ConstBoolNode*>(node->expr);
            result += "=" + generateConstBool(boolValue);
        } else if (node->expr->type == NodeType::Id) {
            IdentifierNode* id = static_cast<IdentifierNode*>(node->expr);
            result += "=" + generateId(id);
        } else if (node->expr->type == NodeType::BinOp) {
            BinOpNode* binExpr = static_cast<BinOpNode*>(node->expr);
            result += "=" + generateBinaryExpr(binExpr);
        } else if (node->expr->type == NodeType::FuncCall) {
            FuncCallNode* funcCall = static_cast<FuncCallNode*>(node->expr);
            result += "=$(" + funcCall->name + ")";
        }
    }

    return result;
}

std::string BashGenerator::generateMathExpr(BinOpNode* node) {
    std::string result = "$((";

    std::string leftResult = "(";
    leftResult += generateStatement(node->left);
    leftResult += ")";

    std::string rightResult = "(";
    rightResult += generateStatement(node->right);
    rightResult += ")";

    switch (node->binOpType) {
        case BinOpType::OperatorPlus: {
            result = leftResult + "+" + rightResult;
            break;
        }
        case BinOpType::OperatorMinus: {
            result = leftResult + "-" + rightResult;
            break;
        }
        case BinOpType::OperatorMul: {
            result = leftResult + "*" + rightResult;
            break;
        }
        case BinOpType::OperatorDiv: {
            result = leftResult + "/" + rightResult;
            break;
        }
        case BinOpType::OperatorBoolAND: {
            result = leftResult + "&&" + rightResult;
            break;
        }
        case BinOpType::OperatorBoolOR: {
            result = leftResult + "||" + rightResult;
            break;
        }
        case BinOpType::OperatorEqual: {
            result = leftResult + "==" + rightResult;
            break;
        }
        case BinOpType::OperatorLess: {
            result = leftResult + "<" + rightResult;
            break;
        }
        case BinOpType::OperatorGreater: {
            result = leftResult + ">" + rightResult;
            break;
        }
        default: {

        }
    }

    result += "))";
    return result;
}
