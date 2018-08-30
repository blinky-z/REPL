#include "Parser.h"

ASTNode* Parser::expression() {
    ASTNode* termNode = term();

    const Token& nextOp = tokens.getNextToken();
    tokens.returnToken();
    if (nextOp.Type == TokenTypes::Add || nextOp.Type == TokenTypes::Sub) {
        return expressionTail(termNode);
    } else {
        return termNode;
    }
}

ASTNode* Parser::expressionTail(ASTNode* lvalue) {
    ASTNode* rvalue;
    ASTNode* expressionTailNode;

    const Token& token = tokens.getNextToken();

    if (token.Type == TokenTypes::Add) {
        rvalue = term();

        ASTNode* currentBinOpNode = createBinOpNode(OperatorPlus, lvalue, rvalue);
        expressionTailNode = expressionTail(currentBinOpNode);

        if (expressionTailNode->type == Empty) {
            delete expressionTailNode;

            return currentBinOpNode;
        } else {
            return expressionTailNode;
        }
    } else if (token.Type == TokenTypes::Sub) {
        rvalue = term();

        ASTNode* currentBinOpNode = createBinOpNode(OperatorMinus, lvalue, rvalue);
        expressionTailNode = expressionTail(currentBinOpNode);

        if (expressionTailNode->type == Empty) {
            delete expressionTailNode;

            return currentBinOpNode;
        } else {
            return expressionTailNode;
        }
    } else {
        tokens.returnToken();
    }

    return createEmptyNode();
}

ASTNode* Parser::term() {
    ASTNode* factorNode = factor();

    const Token& nextOp = tokens.getNextToken();
    tokens.returnToken();
    if (nextOp.Type == TokenTypes::Mul || nextOp.Type == TokenTypes::Div) {
        return termTail(factorNode);
    } else {
        return factorNode;
    }
}

ASTNode* Parser::termTail(ASTNode* lvalue) {
    ASTNode* rvalue;
    ASTNode* termTailNode;

    const Token& token = tokens.getNextToken();

    if (token.Type == TokenTypes::Mul) {
        rvalue = factor();

        ASTNode* currentBinOpNode = createBinOpNode(OperatorMul, lvalue, rvalue);
        termTailNode = termTail(currentBinOpNode);

        if (termTailNode->type == Empty) {
            delete termTailNode;

            return currentBinOpNode;
        } else {
            return termTailNode;
        }
    } else if (token.Type == TokenTypes::Div) {
        rvalue = factor();

        ASTNode* currentBinOpNode = createBinOpNode(OperatorDiv, lvalue, rvalue);
        termTailNode = termTail(currentBinOpNode);

        if (termTailNode->type == Empty) {
            delete termTailNode;

            return currentBinOpNode;
        } else {
            return termTailNode;
        }
    } else {
        tokens.returnToken();
    }

    return createEmptyNode();
}

ASTNode* Parser::factor() {
    const Token& token = tokens.getNextToken();

    if (token.Type == TokenTypes::Num) {
        double value = getNumTokenValue(token);

        return createNumberNode(value);
    } else if (token.Type == TokenTypes::Sub) {
        ASTNode* value = factor();
        ASTNode* leafMinusNode = createNumberNode(0);

        return createBinOpNode(OperatorMinus, leafMinusNode, value);
    } else if (token.Type == TokenTypes::Id) {
        const std::string& idName = token.Value;

        return createIdentifierNode(idName);
    } else if (token.Type == TokenTypes::ROUND_BRACKET_START) {
        ASTNode* result = expression();

        if (tokens.getNextToken().Type == TokenTypes::ROUND_BRACKET_END) {
            return result;
        } else {
            throw std::runtime_error("Invalid syntax");
        }
    } else {
        throw std::runtime_error("Invalid syntax");
    }
}

ASTNode* Parser::parseMath() {
    return expression();
}

ASTNode* Parser::parseAssign() {
    const Token& id = tokens.getNextToken();
    const std::string& idName = id.Value;

    // skip Assign token
    tokens.getNextToken();

    IdentifierNode* idNode = createIdentifierNode(idName);
    ASTNode* expr = expression();

    return createBinOpNode(OperatorAssign, idNode, expr);
}

ASTNode* Parser::parseDeclVar() {
    ASTNode* expr = nullptr;

    // skip "var" token
    tokens.getNextToken();

    const Token& id = tokens.getNextToken();
    const std::string& idName = id.Value;

    IdentifierNode* idNode = createIdentifierNode(idName);

    const Token& nextToken = tokens.getNextToken();

    if (nextToken.Type == TokenTypes::Assign) {
        expr = expression();
    } else {
        tokens.returnToken();
    }

    return createDeclVarNode(idNode, expr);
}


ASTNode* Parser::parse(const TokenContainer& tokenizedSourceData) {
    tokens = tokenizedSourceData;

    const Token& currentToken = tokens.getNextToken();

    ASTNode* parseResult;

    if (currentToken.Type == TokenTypes::Id) {
        const Token& nextToken = tokens.getNextToken();
        tokens.returnToken();
        tokens.returnToken();

        if (nextToken.Type == TokenTypes::Assign) {
            parseResult = parseAssign();
        } else {
            parseResult = parseMath();
        }
    } else if (currentToken.Type == TokenTypes::ROUND_BRACKET_START) {
        tokens.returnToken();
        parseResult = parseMath();
    } else if (currentToken.Type == TokenTypes::Num) {
        tokens.returnToken();
        parseResult = parseMath();
    } else if (currentToken.Type == TokenTypes::Sub) {
        tokens.returnToken();
        parseResult = parseMath();
    } else if (currentToken.Type == TokenTypes::DeclareId) {
        tokens.returnToken();
        parseResult = parseDeclVar();
    } else {
        throw std::runtime_error("Invalid syntax");
    }

    if (matchParseComplete()) {
        return parseResult;
    } else {
        throw std::runtime_error("Invalid syntax");
    }
}

BinOpNode* Parser::createBinOpNode(ASTNodeBinOpType type, ASTNode* left, ASTNode* right) {
    BinOpNode* node = new BinOpNode;
    node->binOpType = type;
    node->left = left;
    node->right = right;

    return node;
}

NumberNode* Parser::createNumberNode(double value) {
    NumberNode* node = new NumberNode;
    node->value = value;

    return node;
}

IdentifierNode* Parser::createIdentifierNode(std::string name) {
    IdentifierNode* node = new IdentifierNode;
    node->name = name;

    return node;
}

DeclVarNode* Parser::createDeclVarNode(IdentifierNode* id, ASTNode* expr) {
    DeclVarNode* node = new DeclVarNode;
    node->id = id;
    node->expr = expr;

    return node;
}

ASTNode* Parser::createEmptyNode() {
    ASTNode* node = new ASTNode;
    node->type = Empty;

    return node;
}

double Parser::getNumTokenValue(const Token& numToken) {
    return std::stod(numToken.Value);
}

bool Parser::matchParseComplete() {
    const Token& currentToken = tokens.getNextToken();

    if (currentToken.Type == TokenTypes::SEMICOLON) {
        return tokens.getNextToken().Type == TokenTypes::eof;
    } else {
        return currentToken.Type == TokenTypes::eof;
    }
}
