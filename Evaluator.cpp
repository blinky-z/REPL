#include "Evaluator.h"

double Evaluator::EvaluateSubtree(ASTNode* subtree) {
    if (subtree->Type == NumberValue) {
        return subtree->Value;
    } else {
        double leftValue = EvaluateSubtree(subtree->Left);
        double rightValue = EvaluateSubtree(subtree->Right);

        if (subtree->Type == OperatorPlus) {
            return leftValue + rightValue;
        } else if (subtree->Type == OperatorMinus) {
            return leftValue - rightValue;
        } else if (subtree->Type == OperatorMul) {
            return leftValue * rightValue;
        } else {
            return leftValue / rightValue;
        }
    }
}

double Evaluator::Evaluate(ASTNode* root) {
    return EvaluateSubtree(root);
}
