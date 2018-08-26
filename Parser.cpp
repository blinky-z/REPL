#include "Parser.h"

ASTNode* Parser::expression() {
    ASTNode* termNode = term();
    ASTNode* expressionTailNode = expressionTail();

    if (expressionTailNode->Type == Empty) {
        return termNode;
    }
    return createNode(OperatorPlus, termNode, expressionTailNode);
}

ASTNode* Parser::expressionTail() {
    ASTNode* termNode;
    ASTNode* expressionTailNode;

    const Token& token = data.GetNextToken();

    if (token.type == data.Add) {
        termNode = term();
        expressionTailNode = expressionTail();

        if (expressionTailNode->Type == Empty) {
            return termNode;
        }
        return createNode(OperatorPlus, expressionTailNode, termNode);
    } else if (token.type == data.Sub) {
        termNode = term();
        expressionTailNode = expressionTail();

        if (expressionTailNode->Type == Empty) {
            return createNode(OperatorMinus, createLeafMinusOpNode(), termNode);
        }
        return createNode(OperatorMinus, expressionTailNode, termNode);
    } else {
        data.ReturnToken();
    }

    return createEmptyNode();
}

ASTNode* Parser::term() {
    ASTNode* factorNode = factor();
    ASTNode* termTailNode = termTail();

    if (termTailNode->Type == Empty) {
        return factorNode;
    }
    return createNode(OperatorMul, factorNode, termTailNode);
}

ASTNode* Parser::termTail() {
    ASTNode* factorNode;
    ASTNode* termTailNode;

    const Token& token = data.GetNextToken();

    if (token.type == data.Mul) {
        factorNode = factor();
        termTailNode = termTail();

        if (termTailNode->Type == Empty) {
            return factorNode;
        }
        return createNode(OperatorMul, termTailNode, factorNode);
    } else if (token.type == data.Div) {
        factorNode = factor();
        termTailNode = termTail();

        if (termTailNode->Type == Empty) {
            return createNode(OperatorDiv, createLeafDivOpNode(), factorNode);
        }
        return createNode(OperatorDiv, termTailNode, factorNode);
    } else {
        data.ReturnToken();
    }

    return createEmptyNode();
}

ASTNode* Parser::factor() {
    const Token& token = data.GetNextToken();

    if (token.type == data.Num) {
        int value = getNumTokenValue(token);

        return createNodeNumber(value);
    } else if (token.type == data.ROUND_BRACKET_START) {
        ASTNode* result = expression();

        if (data.GetNextToken().type == data.ROUND_BRACKET_END) {
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

ASTNode* Parser::createNodeNumber(int value) {
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

ASTNode* Parser::createLeafMinusOpNode() {
    ASTNode* leafNode = new ASTNode;
    leafNode->Type = NumberValue;
    leafNode->Value = 0;

    return leafNode;
}

ASTNode* Parser::createLeafDivOpNode() {
    ASTNode* leafNode = new ASTNode;
    leafNode->Type = NumberValue;
    leafNode->Value = 1;

    return leafNode;
}

int Parser::getNumTokenValue(const Token& numToken) {
    return std::stoi(numToken.value);
}
