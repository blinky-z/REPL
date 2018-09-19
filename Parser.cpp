#include "Parser.h"

ASTNode* Parser::parseExpression() {
    std::queue<Token> exprTokens = convertToReversePolish();

    std::stack<ASTNode*> nodeStack;

    Token currentToken;
    while (!exprTokens.empty()) {
        currentToken = exprTokens.front();
        exprTokens.pop();

        if (!isOperator(currentToken)) {
            switch (currentToken.Type) {
                case TokenType::Number: {
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
                    errorExpected("Rvalue or Identifier", currentToken);
                }
            }
        } else {
            if (isBinaryOperator(currentToken)) {
                ASTNode* operationNode;

                // since stack is LIFO need to get 2nd operand first to not break the order of operands
                ASTNode* operand1;
                ASTNode* operand2;

                if (nodeStack.size() >= 2) {
                    operand2 = nodeStack.top();
                    nodeStack.pop();
                    operand1 = nodeStack.top();
                    nodeStack.pop();
                } else {
                    throw std::runtime_error("Invalid Syntax: Not enough operands for binary operation");
                }

                switch (currentToken.Type) {
                    case TokenType::Add: {
                        operationNode = createBinOpNode(BinOpType::OperatorPlus, operand1, operand2);
                        nodeStack.push(operationNode);
                        break;
                    }
                    case TokenType::Sub: {
                        operationNode = createBinOpNode(BinOpType::OperatorMinus, operand1, operand2);
                        nodeStack.push(operationNode);
                        break;
                    }
                    case TokenType::Mul: {
                        operationNode = createBinOpNode(BinOpType::OperatorMul, operand1, operand2);
                        nodeStack.push(operationNode);
                        break;
                    }
                    case TokenType::Div: {
                        operationNode = createBinOpNode(BinOpType::OperatorDiv, operand1, operand2);
                        nodeStack.push(operationNode);
                        break;
                    }
                    case TokenType::BoolAND: {
                        operationNode = createBinOpNode(BinOpType::OperatorBoolAND, operand1, operand2);
                        nodeStack.push(operationNode);
                        break;
                    }
                    case TokenType::BoolOR: {
                        operationNode = createBinOpNode(BinOpType::OperatorBoolOR, operand1, operand2);
                        nodeStack.push(operationNode);
                        break;
                    }
                    case TokenType::Equal: {
                        operationNode = createBinOpNode(BinOpType::OperatorEqual, operand1, operand2);
                        nodeStack.push(operationNode);
                        break;
                    }
                    case TokenType::LESS: {
                        operationNode = createBinOpNode(BinOpType::OperatorLess, operand1, operand2);
                        nodeStack.push(operationNode);
                        break;
                    }
                    case TokenType::GREATER: {
                        operationNode = createBinOpNode(BinOpType::OperatorGreater, operand1, operand2);
                        nodeStack.push(operationNode);
                        break;
                    }
                    case TokenType::Assign: {
                        operationNode = createBinOpNode(BinOpType::OperatorAssign, operand1, operand2);
                        nodeStack.push(operationNode);
                        break;
                    }
                    default: {
                        errorExpected("Binary operator", currentToken);
                    }
                }
            } else {
                ASTNode* operationNode;

                ASTNode* operand;

                if (nodeStack.size() >= 1) {
                    operand = nodeStack.top();
                    nodeStack.pop();
                } else {
                    throw std::runtime_error("Invalid Syntax: Not enough operands for unary operation");
                }

                switch (currentToken.Type) {
                    case TokenType::UnaryMinus: {
                        operationNode = createBinOpNode(BinOpType::OperatorMinus, createNumberNode(0), operand);
                        nodeStack.push(operationNode);
                        break;
                    }
                    default: {
                        errorExpected("Unary operator", currentToken);
                    }
                }
            }
        }
    }

    if (nodeStack.empty()) {
        errorExpected("Expression");
    }
    if (nodeStack.size() > 1) {
        throw std::runtime_error("Invalid expression");
    }

    return nodeStack.top();
}

DeclVarNode* Parser::parseDeclVar() {
    expect("var");

    IdentifierNode* id = parseIdentifier();
    ASTNode* expr = nullptr;

    const Token& nextToken = tokens.getNextToken();
    if (nextToken.Type == TokenType::Assign) {
        expr = parseExpression();
    } else {
        tokens.returnToken();
    }

    return createDeclVarNode(id, expr);
}

IdentifierNode* Parser::parseIdentifier() {
    const Token& token = tokens.getNextToken();

    if (token.Type != TokenType::Id) {
        errorExpected("identifier", token);
    }
    return createIdentifierNode(token.Value);
}

BlockStmtNode* Parser::parseBlockStmt() {
    expect("{");

    std::vector<ASTNode*> stmtList;
    Token nextToken;
    while ((nextToken = tokens.lookNextToken()).Type != TokenType::CURLY_BRACKET_END &&
           nextToken.Type != TokenType::eof) {
        stmtList.emplace_back(parseStatement());
    }

    expect("}");

    return createBlockStmtNode(stmtList);
}

IfStmtNode* Parser::parseIfStmt() {
    expect("if");

    expect("(");

    // need to return left round bracket for proper parsing of expression with convertToReversePolish() func
    // convertToReversePolish() will match both left and right round and checks if they are missed
    tokens.returnToken();
    ASTNode* condition = parseExpression();

    BlockStmtNode* body = parseBlockStmt();

    return createIfStmtNode(condition, body);
}

ForLoopNode* Parser::parseForLoop() {
    // TODO: имплементировать For Loop

    return createForLoopNode();
}

