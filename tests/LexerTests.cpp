#include "catch.hpp"
#include "../Lexer.h"
#include "../AnalysisContainer.h"

TEST_CASE("Addition", "[Math operations tokenizing]") {
    string expr = "313 + 425 + 1131";

    Lexer lexer;
    AnalysisContainer data = lexer.tokenize(expr);

    const vector<Token>& tokens = data.GetTokens();
}

TEST_CASE("Subtraction", "[Math operations tokenizing]") {
    string expr = "1000 - 100 - 255";

    Lexer lexer;
    AnalysisContainer data = lexer.tokenize(expr);

    const vector<Token>& tokens = data.GetTokens();
}

TEST_CASE("Multiplication", "[Math operations tokenizing]") {
    string expr = "5 * 123 * 3464";

    Lexer lexer;
    AnalysisContainer data = lexer.tokenize(expr);

    const vector<Token>& tokens = data.GetTokens();
}

TEST_CASE("Division", "[Math operations tokenizing]") {
    string expr = "2500 / 5 / 10";

    Lexer lexer;
    AnalysisContainer data = lexer.tokenize(expr);

    const vector<Token>& tokens = data.GetTokens();
}