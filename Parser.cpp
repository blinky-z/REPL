#include "Parser.h"

ASTNode* Parser::expression() {
    ASTNode* termNode = term();

    const Token& nextOp = data.GetNextToken();
    data.ReturnToken();
    if (nextOp.Type == data.Add || nextOp.Type == data.Sub) {
        return expressionTail(termNode);
    } else {
        return termNode;
    }
}

ASTNode* Parser::expressionTail(ASTNode* lvalue) {
    ASTNode* rvalue;
    ASTNode* expressionTailNode;

    const Token& token = data.GetNextToken();

    if (token.Type == data.Add) {
        rvalue = term();

        ASTNode* prevBinOpNode = createNode(OperatorPlus, lvalue, rvalue);
        expressionTailNode = expressionTail(prevBinOpNode);

        if (expressionTailNode->Type == Empty) {
            return prevBinOpNode;
        } else {
            return expressionTailNode;
        }
    } else if (token.Type == data.Sub) {
        rvalue = term();

        ASTNode* prevBinOpNode = createNode(OperatorMinus, lvalue, rvalue);
        expressionTailNode = expressionTail(prevBinOpNode);

        if (expressionTailNode->Type == Empty) {
            return prevBinOpNode;
        } else {
            return expressionTailNode;
        }
    } else {
        data.ReturnToken();
    }

    return createEmptyNode();
}

ASTNode* Parser::term() {
    ASTNode* factorNode = factor();

    const Token& nextOp = data.GetNextToken();
    data.ReturnToken();
    if (nextOp.Type == data.Mul || nextOp.Type == data.Div) {
        return termTail(factorNode);
    } else {
        return factorNode;
    }
}

ASTNode* Parser::termTail(ASTNode* lvalue) {
    ASTNode* rvalue;
    ASTNode* termTailNode;

    const Token& token = data.GetNextToken();

    if (token.Type == data.Mul) {
        rvalue = factor();

        ASTNode* prevBinOpNode = createNode(OperatorMul, lvalue, rvalue);
        termTailNode = termTail(prevBinOpNode);

        if (termTailNode->Type == Empty) {
            return prevBinOpNode;
        } else {
            return termTailNode;
        }
    } else if (token.Type == data.Div) {
        rvalue = factor();

        ASTNode* prevBinOpNode = createNode(OperatorDiv, lvalue, rvalue);
        termTailNode = termTail(prevBinOpNode);

        if (termTailNode->Type == Empty) {
            return prevBinOpNode;
        } else {
            return termTailNode;
        }
    } else {
        data.ReturnToken();
    }

    return createEmptyNode();
}

ASTNode* Parser::factor() {
    const Token& token = data.GetNextToken();

    if (token.Type == data.Num) {
        int value = getNumTokenValue(token);

        return createNumberNode(value);
    } else if (token.Type == data.ROUND_BRACKET_START) {
        ASTNode* result = expression();

        if (data.GetNextToken().Type == data.ROUND_BRACKET_END) {
            return result;
        } else {
            std::cerr << "Invalid syntax" << std::endl;
            exit(EXIT_FAILURE);
        }
    } else {
        std::cerr << "Invalid syntax" << std::endl;
        exit(EXIT_FAILURE);
    }
}

ASTNode* Parser::parseMath() {
    return expression();
}

ASTNode* Parser::parse(const AnalysisContainer& tokenizingStepData) {
    data = tokenizingStepData;

    return parseMath();
}

ASTNode* Parser::createNode(ASTNodeType type, ASTNode* left, ASTNode* right) {
    ASTNode* node = new ASTNode;
    node->Type = type;
    node->Left = left;
    node->Right = right;

    return node;
}

ASTNode* Parser::createNumberNode(int value) {
    ASTNode* node = new ASTNode;
    node->Type = NumberValue;
    node->Value = value;

    return node;
}

ASTNode* Parser::createEmptyNode() {
    ASTNode* node = new ASTNode;
    node->Type = Empty;

    return node;
}

int Parser::getNumTokenValue(const Token& numToken) {
    return std::stoi(numToken.Value);
}