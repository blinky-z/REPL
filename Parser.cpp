#include "Parser.h"

ASTNode* Parser::parseExpression() {
    std::queue<Token> outQueue = convertExpr();

    std::stack<ASTNode*> nodeStack;

    Token currentToken;
    while (!outQueue.empty()) {
        currentToken = outQueue.front();
        outQueue.pop();

        if (!isOperator(currentToken)) {
            switch (currentToken.Type) {
                case TokenType::Num: {
                    nodeStack.push(createNumberNode(getNumTokenValue(currentToken)));
                    break;
                }
                case TokenType::Bool: {
                    nodeStack.push(createBoolNode(getBoolTokenValue(currentToken)));
                    break;
                }
                case TokenType::Id: {
                    nodeStack.push(createIdentifierNode(currentToken.Value));
                    break;
                }
                default: {
                    throw std::runtime_error("Invalid syntax");
                }
            }
        } else {
            if (isBinaryOperator(currentToken)) {
                ASTNode* node;

                // since stack is LIFO need to get 2nd operand first to not break the order of operands
                ASTNode* operand2 = nodeStack.top();
                nodeStack.pop();
                ASTNode* operand1 = nodeStack.top();
                nodeStack.pop();

                switch (currentToken.Type) {
                    case TokenType::Add: {
                        node = createBinOpNode(BinOpType::OperatorPlus, operand1, operand2);
                        nodeStack.push(node);
                        break;
                    }
                    case TokenType::Sub: {
                        node = createBinOpNode(BinOpType::OperatorMinus, operand1, operand2);
                        nodeStack.push(node);
                        break;
                    }
                    case TokenType::Mul: {
                        node = createBinOpNode(BinOpType::OperatorMul, operand1, operand2);
                        nodeStack.push(node);
                        break;
                    }
                    case TokenType::Div: {
                        node = createBinOpNode(BinOpType::OperatorDiv, operand1, operand2);
                        nodeStack.push(node);
                        break;
                    }
                    case TokenType::BoolOR: {
                        node = createBinOpNode(BinOpType::OperatorBoolOR, operand1, operand2);
                        nodeStack.push(node);
                        break;
                    }
                    case TokenType::BoolAND: {
                        node = createBinOpNode(BinOpType::OperatorBoolAND, operand1, operand2);
                        nodeStack.push(node);
                        break;
                    }
                    case TokenType::Equal: {
                        node = createBinOpNode(BinOpType::OperatorEqual, operand1, operand2);
                        nodeStack.push(node);
                        break;
                    }
                    default: {
                        throw std::runtime_error("Invalid syntax");
                    }
                }
            } else {
                ASTNode* node;

                ASTNode* operand = nodeStack.top();
                nodeStack.pop();

                switch (currentToken.Type) {
                    case TokenType::UnaryMinus: {
                        node = createBinOpNode(BinOpType::OperatorMinus, createNumberNode(0), operand);
                        nodeStack.push(node);
                        break;
                    }
                    default: {
                        throw std::runtime_error("Invalid syntax");
                    }
                }
            }
        }
    }

    return nodeStack.top();
}

ASTNode* Parser::parseAssign() {
    const Token& id = tokens.getNextToken();
    const std::string& idName = id.Value;

    // skip '=' token
    tokens.getNextToken();

    IdentifierNode* idNode = createIdentifierNode(idName);
    ASTNode* expr = parseExpression();

    return createBinOpNode(BinOpType::OperatorAssign, idNode, expr);
}

