#include "catch.hpp"
#include "tinyexpr.h"
#include "../Lexer.h"
#include "../Token.h"
#include "../Identifier.h"
#include "../Parser.h"
#include "../ASTNode.h"
#include "../Evaluator.h"
#include "../EvalResult.h"
#include "../TokenContainer.h"

class ExpressionHandler {
private:
    static Lexer lexer;

    static Parser parser;

    Evaluator evaluator;
public:
    EvalResult handleExpression(const std::string src) {
        std::string expr = src;
        expr.push_back('\n');
        expr.push_back(EOF);

        const TokenContainer& tokens = lexer.tokenize(expr);

        ASTNode* root = parser.parse(tokens);

        const EvalResult& result = evaluator.Evaluate(root);

        delete root;

        return result;
    }
};

Lexer ExpressionHandler::lexer = Lexer();
Parser ExpressionHandler::parser = Parser();

TEST_CASE("Addition Expression Evaluation", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "200 + 2 + 4 + 5";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);
}

TEST_CASE("Subtraction Expression Evaluation", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "200 - 2 - 4 - 5";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);
}

TEST_CASE("Multiplication Expression Evaluation", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "200 * 2 * 4 * 5";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);
}

TEST_CASE("Division Evaluation", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "200 / 2 / 4 / 5";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);
}

TEST_CASE("Math expression with multiple math operations [1]", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "200 + 2 * 4 - 5";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);
}

TEST_CASE("Math expression with multiple math operations [2]", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "200 * 2 / 4 - 5";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);
}

TEST_CASE("Math expression with changing the order of operations [1]", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "(200 + 2) * (4 + 5)";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);
}

TEST_CASE("Math expression with changing the order of operations [2]", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "200 * ((2 - 4) + 5)";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);
}

TEST_CASE("Math expression with using of unary minus", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "-5 + -7";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);
}

TEST_CASE("Math expression with using of variable applying the unary minus", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "-5 * -7";

    std::string expr1 = "var a = 7";
    std::string expr2 = "-5 * -a";

    expressionHandler.handleExpression(expr1);

    const EvalResult& result = expressionHandler.handleExpression(expr2);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);
}

TEST_CASE("Math expression with using of variables", "[Evaluator][Math expressions Eval]") {
    ExpressionHandler expressionHandler;

    std::string expr = "(200 + 200 + 350) * 4 - 5";

    std::string expr1 = "var a = 200";
    std::string expr2 = "var b = a + 350";
    std::string expr3 = "(a + b) * 4 - 5";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const EvalResult& result = expressionHandler.handleExpression(expr3);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);
}

TEST_CASE("Declare & assign number constant to variable in single expr", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 200";
    std::string expr2 = "a";

    expressionHandler.handleExpression(expr1);

    const EvalResult& result = expressionHandler.handleExpression(expr2);
    double properResult = 200;

    REQUIRE(result.getResultDouble() == properResult);
}

TEST_CASE("Declare & assign number constant to variable in separate expr", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a";
    std::string expr2 = "a = 200.25 + 5 * 100";
    std::string expr3 = "a";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const EvalResult& result = expressionHandler.handleExpression(expr3);
    double properResult = 200.25 + 5 * 100;

    REQUIRE(result.getResultDouble() == properResult);
}

TEST_CASE("Assign variable to the other variable", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 200";
    std::string expr2 = "var b = -a";
    std::string expr3 = "b";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const EvalResult& result = expressionHandler.handleExpression(expr3);
    double properResult = -200;

    REQUIRE(result.getResultDouble() == properResult);
}

TEST_CASE("Get error on using of undeclared variable", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "a";

    const EvalResult& result = expressionHandler.handleExpression(expr);

    REQUIRE(result.error.errorCode == EvalError::UNDECLARED_VAR);
}

TEST_CASE("Get error on using of undeclared variable in math expression", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "a + 4";

    const EvalResult& result = expressionHandler.handleExpression(expr);

    REQUIRE(result.error.errorCode == EvalError::UNDECLARED_VAR);
}

TEST_CASE("Get error on using of uninitialized variable in math expression", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a";
    std::string expr2 = "a + 4";

    expressionHandler.handleExpression(expr1);

    const EvalResult& result = expressionHandler.handleExpression(expr2);

    REQUIRE(result.error.errorCode == EvalError::UNINITIALIZED_VAR);
}

