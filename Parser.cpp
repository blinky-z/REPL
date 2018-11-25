#include "Parser.h"

ASTNode* Parser::parseExpression() {
    std::pair<std::queue<Token>, std::queue<ASTNode*>> expr = convertToReversePolish();

    std::queue<Token>& exprTokens = expr.first;
    std::queue<ASTNode*>& funcCallNodes = expr.second;

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
                case TokenType::FuncCall: {
                    nodeStack.push(funcCallNodes.front());
                    funcCallNodes.pop();
                    break;
                }
                default: {
                    errorExpected("Rvalue or Identifier", currentToken);
                }
            }
        } else {
            if (isBinaryOperator(currentToken)) { // binary operator
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
                        if (operand1->type == NodeType::Id) {
                            operationNode = createBinOpNode(BinOpType::OperatorAssign, operand1, operand2);
                            nodeStack.push(operationNode);
                        } else {
                            errorExpected("Identifier on the left side of assign operator");
                        }
                        break;
                    }
                    default: {
                        errorExpected("Binary operator", currentToken);
                    }
                }
            } else { // unary operator
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

std::string Parser::parseFuncName() {
    const Token& token = tokens.getNextToken();

    if (token.Type != TokenType::FuncCall) {
        errorExpected("Function name", token);
    }
    return token.Value;
}

std::vector<ASTNode*> Parser::parseFuncCallParams() {
    std::vector<ASTNode*> args;

    while (tokens.lookNextToken().Type != TokenType::ROUND_BRACKET_END) {
        ASTNode* arg = parseExpression();
        if (arg->type != NodeType::ConstNumber && arg->type != NodeType::ConstBool && arg->type != NodeType::Id &&
            arg->type != NodeType::BinOp && arg->type != NodeType::FuncCall) {
            throw std::runtime_error("Invalid parameter");
        }
        if (arg->type == NodeType::BinOp) {
            BinOpNode* binOpNode = static_cast<BinOpNode*>(arg);
            if (binOpNode->binOpType != BinOpType::OperatorPlus &&
                binOpNode->binOpType != BinOpType::OperatorMinus &&
                binOpNode->binOpType != BinOpType::OperatorMul &&
                binOpNode->binOpType != BinOpType::OperatorDiv &&
                binOpNode->binOpType != BinOpType::OperatorEqual &&
                binOpNode->binOpType != BinOpType::OperatorBoolAND &&
                binOpNode->binOpType != BinOpType::OperatorBoolOR &&
                binOpNode->binOpType != BinOpType::OperatorLess &&
                binOpNode->binOpType != BinOpType::OperatorGreater) {
                throw std::runtime_error("Invalid parameter");
            }
        }

        args.emplace_back(arg);
        if (tokens.lookNextToken().Type != TokenType::ROUND_BRACKET_END) {
            expect(",");
        }
    }

    return args;
}

FuncCallNode* Parser::parseFuncCall() {
    const std::string name = parseFuncName();
    expect("(");

    bool oldParenthesesControl = parenthesesControl;
    parenthesesControl = true;

    const std::vector<ASTNode*> args = parseFuncCallParams();

    parenthesesControl = oldParenthesesControl;

    expect(")");

    return createFuncCallNode(name, args);
}

ValueType::Type Parser::parseDeclFuncReturnType() {
    const Token& token = tokens.getNextToken();

    if (token.Value == "int") {
        return ValueType::Number;
    } else if (token.Value == "bool") {
        return ValueType::Bool;
    } else if (token.Value == "void") {
        return ValueType::Void;
    } else {
        errorExpected("Function return type");
        throw;
    }
}

std::vector<IdentifierNode*> Parser::parseDeclFuncParams() {
    std::vector<IdentifierNode*> args;

    Token nextToken;
    while ((nextToken = tokens.lookNextToken()).Type != TokenType::ROUND_BRACKET_END &&
           nextToken.Type != TokenType::eof && nextToken.Type != TokenType::NL) {
        expect("var");

        ValueType::Type paramType;
        const Token& nextToken = tokens.getNextToken();
        if (nextToken.Type == TokenType::IntType) {
            paramType = ValueType::Number;
        } else if (nextToken.Type == TokenType::BoolType) {
            paramType = ValueType::Bool;
        } else {
            errorExpected("Parameter type");
            throw;
        }

        IdentifierNode* id = parseIdentifier();
        id->valueType = paramType;
        args.emplace_back(id);
        if (tokens.lookNextToken().Type != TokenType::ROUND_BRACKET_END) {
            expect(",");
        }
    }

    return args;
}

FuncDeclNode* Parser::parseDeclFunc() {
    expect("func");

    ValueType::Type returnType = parseDeclFuncReturnType();
    const std::string& funcName = parseFuncName();

    expect("(");
    const std::vector<IdentifierNode*>& args = parseDeclFuncParams();
    expect(")");
    skipWhitespaces();

    BlockStmtNode* body = parseBlockStmt();

    return createDeclFuncNode(funcName, returnType, args, body);
}

ReturnStmtNode* Parser::parseReturnStmt() {
    expect("return");

    ASTNode* expr = nullptr;
    if (tokens.lookNextToken().Type != TokenType::NL && tokens.lookNextToken().Type != TokenType::CURLY_BRACKET_END) {
        expr = parseExpression();
    }

    return createReturnStmtNode(expr);
}

BreakStmtNode* Parser::parseBreakStmt() {
    tokens.getNextToken();
    return createBreakStmtNode();
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

IfStmtNode* Parser::parseElseIfStmt() {
    expect("if");
    expect("(");

    bool oldParenthesesControl = parenthesesControl;
    parenthesesControl = true;
    ASTNode* condition = parseExpression();
    parenthesesControl = oldParenthesesControl;

    expect(")");
    skipWhitespaces();

    BlockStmtNode* body = parseBlockStmt();
    std::vector<IfStmtNode*> elseIfStmts;
    BlockStmtNode* elseBody = nullptr;

    return createIfStmtNode(condition, body, elseIfStmts, elseBody);
}

IfStmtNode* Parser::parseIfStmt() {
    expect("if");
    expect("(");

    bool oldParenthesesControl = parenthesesControl;
    parenthesesControl = true;
    ASTNode* condition = parseExpression();
    parenthesesControl = oldParenthesesControl;

    expect(")");
    skipWhitespaces();

    BlockStmtNode* body = parseBlockStmt();
    std::vector<IfStmtNode*> elseIfStmts;
    BlockStmtNode* elseBody = nullptr;

    while (true) {
        skipWhitespaces();
        if (tokens.lookNextToken().Type == TokenType::ElseStmt) {
            tokens.getNextToken();
            if (tokens.lookNextToken().Type == TokenType::IfStmt) {
                elseIfStmts.emplace_back(parseElseIfStmt());
            } else {
                skipWhitespaces();
                elseBody = parseBlockStmt();
                break;
            }
        } else {
            tokens.returnToken(); // return ending if instruction newline that we skipped calling SkipWhitespaces()
            break;
        }
    }

    return createIfStmtNode(condition, body, elseIfStmts, elseBody);
}

ASTNode* Parser::parseForLoopInit() {
    ASTNode* init;

    const Token& currentToken = tokens.getNextToken();
    switch (currentToken.Type) {
        case TokenType::DeclareId: {
            tokens.returnToken();
            init = parseDeclVar();
            break;
        }
        case TokenType::Id: {
            const Token& nextOp = tokens.lookNextToken();
            if (nextOp.Type != TokenType::Assign) {
                throw std::runtime_error("Invalid for loop initialization expression");
            }
            tokens.returnToken();
            init = parseExpression();
            break;
        }
        case TokenType::SEMICOLON: {
            tokens.returnToken();
            init = nullptr;
            break;
        }
        default: {
            errorExpected("Variable declaration/assignment or empty initialization");
            throw;
        }
    }

    return init;
}

ForLoopNode* Parser::parseForLoop() {
    expect("for");
    expect("(");

    ASTNode* init;
    ASTNode* condition;
    ASTNode* inc;

    if (tokens.lookNextToken().Type != TokenType::SEMICOLON) {
        init = parseForLoopInit();
    } else {
        init = nullptr;
    }
    expect(";");

    if (tokens.lookNextToken().Type != TokenType::SEMICOLON) {
        condition = parseExpression();
    } else {
        condition = nullptr;
    }
    expect(";");

    if (tokens.lookNextToken().Type != TokenType::ROUND_BRACKET_END) {
        bool oldParenthesesControl = parenthesesControl;
        parenthesesControl = true;
        inc = parseExpression();
        parenthesesControl = oldParenthesesControl;
    } else {
        inc = nullptr;
    }

    expect(")");
    skipWhitespaces();

    BlockStmtNode* body = parseBlockStmt();

    return createForLoopNode(init, condition, inc, body);
}

ASTNode* Parser::parseStatement() {
    ASTNode* parseResult;

    const Token& currentToken = tokens.lookNextToken();
    switch (currentToken.Type) {
        case TokenType::Id:
        case TokenType::FuncCall:
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
        case TokenType::ForLoopStmt: {
            parseResult = parseForLoop();
            break;
        }
        case TokenType::IfStmt: {
            parseResult = parseIfStmt();
            break;
        }
        case TokenType::DeclareFunc: {
            parseResult = parseDeclFunc();
            break;
        }
        case TokenType::Return: {
            parseResult = parseReturnStmt();
            break;
        }
        case TokenType::Break: {
            parseResult = parseBreakStmt();
            break;
        }
        default: {
            errorExpected("Statement", currentToken);
            throw;
        }
    }

    if (tokens.lookNextToken().Type != TokenType::CURLY_BRACKET_END) {
        expect("\n");
    }
    skipWhitespaces();

    return parseResult;
}

ProgramTranslationNode* Parser::parse(const TokenContainer& sourceData) {
    tokens = sourceData;

    ProgramTranslationNode* ast = new ProgramTranslationNode();

    skipWhitespaces();
    while (tokens.lookNextToken().Type != TokenType::eof) {
        ASTNode* currentStatement = parseStatement();
        ast->statements.emplace_back(currentStatement);
    }

    return ast;
}

BinOpNode* Parser::createBinOpNode(BinOpType::ASTNodeBinOpType type, ASTNode* left, ASTNode* right) {
    BinOpNode* node = new BinOpNode;
    node->binOpType = type;
    node->left = left;
    node->right = right;

    return node;
}

ConstNumberNode* Parser::createNumberNode(double value) {
    ConstNumberNode* node = new ConstNumberNode;
    node->value = value;

    return node;
}

ConstBoolNode* Parser::createBoolNode(bool value) {
    ConstBoolNode* node = new ConstBoolNode;
    node->value = value;

    return node;
}

ReturnStmtNode* Parser::createReturnStmtNode(ASTNode* expr) {
    ReturnStmtNode* node = new ReturnStmtNode;
    node->expression = expr;

    return node;
}

BreakStmtNode* Parser::createBreakStmtNode() {
    BreakStmtNode* node = new BreakStmtNode;
    return node;
}

IdentifierNode* Parser::createIdentifierNode(const std::string& name) {
    IdentifierNode* node = new IdentifierNode;
    node->name = name;

    return node;
}

FuncCallNode* Parser::createFuncCallNode(const std::string& name, const std::vector<ASTNode*>& args) {
    FuncCallNode* node = new FuncCallNode;
    node->name = name;
    node->args = args;
    node->argsSize = args.size();

    return node;
}

FuncDeclNode* Parser::createDeclFuncNode(const std::string& name,
                                         ValueType::Type returnType,
                                         const std::vector<IdentifierNode*>& args,
                                         BlockStmtNode* body) {
    FuncDeclNode* node = new FuncDeclNode;
    node->name = name;
    node->returnType = returnType;
    node->args = args;
    node->argsSize = args.size();
    node->body = body;

    return node;
}

DeclVarNode* Parser::createDeclVarNode(IdentifierNode* id, ASTNode* expr) {
    DeclVarNode* node = new DeclVarNode;
    node->id = id;
    node->expr = expr;

    return node;
}

BlockStmtNode* Parser::createBlockStmtNode(const std::vector<ASTNode*>& statements) {
    BlockStmtNode* node = new BlockStmtNode;
    node->stmtList = statements;

    return node;
}

IfStmtNode*
Parser::createIfStmtNode(ASTNode* condition, BlockStmtNode* stmtList, std::vector<IfStmtNode*> elseIfStmts,
                         BlockStmtNode* elseStmtList) {
    IfStmtNode* node = new IfStmtNode;
    node->condition = condition;
    node->body = stmtList;
    node->elseIfStmts = elseIfStmts;
    node->elseBody = elseStmtList;

    return node;
}

ForLoopNode* Parser::createForLoopNode(ASTNode* init, ASTNode* cond, ASTNode* inc, BlockStmtNode* stmtList) {
    ForLoopNode* node = new ForLoopNode;
    node->init = init;
    node->condition = cond;
    node->inc = inc;
    node->body = stmtList;

    return node;
}

double Parser::getNumTokenValue(const Token& numToken) {
    return std::stod(numToken.Value);
}

bool Parser::getBoolTokenValue(const Token& boolToken) {
    return static_cast<bool>(std::stoi(boolToken.Value));
}

std::pair<std::queue<Token>, std::queue<ASTNode*>> Parser::convertToReversePolish() {
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
    std::queue<ASTNode*> funcCallNodes;

    Token token;
    while ((token = tokens.getNextToken()).Type != TokenType::NL && token.Type != TokenType::CURLY_BRACKET_START &&
           token.Type != TokenType::CURLY_BRACKET_END && token.Type != TokenType::SEMICOLON &&
           token.Type != TokenType::Comma && token.Type != TokenType::eof) {
        if (token.Type == TokenType::Number || token.Type == TokenType::Bool || token.Type == TokenType::Id ||
            token.Type == TokenType::FuncCall) {
            expr.push(token);
            if (token.Type == TokenType::FuncCall) {
                tokens.returnToken();
                funcCallNodes.push(parseFuncCall());
            }
        } else if (isOperator(token)) {
            const Token& curOp = token;

            while (!opStack.empty() && isOperator(opStack.top())) {
                const Token& topOp = opStack.top();
                if ((opPrecedence[curOp.Value] == opPrecedence[topOp.Value] && isOpLeftAssociative(topOp)) ||
                    (opPrecedence[curOp.Value] < opPrecedence[topOp.Value])) {
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
                if (parenthesesControl) {
                    break;
                } else {
                    errorExpected("Left Round Bracket");
                }
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

    return std::make_pair(expr, funcCallNodes);
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

bool Parser::isOpLeftAssociative(const Token& token) {
    return token.Type != TokenType::Assign && token.Type != TokenType::UnaryMinus;
}

void Parser::expect(const std::string& expected) {
    const Token& currentToken = tokens.getNextToken();

    if (currentToken.Value != expected) {
        errorExpected(expected, currentToken);
    }
}

void Parser::errorExpected(const std::string& expected, const Token& foundTok) {
    std::string errMsg;
    std::string formatExpected = expected;

    if (expected == "\n") {
        formatExpected = "newline";
    }

    if (foundTok.Type == TokenType::NL) {
        errMsg = "Expected " + formatExpected + ", found newline";
    } else if (foundTok.Type == TokenType::eof) {
        errMsg = "Expected " + formatExpected + ", found EOF";
    } else {
        errMsg = "Expected " + formatExpected + ", found \"" + foundTok.Value + "\"";
    }
    throw std::runtime_error(errMsg);
}

void Parser::errorExpected(const std::string& expected) {
    std::string errMsg = "Expected " + expected + "\n";

    throw std::runtime_error(errMsg);
}

void Parser::skipWhitespaces() {
    while (tokens.lookNextToken().Type == TokenType::NL) {
        tokens.getNextToken();
    }
}