ASTNode* Parser::parseDeclVar() {
    ASTNode* expr = nullptr;

    IdentifierNode* idNode = static_cast<IdentifierNode*>(parseId());

    const Token& nextToken = tokens.getNextToken();
    if (nextToken.Type == TokenType::Assign) {
        expr = parseExpression();
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
            parseResult = parseExpression();
        }
    } else if (currentToken.Type == TokenType::ROUND_BRACKET_START) {
        tokens.returnToken();
        parseResult = parseExpression();
    } else if (currentToken.Type == TokenType::Num) {
        tokens.returnToken();
        parseResult = parseExpression();
    } else if (currentToken.Type == TokenType::Bool) {
        tokens.returnToken();
        parseResult = parseExpression();
    } else if (currentToken.Type == TokenType::UnaryMinus) {
        tokens.returnToken();
        parseResult = parseExpression();
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

std::queue<Token> Parser::convertExpr() {
    static std::unordered_map<std::string, int> opPrecedence;
    opPrecedence["||"] = 1;
    opPrecedence["&&"] = 2;
    opPrecedence["=="] = 3;
    opPrecedence["+"] = 4;
    opPrecedence["-"] = 4;
    opPrecedence["*"] = 5;
    opPrecedence["/"] = 5;
    opPrecedence["u-"] = 6;

    std::stack<Token> stack;
    std::queue<Token> RPNExpr;

    Token token;

    while ((token = tokens.getNextToken()).Type != TokenType::eof) {
        if (token.Type == TokenType::Num || token.Type == TokenType::Bool || token.Type == TokenType::Id) {
            RPNExpr.push(token);
        } else if (isOperator(token)) {
            const Token& op1 = token;

            while (!stack.empty() && isOperator(stack.top())) {
                const Token& op2 = stack.top();

                // из стека нужно вытолкнуть те операторы, которые имеют приоритет выше или равный текущему оператору
                // иначе нарушится порядок выполнения операций
                // дело в том, что наверху стека лежат операторы, которые должны выполняться первее других
                // ведь операторы ложатся в выходной результат с верха стека, и именно они будут выполняться первыми
                // если встретился оператор, имеющий приоритет ниже или равный операторам, лежащим на стеке,
                // значит нужно обязательно вытолкнуть с стека операторы, имеющие приоритет выше, чтобы они выполнились
                // первее
                // например, в выражении expr(2 / 4 + 5) парсинг будет происходить так:
                // 1) 2 4
                // 2) встретился оператор /, он ложится в стек
                // result: 2, 4 | operands stack: /
                // 3) встретился оператор +, он имеет приоритет меньший, чем у того, который лежит наверху стека
                // если бы мы не вытолкнули оператор /, то нарушился бы порядок вычислений, то есть стек был бы такой:
                // result: 2, 4, 5 | operands stack: + (top)
                //                                   /
                //
                // но ведь первые операнды 2, 4 должны были использоваться ранее с оператором /, который имеет приоритет
                // выше, однако сначала со стека выйдет +, и только потом /: 2, 4, 5, +, /, что в корне неверно
                // именно для этого нужно выталкивать операторы, имеющие приоритет выше, чтобы не нарушить порядок
                // вычислений
                // и наоборот: если текущий оператор имеет приоритет выше, чем лежаший наверху стека операторов, то
                // данный оператор должен ложиться наверх стека, не выталкивая со стека ничего, чтобы данный оператор
                // был вычислен первее (ведь он будет вытолкнут со стека первее)
                if (opPrecedence[op1.Value] <= opPrecedence[op2.Value]) {
                    RPNExpr.push(stack.top());
                    stack.pop();
                    continue;
                }
                break;
            }
            stack.push(token);
        } else if (token.Type == TokenType::ROUND_BRACKET_START) {
            stack.push(token);
        } else if (token.Type == TokenType::ROUND_BRACKET_END) {
            try {
                Token topToken;
                while ((topToken = stack.top()).Type != TokenType::ROUND_BRACKET_START) {
                    stack.pop();
                    RPNExpr.push(topToken);
                }
                stack.pop(); // remove left bracket
            } catch (std::exception) {
                throw std::runtime_error("Invalid syntax");
            }
        } else {
            throw std::runtime_error("Invalid syntax");
        }
    }
    tokens.returnToken();

    while (!stack.empty()) {
        const Token& topToken = stack.top();
        stack.pop();
        RPNExpr.push(topToken);
    }

    return RPNExpr;
}

bool Parser::isOperator(const Token& token) {
    return token.Type == TokenType::Add || token.Type == TokenType::Sub ||
           token.Type == TokenType::Mul || token.Type == TokenType::Div || token.Type == TokenType::BoolOR ||
           token.Type == TokenType::BoolAND || token.Type == TokenType::Equal || token.Type == TokenType::UnaryMinus;
}

bool Parser::isLeftAssociative(const Token& token) {
    return token.Type != TokenType::UnaryMinus && token.Type != TokenType::Assign;
}

bool Parser::isBinaryOperator(const Token& token) {
    return token.Type != TokenType::UnaryMinus;
}