TEST_CASE("Get error on using of uninitialized variable in bool expression", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a";
    std::string expr2 = "a == 5";

    expressionHandler.handleExpression(expr1);

    const EvalResult& result = expressionHandler.handleExpression(expr2);

    REQUIRE(result.error.errorCode == EvalError::UNINITIALIZED_VAR);
}

TEST_CASE("Get error on assigning value to undeclared variable", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "a = 5";

    const EvalResult& result = expressionHandler.handleExpression(expr);

    REQUIRE(result.error.errorCode == EvalError::UNDECLARED_VAR);
}

TEST_CASE("Get error on assigning value to not a lvalue", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "4 / 3 = 1";

    const EvalResult& result = expressionHandler.handleExpression(expr);

    REQUIRE(result.error.errorCode == EvalError::INVALID_LVALUE);
}

TEST_CASE("Get error on assigning undeclared variable to the other variable", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "var a = b";

    const EvalResult& result = expressionHandler.handleExpression(expr);

    REQUIRE(result.error.errorCode == EvalError::UNDECLARED_VAR);
}

TEST_CASE("Math expression with using of float point numbers", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "200.5 * 2.25 + 4.1 / 5.6";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);
}

TEST_CASE("Expression with using of float point numbers applying the unary minus", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "-200.5 - 2.25 * -4.1 / -5.6";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);
}

TEST_CASE("Expression with using of unary minus and parentheses", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "-(-((-200) * -5))";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    double properResult = te_interp(expr.c_str(), nullptr);

    REQUIRE(result.getResultDouble() == properResult);
}

TEST_CASE("Declare & assign float point number constant to variable", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 200.25";
    std::string expr2 = "a";

    expressionHandler.handleExpression(expr1);

    const EvalResult& result = expressionHandler.handleExpression(expr2);
    double properResult = 200.25;

    REQUIRE(result.getResultDouble() == properResult);
}

TEST_CASE("Declare & assign float point number constant applying the unary minus to variable", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = -(200.25)";
    std::string expr2 = "a";

    expressionHandler.handleExpression(expr1);

    const EvalResult& result = expressionHandler.handleExpression(expr2);
    double properResult = -200.25;

    REQUIRE(result.getResultDouble() == properResult);
}

TEST_CASE("Declare & assign bool constant to the variable in single expr", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = true";
    std::string expr2 = "a";

    expressionHandler.handleExpression(expr1);

    const EvalResult& result = expressionHandler.handleExpression(expr2);
    bool properResult = true;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Declare & assign bool constant to the variable in separate expr", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a";
    std::string expr2 = "a = false";
    std::string expr3 = "a";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const EvalResult& result = expressionHandler.handleExpression(expr3);
    bool properResult = false;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Assign variable of type bool to the other variable of type bool", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 5 == 7";
    std::string expr2 = "var b = a";
    std::string expr3 = "b";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const EvalResult& result = expressionHandler.handleExpression(expr3);
    bool properResult = 5 == 7;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Bool constant evaluation: true", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "true";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    bool properResult = true;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Bool constant evaluation: false", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "false";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    bool properResult = false;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Bool expression evaluation: using of Logical OR [1]", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "false || true";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    bool properResult = false || true;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Bool expression evaluation: using of Logical OR [2]", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "false || false";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    bool properResult = false || false;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Bool expression evaluation: using of Logical OR [3]", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "true || true";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    bool properResult = true || true;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Bool expression evaluation: using of Logical AND [1]", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "false && true";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    bool properResult = false && true;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Bool expression evaluation: using of Logical AND [2]", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "false && false";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    bool properResult = false && false;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Bool expression evaluation: using of Logical AND [3]", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "true && true";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    bool properResult = true && true;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Bool expression evaluation: using of Logical AND and OR", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "false && true || false";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    bool properResult = false && true || false;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Bool expression evaluation with changing the order of operations", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "false && (true || false)";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    bool properResult = false && (true || false);

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Bool expression evaluation with using of variables", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 3 == 3";
    std::string expr2 = "a || false";

    expressionHandler.handleExpression(expr1);
    const EvalResult& result = expressionHandler.handleExpression(expr2);

    bool properResult = 3 == 3 || false;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Declare & assign bool expr to variable", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = (true && false) || 5 == 9";
    std::string expr2 = "a";

    expressionHandler.handleExpression(expr1);

    const EvalResult& result = expressionHandler.handleExpression(expr2);
    bool properResult = (true && false) || 5 == 9;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Get error on assignment incompatible value type : assign number value to bool type variable",
          "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var b = false";
    std::string expr2 = "b = 5";

    expressionHandler.handleExpression(expr1);

    const EvalResult& result = expressionHandler.handleExpression(expr2);

    REQUIRE(result.error.errorCode == EvalError::INVALID_VALUE_TYPE);
}

