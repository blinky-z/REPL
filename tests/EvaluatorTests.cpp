#include "catch.hpp"
#include "tinyexpr.h"
#include "../Lexer.h"
#include "../Token.h"
#include "../Identifier.h"
#include "../Parser.h"
#include "../ASTNode.h"
#include "../Evaluator.h"
#include "../EvalResult.h"

Lexer EvaluatorTestsLexer;
Parser EvaluatorTestsParser;

std::string getBoolValueStringRepresentation(bool value) {
    return value ? "true" : "false";
}

TEST_CASE("Addition Evaluation", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 + 2 + 4 + 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);

    delete root;
}

TEST_CASE("Subtraction Evaluation", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 - 2 - 4 - 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);

    delete root;
}

TEST_CASE("Multiplication Evaluation", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 * 2 * 4 * 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);

    delete root;
}

TEST_CASE("Division Evaluation", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 / 2 / 4 / 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);

    delete root;
}

TEST_CASE("Multiplication with Sum Parentheses Evaluation", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "(200 + 2) * (4 + 5)";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);

    delete root;
}

TEST_CASE("Multiplication with Difference Parentheses Evaluation", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "(200 - 2) * (4 - 5)";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);

    delete root;
}

TEST_CASE("Division with Sum Parentheses Evaluation", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "(200 + 2) / (4 + 5)";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);

    delete root;
}

TEST_CASE("Division with Difference Parentheses Evaluation", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "(200 - 2) / (4 - 5)";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);

    delete root;
}

TEST_CASE("Multiple operations: Sum, Mul, Sub", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 + 2 * 4 - 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);

    delete root;
}

TEST_CASE("Multiple operations: Sum, Div, Sub", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 + 2 / 4 - 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);

    delete root;
}

TEST_CASE("Multiple operations: Mul, Div, Sub", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 * 2 / 4 - 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);

    delete root;
}

TEST_CASE("Multiple operations: Mul, Div, Sum", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 * 2 / 4 + 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);

    delete root;
}

TEST_CASE("Multiple operations with single parentheses: Mul, Sub, Add", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 * (2 - 4) + 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);

    delete root;
}

TEST_CASE("Multiple operations with single parentheses: Div, Add, Mul", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200 / (2 + 4) * 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);

    delete root;
}

TEST_CASE("Multiple operations with multiple parentheses", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "((200 + 2) / (2 - 4)) * 5 + 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);

    delete root;
}

TEST_CASE("Expression with using of variables", "[Evaluator]") {
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

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root2);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);

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

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root2);
    double properResult = (a);

    REQUIRE(result.getResultDouble() == properResult);

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

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root3);
    double properResult = (a);

    REQUIRE(result.getResultDouble() == properResult);

    delete root1;
    delete root2;
    delete root3;
}

TEST_CASE("Declare & assign var to math expr", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    double a = 200.25 + 5 * 100;

    std::string expr1 = "var a = 200.25 + 5 * 100";
    std::string expr2 = "a";

    expr1.push_back(EOF);
    expr2.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(expr1);
    const TokenContainer& tokensExpr2 = EvaluatorTestsLexer.tokenize(expr2);

    // declare and assign var evaluate
    ASTNode* root1 = EvaluatorTestsParser.parse(tokensExpr1);
    EvaluatorTestsEvaluator.Evaluate(root1);

    ASTNode* root2 = EvaluatorTestsParser.parse(tokensExpr2);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root2);
    double properResult = (a);

    REQUIRE(result.getResultDouble() == properResult);

    delete root1;
    delete root2;
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

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root3);
    double properResult = (b);

    REQUIRE(result.getResultDouble() == properResult);

    delete root1;
    delete root2;
    delete root3;
}

TEST_CASE("Assign variable to the other variable with unary minus", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    double a = 200;
    double b = -a;

    std::string expr1 = "var a = 200";
    std::string expr2 = "var b = -a";
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

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root3);
    double properResult = (b);

    REQUIRE(result.getResultDouble() == properResult);

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

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root3);
    double properResult = (a);

    REQUIRE(result.getResultDouble() == properResult);

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

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);

    REQUIRE(result.error.errorCode == EvalError::UNDECLARED_VAR);

    delete root;
}

