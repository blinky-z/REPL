#include "catch.hpp"
#include "../Lexer.h"
#include "../Token.h"
#include "../Identifier.h"
#include "../Parser.h"
#include "../ASTNode.h"
#include "../TokenContainer.h"
#include "../SemanticAnalyzer.h"
#include "../SemanticAnalysisResult.h"

class ExpressionHandler {
private:
    static Lexer lexer;

    static Parser parser;

    SemanticAnalyzer semanticAnalyzer = SemanticAnalyzer(0);

    std::vector<ProgramTranslationNode*> nodes;
public:
    SemanticAnalysisResult handleExpression(const std::string src) {
        std::string expr = src;
        expr.push_back('\n');
        expr.push_back(EOF);

        const TokenContainer& tokens = lexer.tokenize(expr);

        ProgramTranslationNode* root = parser.parse(tokens);

        const SemanticAnalysisResult& analysisResult = semanticAnalyzer.checkProgram(root);

        nodes.emplace_back(root);

        return analysisResult;
    }

    ~ExpressionHandler() {
        for (const auto& currentStmt : nodes) {
            delete currentStmt;
        }
    }
};

Lexer ExpressionHandler::lexer = Lexer();
Parser ExpressionHandler::parser = Parser();

TEST_CASE("Addition Expression Evaluation", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "var a = 200 + 2 + 4 + 5";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Subtraction Expression Evaluation", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "200 - 2 - 4 - 5";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Multiplication Expression Evaluation", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "200 * 2 * 4 * 5";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Division Evaluation", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "200 / 2 / 4 / 5";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Math expression with multiple math operations [1]", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "200 + 2 * 4 - 5";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Math expression with multiple math operations [2]", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "200 * 2 / 4 - 5";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Math expression with changing the order of operations [1]", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "(200 + 2) * (4 + 5)";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Math expression with changing the order of operations [2]", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "200 * ((2 - 4) + 5)";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Math expression with using of unary minus", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "-5 + -7";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Math expression with using of variable applying the unary minus", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "-5 * -7";

    std::string expr1 = "var a = 7";
    std::string expr2 = "-5 * -a";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(!result.isError());
}

TEST_CASE("Math expression with using of variables", "[SemanticAnalyzer][Math expressions Eval]") {
    ExpressionHandler expressionHandler;

    std::string expr = "(200 + 200 + 350) * 4 - 5";

    std::string expr1 = "var a = 200";
    std::string expr2 = "var b = a + 350";
    std::string expr3 = "(a + b) * 4 - 5";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr3);
    REQUIRE(!result.isError());
}

TEST_CASE("Declare & assign number constant to variable in single expr", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 200";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr1);
    REQUIRE(!result.isError());
}

TEST_CASE("Declare & assign number constant to variable in separate expr", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a";
    std::string expr2 = "a = 200.25 + 5 * 100";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(!result.isError());
}

TEST_CASE("Assign variable to the other variable", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 200";
    std::string expr2 = "var b = -a";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(!result.isError());
}

TEST_CASE("Get error on using of undeclared variable", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "a";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(result.isError());
    REQUIRE(result.errorCode == SemanticAnalysisResult::UNDECLARED_VAR);
}

TEST_CASE("Get error on using of undeclared variable in math expression", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "a + 4";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(result.isError());
    REQUIRE(result.errorCode == SemanticAnalysisResult::UNDECLARED_VAR);
}

TEST_CASE("Get error on using of uninitialized variable in math expression", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a";
    std::string expr2 = "a + 4";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(result.isError());
    REQUIRE(result.errorCode == SemanticAnalysisResult::UNINITIALIZED_VAR);
}

TEST_CASE("Get error on using of uninitialized variable in bool expression", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a";
    std::string expr2 = "a == 5";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(result.isError());
    REQUIRE(result.errorCode == SemanticAnalysisResult::UNINITIALIZED_VAR);
}

TEST_CASE("Get error on assigning value to undeclared variable", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "a = 5";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(result.isError());
    REQUIRE(result.errorCode == SemanticAnalysisResult::UNDECLARED_VAR);
}

