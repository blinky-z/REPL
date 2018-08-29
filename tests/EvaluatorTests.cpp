#include "catch.hpp"
#include "tinyexpr.h"
#include "../Lexer.h"
#include "../Token.h"
#include "../Identifier.h"
#include "../Parser.h"
#include "../ASTNode.h"
#include "../Evaluator.h"

Lexer EvaluatorTestsLexer;
Parser EvaluatorTestsParser;

TEST_CASE("Addition Evaluation", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 + 2 + 4 + 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);
}

TEST_CASE("Subtraction Evaluation", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 - 2 - 4 - 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);
}

TEST_CASE("Multiplication Evaluation", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 * 2 * 4 * 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);
}

TEST_CASE("Division Evaluation", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 / 2 / 4 / 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);
}

TEST_CASE("Multiplication with Sum Parentheses Evaluation", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "(200 + 2) * (4 + 5)";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);
}

TEST_CASE("Multiplication with Difference Parentheses Evaluation", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "(200 - 2) * (4 - 5)";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);
}

TEST_CASE("Division with Sum Parentheses Evaluation", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "(200 + 2) / (4 + 5)";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);
}

TEST_CASE("Division with Difference Parentheses Evaluation", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "(200 - 2) / (4 - 5)";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);
}

TEST_CASE("Multiple operations: Sum, Mul, Sub", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 + 2 * 4 - 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);
}

TEST_CASE("Multiple operations: Sum, Div, Sub", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 + 2 / 4 - 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);
}

TEST_CASE("Multiple operations: Mul, Div, Sub", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 * 2 / 4 - 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);
}

TEST_CASE("Multiple operations: Mul, Div, Sum", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 * 2 / 4 + 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);
}

TEST_CASE("Multiple operations with single parentheses: Mul, Sub, Add", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 * (2 - 4) + 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);
}

TEST_CASE("Multiple operations with single parentheses: Div, Add, Mul", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 / (2 + 4) * 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);
}

TEST_CASE("Multiple operations with multiple parentheses", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "((200 + 2) / (2 - 4)) * 5 + 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);
}

TEST_CASE("Addition with using of variables: Declare & Assign variable in single expr", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 + 2 + 4 + 5";

    std::string expr1 = "var a = 200";
    std::string expr2 = "a + 2 + 4 + 5";

    std::string parseExpr1 = expr1;
    parseExpr1.push_back(EOF);
    std::string parseExpr2 = expr2;
    parseExpr2.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(parseExpr1);
    const TokenContainer& tokensExpr2 = EvaluatorTestsLexer.tokenize(parseExpr2);

    // declare var evaluate
    EvaluatorTestsEvaluator.Evaluate(EvaluatorTestsParser.parse(tokensExpr1));

    ASTNode* root = EvaluatorTestsParser.parse(tokensExpr2);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);
}

TEST_CASE("Declare & assign variable in single expr", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    double a = 200;

    std::string expr1 = "var a = 200";
    std::string expr2 = "a";

    expr1.push_back(EOF);
    expr2.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(expr1);
    const TokenContainer& tokensExpr2 = EvaluatorTestsLexer.tokenize(expr2);

    // declare and assign var evaluate
    EvaluatorTestsEvaluator.Evaluate(EvaluatorTestsParser.parse(tokensExpr1));

    ASTNode* root = EvaluatorTestsParser.parse(tokensExpr2);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(a);

    REQUIRE(result == properResult);
}

TEST_CASE("Declare & assign variable in separate expr", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    double a = 200;

    std::string expr1 = "var a";
    std::string expr2 = "a = 200";
    std::string expr3 = "a";

    expr1.push_back(EOF);
    expr2.push_back(EOF);
    expr3.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(expr1);
    const TokenContainer& tokensExpr2 = EvaluatorTestsLexer.tokenize(expr2);
    const TokenContainer& tokensExpr3 = EvaluatorTestsLexer.tokenize(expr3);

    // declare var evaluate
    EvaluatorTestsEvaluator.Evaluate(EvaluatorTestsParser.parse(tokensExpr1));

    // assign var evaluate
    EvaluatorTestsEvaluator.Evaluate(EvaluatorTestsParser.parse(tokensExpr2));

    ASTNode* root = EvaluatorTestsParser.parse(tokensExpr3);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(a);

    REQUIRE(result == properResult);
}

TEST_CASE("Addition operations with using of variables: Declare & Assign variable in separate expr", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 + 2 + 4 + 5";

    std::string expr1 = "var a";
    std::string expr2 = "a = 200";
    std::string expr3 = "a + 2 + 4 + 5";

    expr1.push_back(EOF);
    expr2.push_back(EOF);
    expr3.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(expr1);
    const TokenContainer& tokensExpr2 = EvaluatorTestsLexer.tokenize(expr2);
    const TokenContainer& tokensExpr3 = EvaluatorTestsLexer.tokenize(expr3);

    // declare var evaluate
    EvaluatorTestsEvaluator.Evaluate(EvaluatorTestsParser.parse(tokensExpr1));

    // assign var evaluate
    EvaluatorTestsEvaluator.Evaluate(EvaluatorTestsParser.parse(tokensExpr2));

    ASTNode* root = EvaluatorTestsParser.parse(tokensExpr3);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);
}

TEST_CASE("Assign variable to the other variable", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    double a = 200;
    double b = a;

    std::string expr1 = "var a = 200";
    std::string expr2 = "var b = a";
    std::string expr3 = "b";

    expr1.push_back(EOF);
    expr2.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(expr1);
    const TokenContainer& tokensExpr2 = EvaluatorTestsLexer.tokenize(expr2);
    const TokenContainer& tokensExpr3 = EvaluatorTestsLexer.tokenize(expr3);

    // declare var a evaluate
    EvaluatorTestsEvaluator.Evaluate(EvaluatorTestsParser.parse(tokensExpr1));

    // declare var b and assign to var a evaluate
    EvaluatorTestsEvaluator.Evaluate(EvaluatorTestsParser.parse(tokensExpr2));

    ASTNode* root = EvaluatorTestsParser.parse(tokensExpr3);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(b);

    REQUIRE(result == properResult);
}