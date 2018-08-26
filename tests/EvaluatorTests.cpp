#include "catch.hpp"
#include "tinyexpr.h"
#include "../Lexer.h"
#include "../AnalysisContainer.h"
#include "../Token.h"
#include "../Identifier.h"
#include "../Parser.h"
#include "../ASTNode.h"
#include "../Evaluator.h"

TEST_CASE("Addition Evaluation", "[Evaluator][Math operations evaluating]") {
    string expr = "200 + 2 + 4 + 5";
    expr.push_back(EOF);

    Lexer lexer;
    AnalysisContainer data = lexer.tokenize(expr);

    Parser parser(data);
    ASTNode* root = parser.parse();

    Evaluator evaluator;
    double result = evaluator.Evaluate(root);
    expr.pop_back();
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Subtraction Evaluation", "[Evaluator][Math operations evaluating]") {
    string expr = "200 - 2 - 4 - 5";
    expr.push_back(EOF);

    Lexer lexer;
    AnalysisContainer data = lexer.tokenize(expr);

    Parser parser(data);
    ASTNode* root = parser.parse();

    Evaluator evaluator;
    double result = evaluator.Evaluate(root);
    expr.pop_back();
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Multiplication Evaluation", "[Evaluator][Math operations evaluating]") {
    string expr = "200 * 2 * 4 * 5";
    expr.push_back(EOF);

    Lexer lexer;
    AnalysisContainer data = lexer.tokenize(expr);

    Parser parser(data);
    ASTNode* root = parser.parse();

    Evaluator evaluator;
    double result = evaluator.Evaluate(root);
    expr.pop_back();
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Division Evaluation", "[Evaluator][Math operations evaluating]") {
    string expr = "200 / 2 / 4 / 5";
    expr.push_back(EOF);

    Lexer lexer;
    AnalysisContainer data = lexer.tokenize(expr);

    Parser parser(data);
    ASTNode* root = parser.parse();

    Evaluator evaluator;
    double result = evaluator.Evaluate(root);
    expr.pop_back();
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Multiplication with Sum Parentheses Evaluation", "[Evaluator][Math operations evaluating]") {
    string expr = "(200 + 2) * (4 + 5)";
    expr.push_back(EOF);

    Lexer lexer;
    AnalysisContainer data = lexer.tokenize(expr);

    Parser parser(data);
    ASTNode* root = parser.parse();

    Evaluator evaluator;
    double result = evaluator.Evaluate(root);
    expr.pop_back();
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Multiplication with Difference Parentheses Evaluation", "[Evaluator][Math operations evaluating]") {
    string expr = "(200 - 2) * (4 - 5)";
    expr.push_back(EOF);

    Lexer lexer;
    AnalysisContainer data = lexer.tokenize(expr);

    Parser parser(data);
    ASTNode* root = parser.parse();

    Evaluator evaluator;
    double result = evaluator.Evaluate(root);
    expr.pop_back();
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Division with Sum Parentheses Evaluation", "[Evaluator][Math operations evaluating]") {
    string expr = "(200 + 2) / (4 + 5)";
    expr.push_back(EOF);

    Lexer lexer;
    AnalysisContainer data = lexer.tokenize(expr);

    Parser parser(data);
    ASTNode* root = parser.parse();

    Evaluator evaluator;
    double result = evaluator.Evaluate(root);
    expr.pop_back();
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}

TEST_CASE("Division with Difference Parentheses Evaluation", "[Evaluator][Math operations evaluating]") {
    string expr = "(200 - 2) / (4 - 5)";
    expr.push_back(EOF);

    Lexer lexer;
    AnalysisContainer data = lexer.tokenize(expr);

    Parser parser(data);
    ASTNode* root = parser.parse();

    Evaluator evaluator;
    double result = evaluator.Evaluate(root);
    expr.pop_back();
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result == properResult);
}