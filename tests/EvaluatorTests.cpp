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

    delete root;
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

    delete root;
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

    delete root;
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

    delete root;
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

    delete root;
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

    delete root;
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

    delete root;
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

    delete root;
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

    delete root;
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

    delete root;
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

    delete root;
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

    delete root;
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

    delete root;
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

    delete root;
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

    delete root;
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
    ASTNode* root1 = EvaluatorTestsParser.parse(tokensExpr1);
    EvaluatorTestsEvaluator.Evaluate(root1);

    ASTNode* root2 = EvaluatorTestsParser.parse(tokensExpr2);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root2);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);

    delete root1;
    delete root2;
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
    ASTNode* root1 = EvaluatorTestsParser.parse(tokensExpr1);
    EvaluatorTestsEvaluator.Evaluate(root1);

    ASTNode* root2 = EvaluatorTestsParser.parse(tokensExpr2);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root2);
    std::string properResult = std::to_string(a);

    REQUIRE(result == properResult);

    delete root1;
    delete root2;
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
    ASTNode* root1 = EvaluatorTestsParser.parse(tokensExpr1);
    EvaluatorTestsEvaluator.Evaluate(root1);

    // assign var evaluate
    ASTNode* root2 = EvaluatorTestsParser.parse(tokensExpr2);
    EvaluatorTestsEvaluator.Evaluate(root2);

    ASTNode* root3 = EvaluatorTestsParser.parse(tokensExpr3);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root3);
    std::string properResult = std::to_string(a);

    REQUIRE(result == properResult);

    delete root1;
    delete root2;
    delete root3;
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
    ASTNode* root1 = EvaluatorTestsParser.parse(tokensExpr1);
    EvaluatorTestsEvaluator.Evaluate(root1);

    // assign var evaluate
    ASTNode* root2 = EvaluatorTestsParser.parse(tokensExpr2);
    EvaluatorTestsEvaluator.Evaluate(root2);

    ASTNode* root3 = EvaluatorTestsParser.parse(tokensExpr3);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root3);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);

    delete root1;
    delete root2;
    delete root3;
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
    expr3.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(expr1);
    const TokenContainer& tokensExpr2 = EvaluatorTestsLexer.tokenize(expr2);
    const TokenContainer& tokensExpr3 = EvaluatorTestsLexer.tokenize(expr3);

    // declare var a evaluate
    ASTNode* root1 = EvaluatorTestsParser.parse(tokensExpr1);
    EvaluatorTestsEvaluator.Evaluate(root1);

    // declare var b and assign to var a evaluate
    ASTNode* root2 = EvaluatorTestsParser.parse(tokensExpr2);
    EvaluatorTestsEvaluator.Evaluate(root2);

    ASTNode* root3 = EvaluatorTestsParser.parse(tokensExpr3);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root3);
    std::string properResult = std::to_string(b);

    REQUIRE(result == properResult);

    delete root1;
    delete root2;
    delete root3;
}

TEST_CASE("Make variable negative by multiplication on -1 using unary minus", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    double a = 200 * -1;

    std::string expr1 = "var a = 200";
    std::string expr2 = "a = a * -1";
    std::string expr3 = "a";

    expr1.push_back(EOF);
    expr2.push_back(EOF);
    expr3.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(expr1);
    const TokenContainer& tokensExpr2 = EvaluatorTestsLexer.tokenize(expr2);
    const TokenContainer& tokensExpr3 = EvaluatorTestsLexer.tokenize(expr3);

    // declare and assign var evaluate
    ASTNode* root1 = EvaluatorTestsParser.parse(tokensExpr1);
    EvaluatorTestsEvaluator.Evaluate(root1);

    // multiplication on -1
    ASTNode* root2 = EvaluatorTestsParser.parse(tokensExpr2);
    EvaluatorTestsEvaluator.Evaluate(root2);

    ASTNode* root3 = EvaluatorTestsParser.parse(tokensExpr3);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root3);
    std::string properResult = std::to_string(a);

    REQUIRE(result == properResult);

    delete root1;
    delete root2;
    delete root3;
}

