#include "Parser.h"

ASTNode* Parser::expression() {
    ASTNode* termNode = term();

    const Token& nextOp = tokens.getNextToken();
    tokens.returnToken();
    if (nextOp.Type == TokenType::Add || nextOp.Type == TokenType::Sub ||
        nextOp.Type == TokenType::BoolOR || nextOp.Type == TokenType::Equal) {
        return expressionTail(termNode);
    } else {
        return termNode;
    }
}

ASTNode* Parser::expressionTail(ASTNode* lvalue) {
    ASTNode* rvalue;
    ASTNode* expressionTailNode;

    const Token& token = tokens.getNextToken();

    if (token.Type == TokenType::Add) {
        rvalue = term();

        ASTNode* currentBinOpNode = createBinOpNode(BinOpType::OperatorPlus, lvalue, rvalue);
        expressionTailNode = expressionTail(currentBinOpNode);

        if (expressionTailNode->type == NodeType::Empty) {
            delete expressionTailNode;

            return currentBinOpNode;
        } else {
            return expressionTailNode;
        }
    } else if (token.Type == TokenType::Sub) {
        rvalue = term();

        ASTNode* currentBinOpNode = createBinOpNode(BinOpType::OperatorMinus, lvalue, rvalue);
        expressionTailNode = expressionTail(currentBinOpNode);

        if (expressionTailNode->type == NodeType::Empty) {
            delete expressionTailNode;

            return currentBinOpNode;
        } else {
            return expressionTailNode;
        }
    } else if (token.Type == TokenType::BoolOR) {
        rvalue = term();

        ASTNode* currentBinOpNode = createBinOpNode(BinOpType::OperatorBoolOR, lvalue, rvalue);
        expressionTailNode = expressionTail(currentBinOpNode);

        if (expressionTailNode->type == NodeType::Empty) {
            delete expressionTailNode;

            return currentBinOpNode;
        } else {
            return expressionTailNode;
        }
    } else if (token.Type == TokenType::Equal) {
        rvalue = term();

        if (lvalue->type == NodeType::BinOp) {
            BinOpNode* node = static_cast<BinOpNode*>(lvalue);

            if (node->binOpType == BinOpType::OperatorBoolOR || node->binOpType == BinOpType::OperatorBoolAND) {
                ASTNode* equalBinOpNode = createBinOpNode(BinOpType::OperatorEqual, node->right, rvalue);

                ASTNode* curBinOpNode = createBinOpNode(node->binOpType, node->left, equalBinOpNode);
                expressionTailNode = expressionTail(curBinOpNode);

                if (expressionTailNode->type == NodeType::Empty) {
                    delete expressionTailNode;

                    return curBinOpNode;
                } else {
                    return expressionTailNode;
                }
            }
        } else {
            ASTNode* currentBinOpNode = createBinOpNode(BinOpType::OperatorEqual, lvalue, rvalue);
            expressionTailNode = expressionTail(currentBinOpNode);

            if (expressionTailNode->type == NodeType::Empty) {
                delete expressionTailNode;

                return currentBinOpNode;
            } else {
                return expressionTailNode;
            }
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
    if (nextOp.Type == TokenType::Mul || nextOp.Type == TokenType::Div || nextOp.Type == TokenType::BoolAND) {
        return termTail(factorNode);
    } else {
        return factorNode;
    }
}

ASTNode* Parser::termTail(ASTNode* lvalue) {
    ASTNode* rvalue;
    ASTNode* termTailNode;

    const Token& token = tokens.getNextToken();

    if (token.Type == TokenType::Mul) {
        rvalue = factor();

        ASTNode* currentBinOpNode = createBinOpNode(BinOpType::OperatorMul, lvalue, rvalue);
        termTailNode = termTail(currentBinOpNode);

        if (termTailNode->type == NodeType::Empty) {
            delete termTailNode;

            return currentBinOpNode;
        } else {
            return termTailNode;
        }
    } else if (token.Type == TokenType::Div) {
        rvalue = factor();

        ASTNode* currentBinOpNode = createBinOpNode(BinOpType::OperatorDiv, lvalue, rvalue);
        termTailNode = termTail(currentBinOpNode);

        if (termTailNode->type == NodeType::Empty) {
            delete termTailNode;

            return currentBinOpNode;
        } else {
            return termTailNode;
        }
    } else if (token.Type == TokenType::BoolAND) {
        rvalue = factor();

        ASTNode* currentBinOpNode = createBinOpNode(BinOpType::OperatorBoolAND, lvalue, rvalue);
        termTailNode = termTail(currentBinOpNode);

        if (termTailNode->type == NodeType::Empty) {
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

    if (token.Type == TokenType::Num) {
        double value = getNumTokenValue(token);

        return createNumberNode(value);
    } else if (token.Type == TokenType::Bool) {
        bool value = getBoolTokenValue(token);

        return createBoolNode(value);
    } else if (token.Type == TokenType::Sub) {
        // convert unary minus (-x) into (0 - x) expr
        ASTNode* value = factor();
        ASTNode* leafMinusNode = createNumberNode(0);

        return createBinOpNode(BinOpType::OperatorMinus, leafMinusNode, value);
    } else if (token.Type == TokenType::Id) {
        const std::string& idName = token.Value;

        return createIdentifierNode(idName);
    } else if (token.Type == TokenType::ROUND_BRACKET_START) {
        ASTNode* result = expression();

        if (tokens.getNextToken().Type == TokenType::ROUND_BRACKET_END) {
            return result;
        } else {
            throw std::runtime_error("Invalid syntax");
        }
    } else {
        throw std::runtime_error("Invalid syntax");
    }
}

ASTNode* Parser::parseAssign() {
    const Token& id = tokens.getNextToken();
    const std::string& idName = id.Value;

    // skip '=' token
    tokens.getNextToken();

    IdentifierNode* idNode = createIdentifierNode(idName);
    ASTNode* expr = expression();

    return createBinOpNode(BinOpType::OperatorAssign, idNode, expr);
}

ASTNode* Parser::parseDeclVar() {
    ASTNode* expr = nullptr;

    IdentifierNode* idNode = static_cast<IdentifierNode*>(parseId());

    const Token& nextToken = tokens.getNextToken();
    if (nextToken.Type == TokenType::Assign) {
        expr = expression();
    } else {
        tokens.returnToken();
    }

    return createDeclVarNode(idNode, expr);
}

ASTNode* Parser::parseId() {
    const Token& token = tokens.getNextToken();

    if (token.Type != TokenType::Id) {
        throw std::runtime_error("Expected identifier");
    } else {
        return createIdentifierNode(token.Value);
    }
}

ASTNode* Parser::parseForLoop() {
    // TODO: имплементировать For Loop

    return createForLoopNode();
}

ASTNode* Parser::parse(const TokenContainer& tokenizedSourceData) {
    // TODO: переделать алгоримт парсинга на алгоритм Дейкстры
    tokens = tokenizedSourceData;

    const Token& currentToken = tokens.getNextToken();

    ASTNode* parseResult;

    if (currentToken.Type == TokenType::Id) {
        const Token& nextToken = tokens.getNextToken();
        tokens.returnToken();
        tokens.returnToken();

        if (nextToken.Type == TokenType::Assign) {
            parseResult = parseAssign();
        } else {
            parseResult = expression();
        }
    } else if (currentToken.Type == TokenType::ROUND_BRACKET_START) {
        tokens.returnToken();
        parseResult = expression();
    } else if (currentToken.Type == TokenType::Num) {
        tokens.returnToken();
        parseResult = expression();
    } else if (currentToken.Type == TokenType::Bool) {
        tokens.returnToken();
        parseResult = expression();
    } else if (currentToken.Type == TokenType::Sub) {
        // unary minus
        tokens.returnToken();
        parseResult = expression();
    } else if (currentToken.Type == TokenType::DeclareId) {
        parseResult = parseDeclVar();
    } else if (currentToken.Type == TokenType::DeclareForLoop) {
        tokens.returnToken();
        parseResult = parseForLoop();
    } else {
        throw std::runtime_error("Invalid syntax");
    }

    if (matchParseComplete()) {
        return parseResult;
    } else {
        throw std::runtime_error("Invalid syntax");
    }
}

BinOpNode* Parser::createBinOpNode(BinOpType::ASTNodeBinOpType type, ASTNode* left, ASTNode* right) {
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

BoolNode* Parser::createBoolNode(bool value) {
    BoolNode* node = new BoolNode;
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

ForLoopNode* Parser::createForLoopNode() {
    ForLoopNode* node = new ForLoopNode;

    return node;
}

ASTNode* Parser::createEmptyNode() {
    ASTNode* node = new ASTNode;
    node->type = NodeType::Empty;

    return node;
}

double Parser::getNumTokenValue(const Token& numToken) {
    return std::stod(numToken.Value);
}

bool Parser::getBoolTokenValue(const Token& boolToken) {
    return static_cast<bool>(std::stoi(boolToken.Value));
}

bool Parser::matchParseComplete() {
    const Token& currentToken = tokens.getNextToken();

    if (currentToken.Type == TokenType::SEMICOLON) {
        return tokens.getNextToken().Type == TokenType::eof;
    } else {
        return currentToken.Type == TokenType::eof;
    }
}