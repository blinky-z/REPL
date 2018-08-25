#include "Parser.h"

ASTNode* Parser::expression() {
    ASTNode* tnode = term();
    ASTNode* e1node = expression1();

    if (e1node->Type == NumberValue && e1node->Value == 0) {
        return tnode;
    }
    return createNode(OperatorPlus, tnode, e1node);
}

ASTNode* Parser::expression1() {
    ASTNode* tnode;
    ASTNode* e1node;

    const Token& token = data.GetNextToken();

    if (token.type == data.Add) {
        tnode = term();
        e1node = expression1();

        if (e1node->Type == Empty) {
            return tnode;
        }
        return createNode(OperatorPlus, e1node, tnode);
    } else if (token.type == data.Sub) {
        tnode = term();
        e1node = expression1();

        if (e1node->Type == Empty) {
            tnode->Value = tnode->Value * -1;
            return tnode;
        }
        return createNode(OperatorMinus, e1node, tnode);
    } else {
        data.ReturnToken();
    }

    return createNodeNumber(0);
}

ASTNode* Parser::term() {
    ASTNode* fnode = factor();
    ASTNode* t1node = term1();

    if (t1node->Type == NumberValue && t1node->Value == 1) {
        return fnode;
    }
    return createNode(OperatorMul, fnode, t1node);
}

ASTNode* Parser::term1() {
    ASTNode* fnode;
    ASTNode* t1node;

    const Token& token = data.GetNextToken();

    if (token.type == data.Mul) {
        fnode = factor();
        t1node = term1();

        if (t1node->Type == Empty) {
            return fnode;
        }
        return createNode(OperatorMul, t1node, fnode);
    } else if (token.type == data.Div) {
        fnode = factor();
        t1node = term1();

        if (t1node->Type == Empty) {
            fnode->Value = 1 / fnode->Value;
            return fnode;
        }
        return createNode(OperatorDiv, t1node, fnode);
    } else {
        data.ReturnToken();
    }

    return createEmptyNode();
}

ASTNode* Parser::factor() {
    const Token& token = data.GetNextToken();

    if (token.type == data.Num) {
        int value = std::stoi(token.value);

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

ASTNode* Parser::parse() {
    // нужно будет определять по контексту какая операция
    // пока будут только математические формулы

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