TEST_CASE("Make variable negative by assigning on other negative variable using unary minus", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    double a = 200;
    double b = a * -1;

    std::string expr1 = "var a = 200";
    std::string expr2 = "var b = -a";
    std::string expr3 = "b";

    expr1.push_back(EOF);
    expr2.push_back(EOF);
    expr3.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(expr1);
    const TokenContainer& tokensExpr2 = EvaluatorTestsLexer.tokenize(expr2);
    const TokenContainer& tokensExpr3 = EvaluatorTestsLexer.tokenize(expr3);

    // declare and assign var a evaluate
    ASTNode* root1 = EvaluatorTestsParser.parse(tokensExpr1);
    EvaluatorTestsEvaluator.Evaluate(root1);

    // declare and assign var b evaluate
    ASTNode* root2 = EvaluatorTestsParser.parse(tokensExpr2);
    EvaluatorTestsEvaluator.Evaluate(root2);

    ASTNode* root3 = EvaluatorTestsParser.parse(tokensExpr3);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root3);
    std::string properResult = std::to_string(b);

    REQUIRE(result == properResult);

    delete root1;
    delete root2;
    delete root3;
}

TEST_CASE("Get throw on using of undeclared variable", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "a";
    expr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(expr);
    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    REQUIRE_THROWS(EvaluatorTestsEvaluator.Evaluate(root));

    delete root;
}

TEST_CASE("Get throw on using of undeclared variable in math bin operation", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "a + 4";
    expr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(expr);
    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    REQUIRE_THROWS(EvaluatorTestsEvaluator.Evaluate(root));

    delete root;
}

TEST_CASE("Get throw on assigning undeclared variable to variable", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "var a = b";
    expr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(expr);
    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    REQUIRE_THROWS(EvaluatorTestsEvaluator.Evaluate(root));

    delete root;
}

TEST_CASE("Multiplication with using of float point numbers Evaluation", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200.5 * 2.25 * 4.1 * 5.6";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);

    delete root;
}

TEST_CASE("Multiplication with using of negative float point numbers Evaluation",
          "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "-200.5 * 2.25 * -4.1 * -5.6";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root);
    std::string properResult = std::to_string(te_interp(expr.c_str(), nullptr));

    REQUIRE(result == properResult);

    delete root;
}

TEST_CASE("Declare & assign float point number value to variable", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    double a = 200.25;

    std::string expr1 = "var a = 200.25";
    std::string expr2 = "a";

    expr1.push_back(EOF);
    expr2.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(expr1);
    const TokenContainer& tokensExpr2 = EvaluatorTestsLexer.tokenize(expr2);

    // declare and assign var evaluate
    ASTNode* root1 = EvaluatorTestsParser.parse(tokensExpr1);
    EvaluatorTestsEvaluator.Evaluate(root1);

    ASTNode* root2 = EvaluatorTestsParser.parse(tokensExpr2);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root2);
    std::string properResult = std::to_string(a);

    REQUIRE(result == properResult);

    delete root1;
    delete root2;
}

TEST_CASE("Declare & assign negative float point number value to variable", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    double a = -200.25;

    std::string expr1 = "var a = -200.25";
    std::string expr2 = "a";

    expr1.push_back(EOF);
    expr2.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(expr1);
    const TokenContainer& tokensExpr2 = EvaluatorTestsLexer.tokenize(expr2);

    // declare and assign var evaluate
    ASTNode* root1 = EvaluatorTestsParser.parse(tokensExpr1);
    EvaluatorTestsEvaluator.Evaluate(root1);

    ASTNode* root2 = EvaluatorTestsParser.parse(tokensExpr2);

    std::string result = EvaluatorTestsEvaluator.Evaluate(root2);
    std::string properResult = std::to_string(a);

    REQUIRE(result == properResult);

    delete root1;
    delete root2;
}