TEST_CASE("Get error on assignment incompatible value type : assign bool value to number type variable",
          "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var b = 5";
    std::string expr2 = "b = true";

    expressionHandler.handleExpression(expr1);

    const EvalResult& result = expressionHandler.handleExpression(expr2);

    REQUIRE(result.error.errorCode == EvalError::INVALID_VALUE_TYPE);
}

TEST_CASE("Get error on assignment incompatible variable value type : assign bool variable to number variable",
          "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = true";
    std::string expr2 = "var b = 5";
    std::string expr3 = "b = a";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const EvalResult& result = expressionHandler.handleExpression(expr3);

    REQUIRE(result.error.errorCode == EvalError::INVALID_VALUE_TYPE);
}

TEST_CASE("Get error on assignment incompatible variable value type : assign num variable to bool variable",
          "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 10";
    std::string expr2 = "var b = false";
    std::string expr3 = "b = a";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const EvalResult& result = expressionHandler.handleExpression(expr3);

    REQUIRE(result.error.errorCode == EvalError::INVALID_VALUE_TYPE);
}

TEST_CASE("Bool expression with using of equality operator", "[Evaluator]") {
    // equality operator has precedence higher than OR and AND
    ExpressionHandler expressionHandler;

    std::string expr = "true && 5 == 8 || false";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    bool properResult = true && 5 == 8 || false;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Compare two identical values (by abs()) but applying unary minus to one of operands", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "-5.25 == 5.25";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    bool properResult = -5.25 == 5.25;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Compare two equal double values", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 4 / 3";
    std::string expr2 = "4 / 3 == a";

    expressionHandler.handleExpression(expr1);

    const EvalResult& result = expressionHandler.handleExpression(expr2);

    REQUIRE(result.getResultBool() == true);
}

TEST_CASE("Compare two non-equal double values", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 100";
    std::string expr2 = "4 / 3 == a";

    expressionHandler.handleExpression(expr1);

    const EvalResult& result = expressionHandler.handleExpression(expr2);

    REQUIRE(result.getResultBool() == false);
}

TEST_CASE("Compare math expressions", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "(2 + 2) == (2 * 2)";

    expressionHandler.handleExpression(expr);

    const EvalResult& result = expressionHandler.handleExpression(expr);

    REQUIRE(result.getResultBool() == true);
}

TEST_CASE("Compare bool expressions", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "(true && false) == (true == false)";

    expressionHandler.handleExpression(expr);

    const EvalResult& result = expressionHandler.handleExpression(expr);

    REQUIRE(result.getResultBool() == true);
}

TEST_CASE("Get error on math expression with incompatible operand value types", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "true + false";

    const EvalResult& result = expressionHandler.handleExpression(expr);

    REQUIRE(result.error.errorCode == EvalError::INCOMPATIBLE_OPERAND_TYPES);
}

TEST_CASE("Get error on bool expression with incompatible operand value types", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "5 || 9";

    const EvalResult& result = expressionHandler.handleExpression(expr);

    REQUIRE(result.error.errorCode == EvalError::INCOMPATIBLE_OPERAND_TYPES);
}

TEST_CASE("Get error on eval of bool expression with incompatible Identifier value types", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 5";
    std::string expr2 = "var b = false";
    std::string expr3 = "a || b";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const EvalResult& result = expressionHandler.handleExpression(expr3);

    REQUIRE(result.error.errorCode == EvalError::INCOMPATIBLE_OPERAND_TYPES);
}

TEST_CASE("Comparison operator evaluation: LESS THAN", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "2 < 5";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    bool properResult = 2 < 5;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Comparison operator evaluation: GREATER THAN", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "2 > 5";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    bool properResult = 2 > 5;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Math Expressions comparison", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "(2 * 5) > (4 * 4)";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    bool properResult = (2 * 5) > (4 * 4);

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Negative values comparison", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "-4 < -2";

    const EvalResult& result = expressionHandler.handleExpression(expr);
    bool properResult = -4 < -2;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Variables comparison", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 5";
    std::string expr2 = "var b = 10";
    std::string expr3 = "a < b";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const EvalResult& result = expressionHandler.handleExpression(expr3);
    bool properResult = 5 < 10;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Negative variables comparison", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 5";
    std::string expr2 = "var b = 10";
    std::string expr3 = "-a < -b";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const EvalResult& result = expressionHandler.handleExpression(expr3);
    bool properResult = -5 < -10;

    REQUIRE(result.getResultBool() == properResult);
}