TEST_CASE("Get error on assigning undeclared variable", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "var a = b";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(result.isError());
    REQUIRE(result.errorCode == SemanticAnalysisResult::UNDECLARED_VAR);
}

TEST_CASE("Math expression with using of float point numbers", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "200.5 * 2.25 + 4.1 / 5.6";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Expression with using of float point numbers with using of unary minus", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "-200.5 - 2.25 * -4.1 / -5.6";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Expression with using of unary minus and parentheses", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "-(-((-200) * -5))";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Declare & assign float point number constant to the variable", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "var a = 200.25";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Declare & assign bool constant to the variable", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "var a = true";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Declare & assign bool constant to the variable in separate expr", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a";
    std::string expr2 = "a = false";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(!result.isError());
}

TEST_CASE("Bool expression evaluation: using of Logical OR", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "false || true";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Bool expression evaluation: using of Logical AND", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "false && true";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Bool expression evaluation: using of Logical AND and OR", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "false && true || false";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Bool expression evaluation with using of variables", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 3 == 3";
    std::string expr2 = "a || false";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(!result.isError());
}

TEST_CASE("Declare & assign bool expr to variable", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "var a = (true && false) || 5 == 9";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Get error on assignment incompatible value type : assign number value to bool type variable",
          "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var b = false";
    std::string expr2 = "b = 5";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(result.errorCode == SemanticAnalysisResult::INVALID_VALUE_TYPE);
}

TEST_CASE("Get error on assignment incompatible value type : assign bool value to number type variable",
          "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var b = 5";
    std::string expr2 = "b = true";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(result.errorCode == SemanticAnalysisResult::INVALID_VALUE_TYPE);
}

TEST_CASE("Get error on assignment incompatible variable value type : assign bool variable to number variable",
          "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = true";
    std::string expr2 = "var b = 5";
    std::string expr3 = "b = a";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr3);
    REQUIRE(result.errorCode == SemanticAnalysisResult::INVALID_VALUE_TYPE);
}

TEST_CASE("Get error on assignment incompatible variable value type : assign num variable to bool variable",
          "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 10";
    std::string expr2 = "var b = false";
    std::string expr3 = "b = a";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr3);
    REQUIRE(result.errorCode == SemanticAnalysisResult::INVALID_VALUE_TYPE);
}

TEST_CASE("Compare math expressions", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "(2 + 2) == (2 * 2)";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Compare bool expressions", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "(true && false) == (true == false)";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Get error on using integer operator with incompatible operand value types", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "true + false";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(result.errorCode == SemanticAnalysisResult::INCOMPATIBLE_OPERAND_TYPES);
}

TEST_CASE("Get error on using boolean operator with incompatible operand value types", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "5 || 9";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(result.errorCode == SemanticAnalysisResult::INCOMPATIBLE_OPERAND_TYPES);
}

TEST_CASE("Get error on eval of bool expression with incompatible Identifier value types", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 5";
    std::string expr2 = "var b = false";
    std::string expr3 = "a || b";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr3);
    REQUIRE(result.errorCode == SemanticAnalysisResult::INCOMPATIBLE_OPERAND_TYPES);
}

TEST_CASE("Comparison operator evaluation: LESS THAN", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "2 < 5";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Math Expressions comparison", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "(2 * 5) > (4 * 4)";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Negative values comparison", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "-4 < -2";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Variables comparison", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 5";
    std::string expr2 = "var b = 10";
    std::string expr3 = "a < b";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr3);
    REQUIRE(!result.isError());
}

TEST_CASE("Get error on comparison of incompatible value types [2]", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "false > (5 + 6)";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(result.errorCode == SemanticAnalysisResult::INCOMPATIBLE_OPERAND_TYPES);
}

TEST_CASE("Get error on comparison of incompatible Identifier value types [1]", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 5";
    std::string expr2 = "var b = 3 == 3";
    std::string expr3 = "a < b";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr3);
    REQUIRE(result.errorCode == SemanticAnalysisResult::INCOMPATIBLE_OPERAND_TYPES);
}