TEST_CASE("Get throw on using of undeclared variable in math bin operation", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "a + 4";
    expr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(expr);
    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);

    REQUIRE(result.error.errorCode == EvalError::UNDECLARED_VAR);

    delete root;
}

TEST_CASE("Get throw on assigning undeclared variable in declaration", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "var a = b";
    expr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(expr);
    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);

    REQUIRE(result.error.errorCode == EvalError::UNDECLARED_VAR);

    delete root;
}

TEST_CASE("Expression with using of float point numbers Evaluation", "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "200.5 * 2.25 * 4.1 * 5.6";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);

    delete root;
}

TEST_CASE("Expression with using of negative float point numbers Evaluation",
          "[Evaluator][Math operations evaluating]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "-200.5 * 2.25 * -4.1 * -5.6";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);

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

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root2);
    double properResult = (a);

    REQUIRE(result.getResultDouble() == properResult);

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

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root2);
    double properResult = (a);

    REQUIRE(result.getResultDouble() == properResult);

    delete root1;
    delete root2;
}

TEST_CASE("Single bool value evaluation: true", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "true";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    bool properResult = true;

    REQUIRE(result.getResultBool() == properResult);

    delete root;
}

TEST_CASE("Single bool value evaluation: false", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "false";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    bool properResult = false;

    REQUIRE(result.getResultBool() == properResult);

    delete root;
}

TEST_CASE("Bool expression evaluation: using of Logical OR with different bool values", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "false || true";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    bool properResult = false || true;

    REQUIRE(result.getResultBool() == properResult);

    delete root;
}

TEST_CASE("Bool expression evaluation: using of Logical OR with the same false bool values",
          "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "false || false";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    bool properResult = false || false;

    REQUIRE(result.getResultBool() == properResult);

    delete root;
}

TEST_CASE("Bool expression evaluation: using of Logical OR with the same true bool values", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "true || true";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    bool properResult = true || true;

    REQUIRE(result.getResultBool() == properResult);

    delete root;
}

TEST_CASE("Bool expression evaluation: using of Logical AND with different bool values", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "false && true";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    bool properResult = false && true;

    REQUIRE(result.getResultBool() == properResult);

    delete root;
}

TEST_CASE("Bool expression evaluation: using of Logical AND with the same false bool values", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "false && false";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    bool properResult = false && false;

    REQUIRE(result.getResultBool() == properResult);

    delete root;
}

TEST_CASE("Bool expression evaluation: using of Logical AND with the same true bool values", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "true && true";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    bool properResult = true && true;

    REQUIRE(result.getResultBool() == properResult);

    delete root;
}

TEST_CASE("Bool expression evaluation: using of Logical AND and OR [1]", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "false && true || false";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    bool properResult = false && true || false;

    REQUIRE(result.getResultBool() == properResult);

    delete root;
}

TEST_CASE("Bool expression evaluation: using of Logical AND and OR [2]", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "false && true || true";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    bool properResult = false && true || true;

    REQUIRE(result.getResultBool() == properResult);

    delete root;
}

TEST_CASE("Bool expression evaluation: using of Logical AND and OR changing the order of operations [1]",
          "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "false && (true || false)";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    bool properResult = false && (true || false);

    REQUIRE(result.getResultBool() == properResult);

    delete root;
}

TEST_CASE("Bool expression evaluation: using of Logical AND and OR changing the order of operations [2]",
          "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "(true || false) && false";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    const TokenContainer& tokens = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(tokens);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);
    bool properResult = (true || false) && false;

    REQUIRE(result.getResultBool() == properResult);

    delete root;
}

TEST_CASE("Declare & assign bool value to the variable in single expr", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    bool a = true;

    std::string expr1 = "var a = true";
    std::string expr2 = "a";

    expr1.push_back(EOF);
    expr2.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(expr1);
    const TokenContainer& tokensExpr2 = EvaluatorTestsLexer.tokenize(expr2);

    // declare and assign var evaluate
    ASTNode* root1 = EvaluatorTestsParser.parse(tokensExpr1);
    EvaluatorTestsEvaluator.Evaluate(root1);

    ASTNode* root2 = EvaluatorTestsParser.parse(tokensExpr2);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root2);
    bool properResult = a;

    REQUIRE(result.getResultBool() == properResult);

    delete root1;
    delete root2;
}