TEST_CASE("Get error on comparison of incompatible value types [1]", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "false < true";

    const EvalResult& result = expressionHandler.handleExpression(expr);

    REQUIRE(result.error.errorCode == EvalError::INCOMPATIBLE_OPERAND_TYPES);
}

TEST_CASE("Get error on comparison of incompatible value types [2]", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "false > (5 + 6)";

    const EvalResult& result = expressionHandler.handleExpression(expr);

    REQUIRE(result.error.errorCode == EvalError::INCOMPATIBLE_OPERAND_TYPES);
}

TEST_CASE("Get error on comparison of incompatible Identifier value types [1]", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 5";
    std::string expr2 = "var b = 3 == 3";
    std::string expr3 = "a < b";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    const EvalResult& result = expressionHandler.handleExpression(expr3);

    REQUIRE(result.error.errorCode == EvalError::INCOMPATIBLE_OPERAND_TYPES);
}

TEST_CASE("Evaluate if block statement (condition is true)", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 5";
    std::string expr2 = "if (a > 0) {"
                        "a\n"
                        "}";

    expressionHandler.handleExpression(expr1);

    EvalResult result = expressionHandler.handleExpression(expr2);
    REQUIRE(result.getResultType() == ValueType::Compound);

    const std::vector<EvalResult> blockResult = result.getResultBlock();
    REQUIRE(blockResult.size() == 1);
    REQUIRE(blockResult[0].getResultDouble() == 5);
}

TEST_CASE("Do not evaluate if block statement (condition is false)", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 5";
    std::string expr2 = "if (a == 10) {"
                        "a\n"
                        "}";

    expressionHandler.handleExpression(expr1);

    EvalResult result = expressionHandler.handleExpression(expr2);
    REQUIRE(result.getResultType() == ValueType::Compound);
    REQUIRE(result.getResultBlock().empty());
}

TEST_CASE("Evaluate variable using inside block scope, outer scope is global", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 5";
    std::string expr2 = "if (a > 0 && a < 10) {"
                        "var a = 150\n"
                        "a\n"
                        "}";

    expressionHandler.handleExpression(expr1);

    EvalResult result = expressionHandler.handleExpression(expr2);
    REQUIRE(result.getResultType() == ValueType::Compound);

    const std::vector<EvalResult> blockResult = result.getResultBlock();
    REQUIRE(blockResult.size() == 2);
    REQUIRE(blockResult[1].getResultDouble() == 150);
}

TEST_CASE("Evaluate variables using inside block scope, outer scope is block scope", "[Evaluator]") {
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

    EvalResult result = expressionHandler.handleExpression(expr2);
    REQUIRE(result.getResultType() == ValueType::Compound);

    const std::vector<EvalResult> mainBlock = result.getResultBlock();
    const std::vector<EvalResult> innerBlock = mainBlock[1].getResultBlock();

    REQUIRE(mainBlock.size() == 3);
    REQUIRE(mainBlock[1].getResultType() == ValueType::Compound);
    REQUIRE(mainBlock[2].getResultBool());

    REQUIRE(innerBlock.size() == 2);
    REQUIRE(innerBlock[1].getResultBool());
}

TEST_CASE("Assign value to variable in outer scope", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 705";
    std::string expr2 = "if (a == 705) {"
                        "a = 1\n"
                        "}";
    std::string expr3 = "a";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    EvalResult result = expressionHandler.handleExpression(expr3);
    REQUIRE(result.getResultDouble() == 1);
}

TEST_CASE("Assert variable destroying after it goes out of scope", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "if (true) {"
                        "var a = 505\n"
                        "}";
    std::string expr2 = "a";

    expressionHandler.handleExpression(expr1);

    EvalResult result = expressionHandler.handleExpression(expr2);
    REQUIRE(result.isError());
    REQUIRE(result.error.errorCode == EvalError::UNDECLARED_VAR);
}

TEST_CASE("Assert variable destroying after it goes out of scope inside another block scope", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "if (true) {"
                        "if (true) {"
                        "var b = 400\n"
                        "}\n"
                        "b\n"
                        "}";

    EvalResult result = expressionHandler.handleExpression(expr);

    const std::vector<EvalResult> mainBlock = result.getResultBlock();

    REQUIRE(mainBlock[1].isError());
    REQUIRE(mainBlock[1].error.errorCode == EvalError::UNDECLARED_VAR);
}