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
#include "../SemanticAnalyzer.h"

class ExpressionHandler {
private:
    static Lexer lexer;

    static Parser parser;

    SemanticAnalyzer semanticAnalyzer = SemanticAnalyzer(0);

    Evaluator evaluator;

    std::vector<ProgramTranslationNode*> nodes;
public:
    EvalResult handleExpression(const std::string src) {
        std::string expr = src;
        expr.push_back('\n');
        expr.push_back(EOF);

        const TokenContainer& tokens = lexer.tokenize(expr);

        ProgramTranslationNode* root = parser.parse(tokens);
        SemanticAnalysisResult checkResult = semanticAnalyzer.checkProgram(root);
        if (checkResult.isError()) {
            throw std::runtime_error(checkResult.what());
        }
        const EvalResult& result = evaluator.Evaluate(root->statements[0]);

        nodes.emplace_back(root);

        return result;
    }

    ~ExpressionHandler() {
        for (const auto& currentNode : nodes) {
            delete currentNode;
        }
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

    const EvalResult& result = expressionHandler.handleExpression(expr);

    REQUIRE(result.getResultBool() == true);
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

TEST_CASE("Evaluate if statement (condition is true)", "[Evaluator]") {
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

TEST_CASE("Evaluate if-else functions call statement (condition is true)", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 5";
    std::string expr2 = "var b = 10";
    std::string expr3 = "func int add() {"
                        "return a + b\n"
                        "}";
    std::string expr4 = "func int mul() {"
                        "return a * b\n"
                        "}";
    std::string expr5 = "if (1 == 1) {"
                        "add()\n"
                        "} else {"
                        "mul()\n"
                        "}";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);
    expressionHandler.handleExpression(expr3);
    expressionHandler.handleExpression(expr4);

    EvalResult result = expressionHandler.handleExpression(expr5);
    REQUIRE(result.getResultType() == ValueType::Compound);

    const std::vector<EvalResult> blockResult = result.getResultBlock();
    REQUIRE(blockResult[0].getResultType() == ValueType::Number);
    REQUIRE(blockResult[0].getResultDouble() == 15);
}

TEST_CASE("Evaluate if-else functions call statement (condition is false)", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 5";
    std::string expr2 = "var b = 10";
    std::string expr3 = "func int add() {"
                        "return a + b\n"
                        "}";
    std::string expr4 = "func int mul() {"
                        "return a * b\n"
                        "}";

    std::string expr5 = "if (true && false) {"
                        "add()\n"
                        "} else {"
                        "mul()\n"
                        "}";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);
    expressionHandler.handleExpression(expr3);
    expressionHandler.handleExpression(expr4);

    EvalResult result = expressionHandler.handleExpression(expr5);
    REQUIRE(result.getResultType() == ValueType::Compound);

    const std::vector<EvalResult> blockResult = result.getResultBlock();
    REQUIRE(blockResult[0].getResultType() == ValueType::Number);
    REQUIRE(blockResult[0].getResultDouble() == 50);
}

TEST_CASE("Evaluate if-else assign statement (condition is false)", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a";
    std::string expr2 = "var b = false";
    std::string expr3 = "if (b) {"
                        "a = 5\n"
                        "} else {"
                        "a = 3\n"
                        "}";
    std::string expr4 = "a";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);
    expressionHandler.handleExpression(expr3);

    EvalResult result = expressionHandler.handleExpression(expr4);
    REQUIRE(result.getResultType() == ValueType::Number);
    REQUIRE(result.getResultDouble() == 3);
}

TEST_CASE("Do not evaluate if statement (condition is false)", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 5";
    std::string expr2 = "if (a == 10) {"
                        "a\n"
                        "}";

    expressionHandler.handleExpression(expr1);

    EvalResult result = expressionHandler.handleExpression(expr2);
    REQUIRE(result.getResultType() == ValueType::Void);
}

TEST_CASE("Use local variables but not outer scope variables with the same name, outer scope is global",
          "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 5";
    std::string expr2 = "if (a > 0 && a < 10) {"
                        "var a = 150\n"
                        "a\n"
                        "}";

    expressionHandler.handleExpression(expr1);

    EvalResult result = expressionHandler.handleExpression(expr2);

    const std::vector<EvalResult> blockResult = result.getResultBlock();
    REQUIRE(blockResult.size() == 2);
    REQUIRE(blockResult[1].getResultDouble() == 150);
}