ASTNode* Parser::parseStatement() {
    ASTNode* parseResult;

    const Token& currentToken = tokens.lookNextToken();
    switch (currentToken.Type) {
        case TokenType::Id:
        case TokenType::ROUND_BRACKET_START:
        case TokenType::Number:
        case TokenType::Bool:
        case TokenType::UnaryMinus: {
            parseResult = parseExpression();
            break;
        }
        case TokenType::DeclareId: {
            parseResult = parseDeclVar();
            break;
        }
        case TokenType::DeclareForLoop: {
            parseResult = parseForLoop();
            break;
        }
        case TokenType::IfStmt: {
            parseResult = parseIfStmt();
            break;
        }
        default: {
            errorExpected("Statement", currentToken);
            throw;
        }
    }

    matchParseComplete();
    return parseResult;
}

ASTNode* Parser::parse(const TokenContainer& tokenizedSourceData) {
    tokens = tokenizedSourceData;

    ASTNode* ast = parseStatement();

    expect("EOF");

    return ast;
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

BlockStmtNode* Parser::createBlockStmtNode(const std::vector<ASTNode*> statements) {
    BlockStmtNode* node = new BlockStmtNode;
    node->stmtList = statements;

    return node;
}

IfStmtNode* Parser::createIfStmtNode(ASTNode* condition, BlockStmtNode* statement) {
    IfStmtNode* node = new IfStmtNode;
    node->condition = condition;
    node->body = statement;

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

void Parser::matchParseComplete() {
    const Token& currentToken = tokens.getNextToken();

    if (currentToken.Type != TokenType::NL) {
        errorExpected("newline", currentToken);
    }
}

std::queue<Token> Parser::convertToReversePolish() {
    // TODO: написать тесты на данный алгоритм
    static std::unordered_map<std::string, int> opPrecedence = {
            std::pair<std::string, int>("=", 1),
            std::pair<std::string, int>("||", 2),
            std::pair<std::string, int>("&&", 3),
            std::pair<std::string, int>("==", 4),
            std::pair<std::string, int>("<", 5),
            std::pair<std::string, int>(">", 5),
            std::pair<std::string, int>("+", 6),
            std::pair<std::string, int>("-", 6),
            std::pair<std::string, int>("*", 7),
            std::pair<std::string, int>("/", 7),
            std::pair<std::string, int>("u-", 8),
    };

    std::stack<Token> opStack;
    std::queue<Token> expr;

    Token token;
    while ((token = tokens.getNextToken()).Type != TokenType::NL && token.Type != TokenType::CURLY_BRACKET_START &&
           token.Type != TokenType::CURLY_BRACKET_END) {
        if (token.Type == TokenType::Number || token.Type == TokenType::Bool || token.Type == TokenType::Id) {
            expr.push(token);
        } else if (isOperator(token)) {
            const Token& curOp = token;

            while (!opStack.empty() && isOperator(opStack.top())) {
                const Token& topOp = opStack.top();

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
                if (opPrecedence[curOp.Value] <= opPrecedence[topOp.Value]) {
                    expr.push(opStack.top());
                    opStack.pop();
                    continue;
                }
                break;
            }
            opStack.push(token);
        } else if (token.Type == TokenType::ROUND_BRACKET_START) {
            opStack.push(token);
        } else if (token.Type == TokenType::ROUND_BRACKET_END) {
            Token topToken;
            while (!opStack.empty() && ((topToken = opStack.top()).Type != TokenType::ROUND_BRACKET_START)) {
                opStack.pop();
                expr.push(topToken);
            }

            if (topToken.Type == TokenType::ROUND_BRACKET_START) {
                opStack.pop(); // remove left bracket
            } else {
                errorExpected("Left Round Bracket");
            }
        } else {
            throw std::runtime_error("Unexpected token: " + token.Value);
        }
    }
    tokens.returnToken();

    while (!opStack.empty()) {
        const Token& topToken = opStack.top();
        opStack.pop();

        if (topToken.Type == TokenType::ROUND_BRACKET_START) {
            errorExpected("Right Round Bracket");
        }

        expr.push(topToken);
    }

    return expr;
}

bool Parser::isOperator(const Token& token) {
    return token.Type == TokenType::Add || token.Type == TokenType::Sub || token.Type == TokenType::Mul ||
           token.Type == TokenType::Div || token.Type == TokenType::BoolOR || token.Type == TokenType::BoolAND ||
           token.Type == TokenType::Equal || token.Type == TokenType::UnaryMinus || token.Type == TokenType::Assign ||
           token.Type == TokenType::LESS || token.Type == TokenType::GREATER;
}

bool Parser::isBinaryOperator(const Token& token) {
    return token.Type != TokenType::UnaryMinus;
}

void Parser::expect(const std::string& expected) {
    const Token& currentToken = tokens.getNextToken();

    if (currentToken.Value != expected) {
        errorExpected(expected, currentToken);
    }
}

void Parser::errorExpected(const std::string& expected, const Token& foundTok) {
    std::string errMsg;

    if (foundTok.Type == TokenType::NL) {
        errMsg = "Expected " + expected + ", found newline";
    } else if (foundTok.Type == TokenType::eof) {
        errMsg = "Expected " + expected + ", found EOF";
    } else {
        errMsg = "Expected " + expected + ", found \"" + foundTok.Value + "\"";
    }
    throw std::runtime_error(errMsg);
}

void Parser::errorExpected(const std::string& expected) {
    std::string errMsg = "Expected " + expected + "\n";

    throw std::runtime_error(errMsg);
}