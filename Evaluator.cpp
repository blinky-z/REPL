#include "Evaluator.h"

double Evaluator::EvaluateSubtree(ASTNode* subtree) {
    if (subtree->type == NumberValue) {
        NumberNode* node = static_cast<NumberNode*>(subtree);
        return node->value;
    } else {
        BinOpNode* node = static_cast<BinOpNode*>(subtree);
        double leftValue = EvaluateSubtree(node->left);
        double rightValue = EvaluateSubtree(node->right);

        if (node->binOpType == OperatorPlus) {
            return leftValue + rightValue;
        } else if (node->binOpType == OperatorMinus) {
            return leftValue - rightValue;
        } else if (node->binOpType == OperatorMul) {
            return leftValue * rightValue;
        } else {
            return leftValue / rightValue;
        }
    }
}

double Evaluator::Evaluate(ASTNode* root) {
    return EvaluateSubtree(root);
}