TEST_CASE("Use local variables but not outer scope variables with the same name, outer scope is block scope",
          "[Evaluator]") {
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

TEST_CASE("Evaluate for statement declaring loop control variable in for initialization", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "for (var i = 0; i < 5; i = i + 1) {"
                       "i\n"
                       "}";

    EvalResult result = expressionHandler.handleExpression(expr);
    REQUIRE(result.getResultType() == ValueType::Compound);

    const std::vector<EvalResult> blockResult = result.getResultBlock();
    REQUIRE(blockResult.size() == 5);
    REQUIRE(blockResult[0].getResultBlock()[0].getResultDouble() == 0.0);
    REQUIRE(blockResult[1].getResultBlock()[0].getResultDouble() == 1);
    REQUIRE(blockResult[2].getResultBlock()[0].getResultDouble() == 2);
    REQUIRE(blockResult[3].getResultBlock()[0].getResultDouble() == 3);
    REQUIRE(blockResult[4].getResultBlock()[0].getResultDouble() == 4);
}

TEST_CASE("Evaluate for statement declaring loop control variable out of for initialization", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var i = 0";
    std::string expr2 = "for (; i < 5; i = i + 1) {"
                        "i\n"
                        "}";

    expressionHandler.handleExpression(expr1);

    EvalResult result = expressionHandler.handleExpression(expr2);
    REQUIRE(result.getResultType() == ValueType::Compound);

    const std::vector<EvalResult> blockResult = result.getResultBlock();
    REQUIRE(blockResult.size() == 5);
    REQUIRE(blockResult[0].getResultBlock()[0].getResultDouble() == 0.0);
    REQUIRE(blockResult[1].getResultBlock()[0].getResultDouble() == 1);
    REQUIRE(blockResult[2].getResultBlock()[0].getResultDouble() == 2);
    REQUIRE(blockResult[3].getResultBlock()[0].getResultDouble() == 3);
    REQUIRE(blockResult[4].getResultBlock()[0].getResultDouble() == 4);
}

TEST_CASE("Evaluate for statement declaring loop control variable out of for initialization but assign inside",
          "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var i";
    std::string expr2 = "for (i = 0; i < 5; i = i + 1) {"
                        "i\n"
                        "}";

    expressionHandler.handleExpression(expr1);

    EvalResult result = expressionHandler.handleExpression(expr2);
    REQUIRE(result.getResultType() == ValueType::Compound);

    const std::vector<EvalResult> blockResult = result.getResultBlock();
    REQUIRE(blockResult.size() == 5);
    REQUIRE(blockResult[0].getResultBlock()[0].getResultDouble() == 0.0);
    REQUIRE(blockResult[1].getResultBlock()[0].getResultDouble() == 1);
    REQUIRE(blockResult[2].getResultBlock()[0].getResultDouble() == 2);
    REQUIRE(blockResult[3].getResultBlock()[0].getResultDouble() == 3);
    REQUIRE(blockResult[4].getResultBlock()[0].getResultDouble() == 4);
}

TEST_CASE("Evaluate empty for block statement", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "for (var i = 0; i < 5; i = i + 1) {}";

    EvalResult result = expressionHandler.handleExpression(expr);
    const std::vector<EvalResult> blockResult = result.getResultBlock();
    REQUIRE(blockResult.size() == 5);
    REQUIRE(blockResult[0].getResultBlock().size() == 0);
    REQUIRE(blockResult[1].getResultBlock().size() == 0);
    REQUIRE(blockResult[2].getResultBlock().size() == 0);
    REQUIRE(blockResult[3].getResultBlock().size() == 0);
    REQUIRE(blockResult[4].getResultBlock().size() == 0);
}

TEST_CASE("Assert using of local loop control variable in for statement but not variable in out of scope",
          "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var i = 10";
    std::string expr = "for (var i = -5; i < 0; i = i + 1) {"
                       "i\n"
                       "}";

    EvalResult result = expressionHandler.handleExpression(expr);
    REQUIRE(result.getResultType() == ValueType::Compound);

    const std::vector<EvalResult> blockResult = result.getResultBlock();
    REQUIRE(blockResult.size() == 5);
    REQUIRE(blockResult[0].getResultBlock()[0].getResultDouble() == -5);
    REQUIRE(blockResult[1].getResultBlock()[0].getResultDouble() == -4);
    REQUIRE(blockResult[2].getResultBlock()[0].getResultDouble() == -3);
    REQUIRE(blockResult[3].getResultBlock()[0].getResultDouble() == -2);
    REQUIRE(blockResult[4].getResultBlock()[0].getResultDouble() == -1);
}

TEST_CASE("Evaluate for statement inside another for statement", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "for (var i = 55; i < 56; i = i + 1) {"
                       "i\n"
                       "for (var i = 100; i < 101; i = i + 1) {"
                       "i\n"
                       "}\n"
                       "}";

    EvalResult result = expressionHandler.handleExpression(expr);
    REQUIRE(result.getResultType() == ValueType::Compound);

    const std::vector<EvalResult> blockResult = result.getResultBlock();
    REQUIRE(blockResult[0].getResultBlock().size() == 2);
    REQUIRE(blockResult[0].getResultBlock()[0].getResultType() == ValueType::Number);
    REQUIRE(blockResult[0].getResultBlock()[0].getResultDouble() == 55);
    REQUIRE(blockResult[0].getResultBlock()[1].getResultType() == ValueType::Compound);
    REQUIRE(blockResult[0].getResultBlock()[1].getResultBlock()[0].getResultBlock()[0].getResultType() ==
            ValueType::Number);
    REQUIRE(blockResult[0].getResultBlock()[1].getResultBlock()[0].getResultBlock()[0].getResultDouble() == 100);
}

TEST_CASE("Use global variable in function", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 10";
    std::string expr2 = "var b = 50";
    std::string expr3 = "func int add() {"
                        "return a + b\n"
                        "}";

    std::string expr4 = "add()";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);
    expressionHandler.handleExpression(expr3);

    EvalResult result = expressionHandler.handleExpression(expr4);

    REQUIRE(result.getResultType() == ValueType::Number);
    REQUIRE(result.getResultDouble() == 10 + 50);
}