TEST_CASE("Declare & assign bool value to the variable in separate expr", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    bool a = false;

    std::string expr1 = "var a";
    std::string expr2 = "a = false";
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

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root3);
    bool properResult = a;

    REQUIRE(result.getResultBool() == properResult);

    delete root1;
    delete root2;
    delete root3;
}

TEST_CASE("Assign variable of type bool to the other variable of type bool", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    bool a = true;
    bool b = a;

    std::string expr1 = "var a = true";
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

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root3);
    bool properResult = b;

    REQUIRE(result.getResultBool() == properResult);

    delete root1;
    delete root2;
    delete root3;
}

TEST_CASE("Get exception on assign variable of type bool to the value of INCOMPATIBLE type: int to bool",
          "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr1 = "var b = 5";
    std::string expr2 = "b = false";

    expr1.push_back(EOF);
    expr2.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(expr1);
    const TokenContainer& tokensExpr2 = EvaluatorTestsLexer.tokenize(expr2);

    ASTNode* root1 = EvaluatorTestsParser.parse(tokensExpr1);
    EvaluatorTestsEvaluator.Evaluate(root1);

    ASTNode* root2 = EvaluatorTestsParser.parse(tokensExpr2);
    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root2);

    REQUIRE(result.error.errorCode == EvalError::INVALID_VALUE_TYPE);

    delete root1;
    delete root2;
}

TEST_CASE("Get exception on assign variable of type bool to the value of INCOMPATIBLE type: bool to int",
          "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr1 = "var b = true";
    std::string expr2 = "b = 5";

    expr1.push_back(EOF);
    expr2.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(expr1);
    const TokenContainer& tokensExpr2 = EvaluatorTestsLexer.tokenize(expr2);

    ASTNode* root1 = EvaluatorTestsParser.parse(tokensExpr1);
    EvaluatorTestsEvaluator.Evaluate(root1);

    ASTNode* root2 = EvaluatorTestsParser.parse(tokensExpr2);
    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root2);

    REQUIRE(result.error.errorCode == EvalError::INVALID_VALUE_TYPE);

    delete root1;
    delete root2;
}

TEST_CASE("Get exception on assign variable of type bool to the other variable of INCOMPATIBLE type: int to bool",
          "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr1 = "var a = true";
    std::string expr2 = "var b = 5";
    std::string expr3 = "b = a";

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
    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root3);

    REQUIRE(result.error.errorCode == EvalError::INVALID_VALUE_TYPE);

    delete root1;
    delete root2;
    delete root3;
}

TEST_CASE("Get exception on assign variable of type bool to the other variable of INCOMPATIBLE type: bool to int",
          "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr1 = "var a = 5";
    std::string expr2 = "var b = false";
    std::string expr3 = "b = a";

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
    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root3);

    REQUIRE(result.error.errorCode == EvalError::INVALID_VALUE_TYPE);

    delete root1;
    delete root2;
    delete root3;
}

TEST_CASE("Bool expression evaluation with variables: Logical AND", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    bool a = true;

    std::string expr1 = "var a = true";
    std::string expr2 = "a && false";

    expr1.push_back(EOF);
    expr2.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(expr1);
    const TokenContainer& tokensExpr2 = EvaluatorTestsLexer.tokenize(expr2);

    ASTNode* root1 = EvaluatorTestsParser.parse(tokensExpr1);
    EvaluatorTestsEvaluator.Evaluate(root1);

    ASTNode* root2 = EvaluatorTestsParser.parse(tokensExpr2);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root2);

    bool properResult = a && false;

    REQUIRE(result.getResultBool() == properResult);

    delete root1;
    delete root2;
}

TEST_CASE("Bool expression evaluation with variables: Logical OR", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    bool a = true;

    std::string expr1 = "var a = true";
    std::string expr2 = "a || false";

    expr1.push_back(EOF);
    expr2.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(expr1);
    const TokenContainer& tokensExpr2 = EvaluatorTestsLexer.tokenize(expr2);

    ASTNode* root1 = EvaluatorTestsParser.parse(tokensExpr1);
    EvaluatorTestsEvaluator.Evaluate(root1);

    ASTNode* root2 = EvaluatorTestsParser.parse(tokensExpr2);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root2);

    bool properResult = a || false;

    REQUIRE(result.getResultBool() == properResult);

    delete root1;
    delete root2;
}

