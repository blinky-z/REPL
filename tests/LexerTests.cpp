#include "catch.hpp"
#include "../Lexer.h"
#include "../AnalysisContainer.h"
#include "../Token.h"
#include "../Identifier.h"

TEST_CASE("Addition", "[Math operations tokenizing]") {
    string expr = "313 + 425 + 1131";

    Lexer lexer;
    AnalysisContainer data = lexer.tokenize(expr);
    const vector<Token>& tokens = data.GetTokens();

    vector<Token> properTokens;
    properTokens.emplace_back(Token{data.Num, "313"});
    properTokens.emplace_back(Token{data.Add, "+"});
    properTokens.emplace_back(Token{data.Num, "425"});
    properTokens.emplace_back(Token{data.Add, "+"});
    properTokens.emplace_back(Token{data.Num, "1131"});

    REQUIRE(tokens.size() == properTokens.size());
    if (tokens.size() == properTokens.size()) {
        unsigned long currentTokenNum = 0;
        for (; currentTokenNum < tokens.size(); currentTokenNum++) {
            REQUIRE(tokens[currentTokenNum].type == properTokens[currentTokenNum].type);
            REQUIRE(tokens[currentTokenNum].value == properTokens[currentTokenNum].value);
        }
    }
}

TEST_CASE("Subtraction", "[Math operations tokenizing]") {
    string expr = "1000 - 100 - 255";

    Lexer lexer;
    AnalysisContainer data = lexer.tokenize(expr);

    const vector<Token>& tokens = data.GetTokens();

    vector<Token> properTokens;
    properTokens.emplace_back(Token{data.Num, "1000"});
    properTokens.emplace_back(Token{data.Sub, "-"});
    properTokens.emplace_back(Token{data.Num, "100"});
    properTokens.emplace_back(Token{data.Sub, "-"});
    properTokens.emplace_back(Token{data.Num, "255"});

    REQUIRE(tokens.size() == properTokens.size());
    if (tokens.size() == properTokens.size()) {
        unsigned long currentTokenNum = 0;
        for (; currentTokenNum < tokens.size(); currentTokenNum++) {
            REQUIRE(tokens[currentTokenNum].type == properTokens[currentTokenNum].type);
            REQUIRE(tokens[currentTokenNum].value == properTokens[currentTokenNum].value);
        }
    }
}

TEST_CASE("Multiplication", "[Math operations tokenizing]") {
    string expr = "5 * 123 * 3464";

    Lexer lexer;
    AnalysisContainer data = lexer.tokenize(expr);

    const vector<Token>& tokens = data.GetTokens();

    vector<Token> properTokens;
    properTokens.emplace_back(Token{data.Num, "5"});
    properTokens.emplace_back(Token{data.Mul, "*"});
    properTokens.emplace_back(Token{data.Num, "123"});
    properTokens.emplace_back(Token{data.Mul, "*"});
    properTokens.emplace_back(Token{data.Num, "3464"});

    REQUIRE(tokens.size() == properTokens.size());
    if (tokens.size() == properTokens.size()) {
        unsigned long currentTokenNum = 0;
        for (; currentTokenNum < tokens.size(); currentTokenNum++) {
            REQUIRE(tokens[currentTokenNum].type == properTokens[currentTokenNum].type);
            REQUIRE(tokens[currentTokenNum].value == properTokens[currentTokenNum].value);
        }
    }
}

TEST_CASE("Division", "[Math operations tokenizing]") {
    string expr = "2500 / 5 / 10";

    Lexer lexer;
    AnalysisContainer data = lexer.tokenize(expr);

    const vector<Token>& tokens = data.GetTokens();

    vector<Token> properTokens;
    properTokens.emplace_back(Token{data.Num, "2500"});
    properTokens.emplace_back(Token{data.Div, "/"});
    properTokens.emplace_back(Token{data.Num, "5"});
    properTokens.emplace_back(Token{data.Div, "/"});
    properTokens.emplace_back(Token{data.Num, "10"});

    REQUIRE(tokens.size() == properTokens.size());
    if (tokens.size() == properTokens.size()) {
        unsigned long currentTokenNum = 0;
        for (; currentTokenNum < tokens.size(); currentTokenNum++) {
            REQUIRE(tokens[currentTokenNum].type == properTokens[currentTokenNum].type);
            REQUIRE(tokens[currentTokenNum].value == properTokens[currentTokenNum].value);
        }
    }
}