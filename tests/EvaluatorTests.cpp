#include "catch.hpp"
#include "tinyexpr.h"
#include "../Lexer.h"
#include "../AnalysisContainer.h"
#include "../Token.h"
#include "../Identifier.h"
#include "../Parser.h"
#include "../ASTNode.h"
#include "../Evaluator.h"

Lexer EvaluatorTestsLexer;
Parser EvaluatorTestsParser;
Evaluator EvaluatorTestsEvaluator;

TEST_CASE("Addition Evaluation", "[Evaluator][Math operations evaluating]") {
    std::string expr = "200 + 2 + 4 + 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    AnalysisContainer data = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(data);

    double result = EvaluatorTestsEvaluator.Evaluate(root);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Subtraction Evaluation", "[Evaluator][Math operations evaluating]") {
    std::string expr = "200 - 2 - 4 - 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    AnalysisContainer data = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(data);

    double result = EvaluatorTestsEvaluator.Evaluate(root);

    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Multiplication Evaluation", "[Evaluator][Math operations evaluating]") {
    std::string expr = "200 * 2 * 4 * 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    AnalysisContainer data = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(data);

    double result = EvaluatorTestsEvaluator.Evaluate(root);

    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Division Evaluation", "[Evaluator][Math operations evaluating]") {
    std::string expr = "200 / 2 / 4 / 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    AnalysisContainer data = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(data);

    double result = EvaluatorTestsEvaluator.Evaluate(root);

    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Multiplication with Sum Parentheses Evaluation", "[Evaluator][Math operations evaluating]") {
    std::string expr = "(200 + 2) * (4 + 5)";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    AnalysisContainer data = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(data);

    double result = EvaluatorTestsEvaluator.Evaluate(root);

    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Multiplication with Difference Parentheses Evaluation", "[Evaluator][Math operations evaluating]") {
    std::string expr = "(200 - 2) * (4 - 5)";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    AnalysisContainer data = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(data);

    double result = EvaluatorTestsEvaluator.Evaluate(root);

    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Division with Sum Parentheses Evaluation", "[Evaluator][Math operations evaluating]") {
    std::string expr = "(200 + 2) / (4 + 5)";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    AnalysisContainer data = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(data);

    double result = EvaluatorTestsEvaluator.Evaluate(root);

    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Division with Difference Parentheses Evaluation", "[Evaluator][Math operations evaluating]") {
    std::string expr = "(200 - 2) / (4 - 5)";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    AnalysisContainer data = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(data);

    double result = EvaluatorTestsEvaluator.Evaluate(root);

    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Multiple operations: Sum, Mul, Sub", "[Evaluator][Math operations evaluating]") {
    std::string expr = "200 + 2 * 4 - 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    AnalysisContainer data = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(data);

    double result = EvaluatorTestsEvaluator.Evaluate(root);

    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Multiple operations: Sum, Div, Sub", "[Evaluator][Math operations evaluating]") {
    std::string expr = "200 + 2 / 4 - 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    AnalysisContainer data = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(data);

    double result = EvaluatorTestsEvaluator.Evaluate(root);

    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Multiple operations: Mul, Div, Sub", "[Evaluator][Math operations evaluating]") {
    std::string expr = "200 * 2 / 4 - 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    AnalysisContainer data = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(data);

    double result = EvaluatorTestsEvaluator.Evaluate(root);

    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Multiple operations: Mul, Div, Sum", "[Evaluator][Math operations evaluating]") {
    std::string expr = "200 * 2 / 4 + 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    AnalysisContainer data = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(data);

    double result = EvaluatorTestsEvaluator.Evaluate(root);

    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Multiple operations with single parentheses: Mul, Sub, Add", "[Evaluator][Math operations evaluating]") {
    std::string expr = "200 * (2 - 4) + 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    AnalysisContainer data = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(data);

    double result = EvaluatorTestsEvaluator.Evaluate(root);

    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Multiple operations with single parentheses: Div, Add, Mul", "[Evaluator][Math operations evaluating]") {
    std::string expr = "200 / (2 + 4) * 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    AnalysisContainer data = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(data);

    double result = EvaluatorTestsEvaluator.Evaluate(root);

    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Multiple operations with multiple parentheses", "[Evaluator][Math operations evaluating]") {
    std::string expr = "((200 + 2) / (2 - 4)) * 5 + 5";

    std::string parseExpr = expr;
    parseExpr.push_back(EOF);

    AnalysisContainer data = EvaluatorTestsLexer.tokenize(parseExpr);

    ASTNode* root = EvaluatorTestsParser.parse(data);

    double result = EvaluatorTestsEvaluator.Evaluate(root);

    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}