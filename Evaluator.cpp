#include "Evaluator.h"

double Evaluator::EvaluateSubtree(ASTNode* subtree) {
    if (subtree->type == NumberValue) {
        NumberNode* node = static_cast<NumberNode*>(subtree);
        return node->value;
    } else if (subtree->type == BinOp) {
        BinOpNode* node = static_cast<BinOpNode*>(subtree);
        double leftValue = EvaluateSubtree(node->left);
        double rightValue = EvaluateSubtree(node->right);

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
                throw std::runtime_error("Unknown binary operation");
        }
    } else {
        throw std::runtime_error("Invalid AST");
    }
}

double Evaluator::Evaluate(ASTNode* root) {
    return EvaluateSubtree(root);
}