TEST_CASE("Declare & assign bool expr to variable", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    bool a = (true || false) && false;

    std::string expr1 = "var a = (true || false) && false";
    std::string expr2 = "a";

    expr1.push_back(EOF);
    expr2.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(expr1);
    const TokenContainer& tokensExpr2 = EvaluatorTestsLexer.tokenize(expr2);

    ASTNode* root1 = EvaluatorTestsParser.parse(tokensExpr1);
    EvaluatorTestsEvaluator.Evaluate(root1);

    ASTNode* root2 = EvaluatorTestsParser.parse(tokensExpr2);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root2);
    bool properResult = a;

    REQUIRE(result.getResultBool() == properResult);

    delete root1;
    delete root2;
}

TEST_CASE("Bool expression with using of equality operator [1]", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr1 = "false || true == false || true";

    expr1.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(expr1);

    ASTNode* root1 = EvaluatorTestsParser.parse(tokensExpr1);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root1);
    bool properResult = false || true == false || true;

    REQUIRE(result.getResultBool() == properResult);

    delete root1;
}

TEST_CASE("Bool expression with using of equality operator [2]", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr1 = "false || true && 1 == 1";

    expr1.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(expr1);

    ASTNode* root1 = EvaluatorTestsParser.parse(tokensExpr1);

    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root1);
    bool properResult = false || true && 1 == 1;

    REQUIRE(result.getResultBool() == properResult);

    delete root1;
}

TEST_CASE("Compare two equal double values", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr1 = "var a = 4 / 3";
    std::string expr2 = "4 / 3 == a";

    expr1.push_back(EOF);
    expr2.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(expr1);
    const TokenContainer& tokensExpr2 = EvaluatorTestsLexer.tokenize(expr2);

    ASTNode* root1 = EvaluatorTestsParser.parse(tokensExpr1);
    EvaluatorTestsEvaluator.Evaluate(root1);

    ASTNode* root2 = EvaluatorTestsParser.parse(tokensExpr2);
    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root2);

    REQUIRE(result.getResultBool() == true);

    delete root1;
    delete root2;
}

TEST_CASE("Compare two non-equal double values", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr1 = "var a = 100";
    std::string expr2 = "4 / 3 == a";

    expr1.push_back(EOF);
    expr2.push_back(EOF);

    const TokenContainer& tokensExpr1 = EvaluatorTestsLexer.tokenize(expr1);
    const TokenContainer& tokensExpr2 = EvaluatorTestsLexer.tokenize(expr2);

    ASTNode* root1 = EvaluatorTestsParser.parse(tokensExpr1);
    EvaluatorTestsEvaluator.Evaluate(root1);

    ASTNode* root2 = EvaluatorTestsParser.parse(tokensExpr2);
    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root2);

    REQUIRE(result.getResultBool() == false);

    delete root1;
    delete root2;
}

TEST_CASE("Get throw on assigning expression to not a lvalue", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "4 / 3 = 1";
    expr.push_back(EOF);

    const TokenContainer& tokensExpr = EvaluatorTestsLexer.tokenize(expr);
    ASTNode* root = EvaluatorTestsParser.parse(tokensExpr);
    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);

    REQUIRE(result.error.errorCode == EvalError::INVALID_LVALUE);

    delete root;
}

TEST_CASE("Get throw on assigning expression to undeclared variable", "[Evaluator]") {
    Evaluator EvaluatorTestsEvaluator;

    std::string expr = "a = 1";
    expr.push_back(EOF);

    const TokenContainer& tokensExpr = EvaluatorTestsLexer.tokenize(expr);
    ASTNode* root = EvaluatorTestsParser.parse(tokensExpr);
    const EvalResult& result = EvaluatorTestsEvaluator.Evaluate(root);

    REQUIRE(result.error.errorCode == EvalError::INVALID_LVALUE);

    delete root;
}