TEST_CASE("Declare if statement", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 5";
    std::string expr2 = "if (a > 0) {"
                        "a\n"
                        "}";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(!result.isError());
}

TEST_CASE("Can use local variables but not outer scope variables with the same name, outer scope is global",
          "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 5";
    std::string expr2 = "if (a > 0 && a < 10) {"
                        "var a = 150\n"
                        "a\n"
                        "}";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(!result.isError());
}

TEST_CASE("Can use local variables but not outer scope variables with the same name, outer scope is block scope",
          "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = true";
    std::string expr2 = "if (a) {"
                        "var b = 755\n"
                        "if (b == 755) {"
                        "var b = 404\n"
                        "b == 404\n"
                        "}\n"
                        "b == 755\n"
                        "}";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(!result.isError());
}

TEST_CASE("Assign value to variable in outer scope", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 705";
    std::string expr2 = "if (a == 705) {"
                        "a = 1\n"
                        "}";
    std::string expr3 = "a";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr3);
    REQUIRE(!result.isError());
}

TEST_CASE("Assert variable destroying after it goes out of if statement scope", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "if (true) {"
                        "var a = 505\n"
                        "}";
    std::string expr2 = "a";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(result.isError());
    REQUIRE(result.errorCode == SemanticAnalysisResult::UNDECLARED_VAR);
}

TEST_CASE("Assert variable destroying after it goes out of scope inside another if statement block scope",
          "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "if (true) {"
                       "if (true) {"
                       "var b = 400\n"
                       "}\n"
                       "b\n"
                       "}";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(result.isError());
    REQUIRE(result.errorCode == SemanticAnalysisResult::UNDECLARED_VAR);
}

TEST_CASE("Declare for statement with loop control variable", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "for (var i = 0; i < 5; i = i + 1) {"
                       "i\n"
                       "}";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Declare for statement declaring loop control variable out of for initialization", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var i = 0";
    std::string expr2 = "for (; i < 5; i = i + 1) {"
                        "i\n"
                        "}";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(!result.isError());
}

TEST_CASE("Evaluate for statement declaring loop control variable out of for initialization but assign inside",
          "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var i";
    std::string expr2 = "for (i = 0; i < 5; i = i + 1) {"
                        "i\n"
                        "}";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(!result.isError());
}

TEST_CASE("Assert using of local loop control variable in for statement but not variable in out of scope",
          "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var i = 10";
    std::string expr2 = "for (var i = -5; i < 0; i = i + 1) {"
                        "i\n"
                        "}";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(!result.isError());
}

TEST_CASE("Declare for statement contains another for statement", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "for (var i = 55; i < 56; i = i + 1) {"
                       "i\n"
                       "for (var i = 100; i < 101; i = i + 1) {"
                       "i\n"
                       "}\n"
                       "}";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(!result.isError());
}

TEST_CASE("Get error on using of undeclared loop control variable in for initialization", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "for (i = 0; ; ) {}";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(result.errorCode == SemanticAnalysisResult::UNDECLARED_VAR);
}

TEST_CASE("Get error on using of undeclared loop control variable in for condition", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "for (; i < 5; ) {}";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(result.errorCode == SemanticAnalysisResult::UNDECLARED_VAR);
}

TEST_CASE("Get error on using of undeclared loop control variable in for increase", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "for (; ; i = i + 1) {}";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(result.errorCode == SemanticAnalysisResult::UNDECLARED_VAR);
}

TEST_CASE("Assert loop control variable declared in initialization destroying after it goes out of for statement scope",
          "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "for (var i = 0; i < 5; i = i + 1) {}";
    std::string expr2 = "i";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(result.errorCode == SemanticAnalysisResult::UNDECLARED_VAR);
}

TEST_CASE("Assert variable declared in block statement was destroyed after it goes out of for statement scope",
          "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "for (var i = 0; i < 1; i = i + 1) {"
                        "var a = 10\n"
                        "}";
    std::string expr2 = "a";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(result.errorCode == SemanticAnalysisResult::UNDECLARED_VAR);
}

TEST_CASE("Get error on passing not enough params to function", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func void mul(var int n, var int m) {"
                        "n * m\n"
                        "}";
    std::string expr2 = "var a = 10";
    std::string expr3 = "mul(a)";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr3);
    REQUIRE(result.errorCode == SemanticAnalysisResult::NO_MATCHING_FUNC);
}

TEST_CASE("Get error on passing too many params to function", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func void mul(var int n, var int m) {"
                        "n * m\n"
                        "}";
    std::string expr2 = "var a = 10";
    std::string expr3 = "var b = 50";
    std::string expr4 = "var c = 100";
    std::string expr5 = "mul(a, b, c)";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);
    expressionHandler.handleExpression(expr3);
    expressionHandler.handleExpression(expr4);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr5);
    REQUIRE(result.errorCode == SemanticAnalysisResult::NO_MATCHING_FUNC);
}

TEST_CASE("Get error on passing undeclared variable to function", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func void mul(var int n, var int m) {"
                        "n * m\n"
                        "}";
    std::string expr2 = "var a = 10";
    std::string expr3 = "mul(a, b)";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr3);
    REQUIRE(result.errorCode == SemanticAnalysisResult::UNDECLARED_VAR);
}

TEST_CASE("Get error on passing uninitialized variable to function", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func int getInt(var int n) {"
                        "return n\n"
                        "}";
    std::string expr2 = "var a";
    std::string expr3 = "getInt(a)";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr3);
    REQUIRE(result.errorCode == SemanticAnalysisResult::UNINITIALIZED_VAR);
}

TEST_CASE("Use variable declared in global scope in function", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 10";
    std::string expr2 = "var b = 50";
    std::string expr3 = "func int add() {"
                        "return a + b\n"
                        "}";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr3);
    REQUIRE(!result.isError());
}

