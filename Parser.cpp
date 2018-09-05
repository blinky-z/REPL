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
        // convert unary minus (-x) into (0 - x) expr
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

ASTNode* Parser::parseMathExpr() {
    return createMathExprNode(expression());
}

ASTNode* Parser::expressionBool() {
    ASTNode* termNode = termBool();

    const Token& nextOp = tokens.getNextToken();
    tokens.returnToken();
    if (nextOp.Type == TokenTypes::BoolOR) {
        return expressionTailBool(termNode);
    } else {
        return termNode;
    }
}

ASTNode* Parser::expressionTailBool(ASTNode* lvalue) {
    ASTNode* rvalue;
    ASTNode* expressionTailNode;

    const Token& token = tokens.getNextToken();

    if (token.Type == TokenTypes::BoolOR) {
        rvalue = termBool();

        ASTNode* currentBinOpNode = createBinOpNode(OperatorBoolOR, lvalue, rvalue);
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

ASTNode* Parser::termBool() {
    ASTNode* factorNode = factorBool();

    const Token& nextOp = tokens.getNextToken();
    tokens.returnToken();
    if (nextOp.Type == TokenTypes::BoolAND) {
        return termTailBool(factorNode);
    } else {
        return factorNode;
    }
}

ASTNode* Parser::termTailBool(ASTNode* lvalue) {
    ASTNode* rvalue;
    ASTNode* termTailNode;

    const Token& token = tokens.getNextToken();

    if (token.Type == TokenTypes::BoolAND) {
        rvalue = factorBool();

        ASTNode* currentBinOpNode = createBinOpNode(OperatorBoolAND, lvalue, rvalue);
        termTailNode = termTailBool(currentBinOpNode);

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

ASTNode* Parser::factorBool() {
    const Token& token = tokens.getNextToken();

    if (token.Type == TokenTypes::Bool) {
        bool value = getBoolTokenValue(token);

        return createBoolNode(value);
    } else if (token.Type == TokenTypes::ROUND_BRACKET_START) {
        ASTNode* result = expressionBool();

        if (tokens.getNextToken().Type == TokenTypes::ROUND_BRACKET_END) {
            return result;
        } else {
            throw std::runtime_error("Invalid syntax");
        }
    } else {
        throw std::runtime_error("Invalid syntax");
    }
}

ASTNode* Parser::parseBoolExpr() {
    return createBoolExprNode(expressionBool());
}

ASTNode* Parser::parseExpr() {
    const std::vector<Token> exprTokens = tokens.getTokens();

    for (const auto& currentToken : exprTokens) {
        if (currentToken.Type == TokenTypes::Bool || currentToken.Type == TokenTypes::BoolOR ||
            currentToken.Type == TokenTypes::BoolAND) {
            return parseBoolExpr();
        } else if (currentToken.Type == TokenTypes::Num || currentToken.Type == TokenTypes::Add ||
                   currentToken.Type == TokenTypes::Sub || currentToken.Type == TokenTypes::Mul ||
                   currentToken.Type == TokenTypes::Div) {
            return parseMathExpr();
        }
    }
    if (tokens.getNextToken().Type == TokenTypes::Id && tokens.getNextToken().Type == TokenTypes::eof) {
        tokens.returnToken();
        tokens.returnToken();
        return parseId();
    }
    throw std::runtime_error("Invalid Syntax");
}

ASTNode* Parser::parseAssign() {
    const Token& id = tokens.getNextToken();
    const std::string& idName = id.Value;

    // skip Assign token
    tokens.getNextToken();

    IdentifierNode* idNode = createIdentifierNode(idName);
    ASTNode* expr = parseExpr();

    return createBinOpNode(OperatorAssign, idNode, expr);
}

ASTNode* Parser::parseDeclVar() {
    ASTNode* expr = nullptr;

    const Token& id = tokens.getNextToken();
    const std::string& idName = id.Value;

    IdentifierNode* idNode = createIdentifierNode(idName);

    const Token& nextToken = tokens.getNextToken();
    if (nextToken.Type == TokenTypes::Assign) {
        expr = parseExpr();
    } else {
        tokens.returnToken();
    }

    return createDeclVarNode(idNode, expr);
}

ASTNode* Parser::parseId() {
    const Token& id = tokens.getNextToken();

    return createIdentifierNode(id.Value);
}

ASTNode* Parser::parseForLoop() {
    // TODO: имплементировать For Loop

    return createForLoopNode();
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
            parseResult = parseExpr();
        }
    } else if (currentToken.Type == TokenTypes::ROUND_BRACKET_START) {
        tokens.returnToken();
        parseResult = parseExpr();
    } else if (currentToken.Type == TokenTypes::Num) {
        tokens.returnToken();
        parseResult = parseMathExpr();
    } else if (currentToken.Type == TokenTypes::Bool) {
        tokens.returnToken();
        parseResult = parseBoolExpr();
    } else if (currentToken.Type == TokenTypes::Sub) {
        // unary minus
        tokens.returnToken();
        parseResult = parseMathExpr();
    } else if (currentToken.Type == TokenTypes::DeclareId) {
        parseResult = parseDeclVar();
    } else if (currentToken.Type == TokenTypes::DeclareForLoop) {
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

MathExprNode* Parser::createMathExprNode(ASTNode* expr) {
    MathExprNode* node = new MathExprNode;

    node->expr = expr;

    return node;
}

BoolExprNode* Parser::createBoolExprNode(ASTNode* expr) {
    BoolExprNode* node = new BoolExprNode;

    node->expr = expr;

    return node;
}

ForLoopNode* Parser::createForLoopNode() {
    ForLoopNode* node = new ForLoopNode;

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

bool Parser::getBoolTokenValue(const Token& boolToken) {
    return static_cast<bool>(std::stoi(boolToken.Value));
}

bool Parser::matchParseComplete() {
    const Token& currentToken = tokens.getNextToken();

    if (currentToken.Type == TokenTypes::SEMICOLON) {
        return tokens.getNextToken().Type == TokenTypes::eof;
    } else {
        return currentToken.Type == TokenTypes::eof;
    }
}