TEST_CASE("Pass rvalue to function [0]. Pass number value", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func int getInt(var int a) {"
                        "return a\n"
                        "}";
    std::string expr2 = "getInt(5)";

    expressionHandler.handleExpression(expr1);

    EvalResult result = expressionHandler.handleExpression(expr2);

    REQUIRE(result.getResultType() == ValueType::Number);
    REQUIRE(result.getResultDouble() == 5);
}

TEST_CASE("Pass rvalue to function [1]. Pass bool value", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func bool getBool(var bool a) {"
                        "return a\n"
                        "}";
    std::string expr2 = "getBool(true)";

    expressionHandler.handleExpression(expr1);

    EvalResult result = expressionHandler.handleExpression(expr2);

    REQUIRE(result.getResultType() == ValueType::Bool);
    REQUIRE(result.getResultBool() == true);
}

TEST_CASE("Pass math expression to function", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func int getNumberExpr(var int a) {"
                        "return a\n"
                        "}";
    std::string expr2 = "getNumberExpr(100 * 500)";

    expressionHandler.handleExpression(expr1);

    EvalResult result = expressionHandler.handleExpression(expr2);

    REQUIRE(result.getResultType() == ValueType::Number);
    REQUIRE(result.getResultDouble() == 100 * 500);
}

TEST_CASE("Pass bool expression to function", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func bool getBoolExpr(var bool a) {"
                        "return a\n"
                        "}";
    std::string expr2 = "var a = true";
    std::string expr3 = "var b = false";
    std::string expr4 = "getBoolExpr(a || b)";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);
    expressionHandler.handleExpression(expr3);

    EvalResult result = expressionHandler.handleExpression(expr4);

    REQUIRE(result.getResultType() == ValueType::Bool);
    REQUIRE(result.getResultBool() == true);
}

TEST_CASE("Call function in if statement", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func int getInt(var int a) {"
                        "return a\n"
                        "}";
    std::string expr2 = "if (true) {"
                        "var a = 50\n"
                        "getInt(a)\n"
                        "}";

    expressionHandler.handleExpression(expr1);

    EvalResult result = expressionHandler.handleExpression(expr2);

    const auto& blockResult = result.getResultBlock();
    REQUIRE(blockResult[1].getResultType() == ValueType::Number);
    REQUIRE(blockResult[1].getResultDouble() == 50);
}

TEST_CASE("Call function in for statement", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func int getInt(var int a) {"
                        "return a\n"
                        "}";
    std::string expr2 = "for (var i = 0; i < 10; i = i + 1) {"
                        "getInt(i)\n"
                        "}";

    expressionHandler.handleExpression(expr1);

    EvalResult result = expressionHandler.handleExpression(expr2);

    const auto& blockResult = result.getResultBlock();

    for (int i = 0; i < 10; i++) {
        REQUIRE(blockResult[i].getResultBlock()[0].getResultDouble() == i);
    }
}

TEST_CASE("Assert that parameter is passed by value", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func void assign(var int a, var int b) {"
                        "a = b\n"
                        "}";
    std::string expr2 = "var a = 5";
    std::string expr3 = "assign(a, 10)";
    std::string expr4 = "a";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);
    expressionHandler.handleExpression(expr3);

    EvalResult result = expressionHandler.handleExpression(expr4);

    REQUIRE(result.getResultDouble() == 5);
}

TEST_CASE("Assert that variable is assigned correctly to outer variable with same name",
          "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "if (true) {"
                       "var a = 5\n"
                       "if (true) {"
                       "var a = a\n"
                       "a\n"
                       "}\n"
                       "a\n"
                       "}";

    EvalResult result = expressionHandler.handleExpression(expr);

    const auto& blockResult = result.getResultBlock();
    REQUIRE(blockResult[1].getResultBlock()[1].getResultType() == ValueType::Number);
    REQUIRE(blockResult[1].getResultBlock()[1].getResultDouble() == 5);
}

TEST_CASE("Assert that variable inside block is assigned correctly to outer variable", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "if (true) {"
                       "var a = 15\n"
                       "if (true) {"
                       "var b\n"
                       "b = a\n"
                       "b\n"
                       "}\n"
                       "a\n"
                       "}";

    EvalResult result = expressionHandler.handleExpression(expr);

    const auto& blockResult = result.getResultBlock();
    REQUIRE(blockResult[1].getResultBlock()[2].getResultType() == ValueType::Number);
    REQUIRE(blockResult[1].getResultBlock()[2].getResultDouble() == 15);
}

TEST_CASE("Assert that outer variable value can be changed inside block", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "if (true) {"
                       "var a = 10\n"
                       "if (true) {"
                       "a = 20\n"
                       "}\n"
                       "a\n"
                       "}";

    EvalResult result = expressionHandler.handleExpression(expr);

    const auto& blockResult = result.getResultBlock();
    REQUIRE(blockResult[2].getResultType() == ValueType::Number);
    REQUIRE(blockResult[2].getResultDouble() == 20);
}

TEST_CASE("Assert that changing value of variable inside block not affects on outer variable", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "if (true) {"
                       "var a = 10\n"
                       "if (true) {"
                       "var a"
                       "a = 20\n"
                       "}\n"
                       "a\n"
                       "}";

    EvalResult result = expressionHandler.handleExpression(expr);

    const auto& blockResult = result.getResultBlock();
    REQUIRE(blockResult[2].getResultType() == ValueType::Number);
    REQUIRE(blockResult[2].getResultDouble() == 10);
}

TEST_CASE("Assign function call result to variable", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func int add(var int a, var int b) {"
                        "return a + b\n"
                        "}";
    std::string expr2 = "var a = add(5, 10)";
    std::string expr3 = "a";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    EvalResult result = expressionHandler.handleExpression(expr3);

    

    REQUIRE(result.getResultType() == ValueType::Number);
    REQUIRE(result.getResultDouble() == 15);
}

TEST_CASE("Use function call result in math expression", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func int mul(var int a, var int b) {"
                        "return a * b\n"
                        "}";
    std::string expr2 = "(mul(3, 5) + mul(2, 10)) / 5";

    expressionHandler.handleExpression(expr1);

    EvalResult result = expressionHandler.handleExpression(expr2);

    REQUIRE(result.getResultType() == ValueType::Number);
    REQUIRE(result.getResultDouble() == 7);
}

TEST_CASE("Assert break statement stops for loop evaluating", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "for (var i = 0; i < 10; i = i + 1) {"
                       "i\n"
                       "break\n"
                       "}";

    EvalResult result = expressionHandler.handleExpression(expr);

    REQUIRE(result.getResultType() == ValueType::Compound);

    const std::vector<EvalResult> blockResult = result.getResultBlock();
    REQUIRE(blockResult.size() == 1);
}

TEST_CASE("Assert break statement stops loop evaluating inside block", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "for (var i = 0; i < 10; i = i + 1) {"
                       "i\n"
                       "if (i == 5) {"
                       "var b = 3 * i\n"
                       "if (b == 15) {"
                       "break\n"
                       "}"
                       "}"
                       "}";

    EvalResult result = expressionHandler.handleExpression(expr);
    
    REQUIRE(result.getResultType() == ValueType::Compound);

    const std::vector<EvalResult> blockResult = result.getResultBlock();
    REQUIRE(blockResult.size() == 6);
}

TEST_CASE("Assert break statement stops nested for loop", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr = "for (var i = 0; i < 5; i = i + 1) {"
                       "i\n"
                       "for (var i1 = 10; i1 < 25; i1 = i1 + 1) {"
                       "i1\n"
                       "if (i1 == 20) {"
                       "break\n"
                       "}"
                       "}"
                       "}";

    EvalResult result = expressionHandler.handleExpression(expr);
    
    const std::vector<EvalResult> blockResult = result.getResultBlock();
    REQUIRE(blockResult.size() == 5);

    for (unsigned long i = 0; i < blockResult.size(); i++) {
        REQUIRE(blockResult[i].getResultBlock()[1].getResultBlock().size() == 11);
    }
}

TEST_CASE("Assert functions can take function calls as parameters", "[Evaluator]") {
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

    EvalResult result = expressionHandler.handleExpression(expr3);
    
    REQUIRE(result.getResultType() == ValueType::Number);
    REQUIRE(result.getResultDouble() == 3500);
}

TEST_CASE("Assert functions can return function calls", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func int mul(var int a, var int b) {"
                        "return a * b\n"
                        "}";

    std::string expr2 = "func int add_mul(var int a, var int b) {"
                        "return mul(a, b) + mul(a, b)\n"
                        "}";

    std::string expr3 = "add_mul(5, 10)";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    EvalResult result = expressionHandler.handleExpression(expr3);

    
    REQUIRE(result.getResultType() == ValueType::Number);
    REQUIRE(result.getResultDouble() == 100);
}

TEST_CASE("Evaluate if-else if statement (else if condition is true)", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 5";
    std::string expr2 = "if (a < 0) {"
                        "a = -1\n"
                        "} else if (a == 5) {"
                        "a = 0\n"
                        "} else {"
                        "a = 1\n"
                        "}";
    std::string expr3 = "a";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    EvalResult result = expressionHandler.handleExpression(expr3);
    REQUIRE(result.getResultType() == ValueType::Number);
    REQUIRE(result.getResultDouble() == 0);
}

TEST_CASE("Evaluate nested if statement inside another if statement", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "var a = 5";
    std::string expr2 = "if (a < 0) {"
                        "a = -1\n"
                        "} else {"
                        "   if (a == 0) {"
                        "       a = 0\n"
                        "   } else if (a == 5) {"
                        "       a = 1\n"
                        "   } else {"
                        "       a == 2\n"
                        "   }"
                        "}";
    std::string expr3 = "a";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    EvalResult result = expressionHandler.handleExpression(expr3);
    REQUIRE(result.getResultType() == ValueType::Number);
    REQUIRE(result.getResultDouble() == 1);
}

TEST_CASE("Return statement inside if and for", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "func int getInt() {"
                       "var a = 10\n"
                       "if (a > 0) {"
                       "    for (var i = 0; i < 10; i = i + 1) {"
                       "        if (i == 5) {"
                       "            return i\n"
                       "        }\n"
                       "    }\n"
                       "    return 50\n"
                       "}\n"
                       "return 100\n"
                       "}";
    std::string expr2 = "var a = getInt()";
    std::string expr3 = "a";

    expressionHandler.handleExpression(expr1);
    expressionHandler.handleExpression(expr2);

    EvalResult result = expressionHandler.handleExpression(expr3);
    REQUIRE(result.getResultType() == ValueType::Number);
    REQUIRE(result.getResultDouble() == 5);
}

TEST_CASE("Use built-in print function : pass number expr", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "print(200 * 5)";

    EvalResult result = expressionHandler.handleExpression(expr1);
    REQUIRE(result.getResultType() == ValueType::Number);
    REQUIRE(result.getResultDouble() == 1000);
}

TEST_CASE("Use built-in print function : pass bool expr", "[Evaluator]") {
    ExpressionHandler expressionHandler;

    std::string expr1 = "print(true || false)";

    EvalResult result = expressionHandler.handleExpression(expr1);
    REQUIRE(result.getResultType() == ValueType::Bool);
    REQUIRE(result.getResultBool() == true);
}