TEST_CASE("Pass incompatible param type to function", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func int getInt(var int a) {"
                        "return a\n"
                        "}";
    std::string expr2 = "getInt(true)";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(result.isError());
    REQUIRE(result.errorCode == SemanticAnalysisResult::INVALID_VALUE_TYPE);
}

TEST_CASE("Pass rvalue to function [0]. Pass number expr", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func int getInt(var int a) {"
                        "return a\n"
                        "}";
    std::string expr2 = "getInt(5 * 100)";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(!result.isError());
}

TEST_CASE("Pass rvalue to function [1]. Pass bool expr", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func bool getInt(var bool a) {"
                        "return a\n"
                        "}";
    std::string expr2 = "getInt(true || false)";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(!result.isError());
}

TEST_CASE("Assert that function declaration allowed only in global scope [0]. Declare func in if statement",
          "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "if (true) {"
                       "func void add() {"
                       "}\n"
                       "}";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(result.errorCode == SemanticAnalysisResult::FUNC_DEFINITION_IS_NOT_ALLOWED);
}

TEST_CASE("Assert that function declaration allowed only in global scope [1]. Declare func void in for statement",
          "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "for (var i = 0; i < 1; i = i + 1) {"
                       "func void add() {"
                       "a + b\n"
                       "}\n"
                       "}";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);

    REQUIRE(result.errorCode == SemanticAnalysisResult::FUNC_DEFINITION_IS_NOT_ALLOWED);
}

TEST_CASE("Assert that function declaration allowed only in global scope [1]. Declare func inside another function",
          "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "func void add() {"
                       "func void mul() {"
                       "}\n"
                       "}";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(result.errorCode == SemanticAnalysisResult::FUNC_DEFINITION_IS_NOT_ALLOWED);
}

TEST_CASE("Call function in if statement", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func int getInt(var int a) {"
                        "return a\n"
                        "}";
    std::string expr2 = "if (true) {"
                        "var a = 50\n"
                        "getInt(a)\n"
                        "}";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(!result.isError());
}

TEST_CASE("Call function in for statement", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func int getInt(var int a) {"
                        "return a\n"
                        "}";
    std::string expr2 = "for (var i = 0; i < 10; i = i + 1) {"
                        "getInt(i)\n"
                        "}";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(!result.isError());
}

TEST_CASE("Get error on calling undeclared function", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "add()";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr1);
    REQUIRE(result.errorCode == SemanticAnalysisResult::UNDECLARED_FUNC);
}

TEST_CASE("Assign function call to variable", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func int add(var int a, var int b) {"
                        "return a + b\n"
                        "}";
    std::string expr2 = "var a = add(5, 10)";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(!result.isError());
}

TEST_CASE("Use function call in math expression", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func int mul(var int a, var int b) {"
                        "return a * b\n"
                        "}";
    std::string expr2 = "(mul(3, 5) + mul(2, 10)) / 5";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(!result.isError());
}

TEST_CASE("Get error on assigning void function to variable", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func void do() {}";
    std::string expr2 = "var a = do()";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(result.errorCode == SemanticAnalysisResult::INVALID_VALUE_TYPE);
}

TEST_CASE("Get error on assigning mismatched func return type to variable", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func int add(var int a, var int b) {return a + b}";
    std::string expr2 = "var a = true";
    std::string expr3 = "a = add(5, 10)";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr3);
    REQUIRE(result.isError());
    REQUIRE(result.errorCode == SemanticAnalysisResult::INVALID_VALUE_TYPE);
}

TEST_CASE("Assert that break statement is allowed only in for loops. Use in if statement [1]", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "if (true) {"
                       "break\n"
                       "}";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(result.errorCode == SemanticAnalysisResult::INVALID_OPERATION);
}

TEST_CASE("Assert that break statement is allowed only in for loops. Use in global scope [2]", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "break";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(result.errorCode == SemanticAnalysisResult::INVALID_OPERATION);
}

TEST_CASE("Assert that break statement is allowed only in for loops. Use in function [3]", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "func void do() {"
                       "break\n"
                       "}";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);

    REQUIRE(result.errorCode == SemanticAnalysisResult::INVALID_OPERATION);
}

TEST_CASE("Assert functions can take function calls as parameters", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func int mul(var int a, var int b) {"
                        "return a * b\n"
                        "}";

    std::string expr2 = "func int add(var int a, var int b) {"
                        "return a + b\n"
                        "}";

    std::string expr3 = "mul(add(add(5, 10), add(10, 10)), 100)";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr3);
    REQUIRE(!result.isError());
}

TEST_CASE("Assert functions can return function calls", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func int mul(var int a, var int b) {"
                        "return a * b\n"
                        "}";

    std::string expr2 = "func int add_mul(var int a, var int b) {"
                        "return mul(a, b) + mul(a, b)\n"
                        "}";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(!result.isError());
}

TEST_CASE("Assert print function can take number expr", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 5";
    std::string expr2 = "func int getInt() { return 500 }";
    std::string expr3 = "print(500 * a + getInt())";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr3);
    REQUIRE(!result.isError());
}

TEST_CASE("Assert print function can take bool expr", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = true";
    std::string expr2 = "func bool getBool() { return false }";
    std::string expr3 = "print(a || getBool())";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr3);
    REQUIRE(!result.isError());
}

TEST_CASE("Assert print function can not take void function call", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func void getVoid() {return}";
    std::string expr2 = "print(getVoid())";

    expressionHandler.handleExpression(expr1);

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr2);
    REQUIRE(result.isError());
    REQUIRE(result.errorCode == SemanticAnalysisResult::INVALID_VALUE_TYPE);
}

TEST_CASE("Assert print function can not take as parameter itself", "[SemanticAnalyzer]") {
    ExpressionHandler expressionHandler;

    std::string expr = "print(print(5))";

    const SemanticAnalysisResult& result = expressionHandler.handleExpression(expr);
    REQUIRE(result.isError());
    REQUIRE(result.errorCode == SemanticAnalysisResult::INVALID_VALUE_TYPE);
}