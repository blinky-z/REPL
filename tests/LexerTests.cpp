#include "catch.hpp"
#include "../Lexer.h"
#include "../AnalysisContainer.h"
#include "../Token.h"
#include "../Identifier.h"

Lexer LexerTestsLexer;

TEST_CASE("Addition Tokenizing", "[Lexer][Math operations tokenizing]") {
    std::string expr = "313 + 425 + 1131";
    expr.push_back(EOF);
    
    AnalysisContainer data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.GetTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{data.Num, "313"});
    properTokens.emplace_back(Token{data.Add, "+"});
    properTokens.emplace_back(Token{data.Num, "425"});
    properTokens.emplace_back(Token{data.Add, "+"});
    properTokens.emplace_back(Token{data.Num, "1131"});
    properTokens.emplace_back(Token{data.eof, "EOF"});

    REQUIRE(tokens.size() == properTokens.size());
    if (tokens.size() == properTokens.size()) {
        unsigned long currentTokenNum = 0;
        for (; currentTokenNum < tokens.size(); currentTokenNum++) {
            REQUIRE(tokens[currentTokenNum].Type == properTokens[currentTokenNum].Type);
            REQUIRE(tokens[currentTokenNum].Value == properTokens[currentTokenNum].Value);
        }
    }
}

TEST_CASE("Subtraction Tokenizing", "[Lexer][Math operations tokenizing]") {
    std::string expr = "1000 - 100 - 255";
    expr.push_back(EOF);
    
    AnalysisContainer data = LexerTestsLexer.tokenize(expr);

    const std::vector<Token>& tokens = data.GetTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{data.Num, "1000"});
    properTokens.emplace_back(Token{data.Sub, "-"});
    properTokens.emplace_back(Token{data.Num, "100"});
    properTokens.emplace_back(Token{data.Sub, "-"});
    properTokens.emplace_back(Token{data.Num, "255"});
    properTokens.emplace_back(Token{data.eof, "EOF"});

    REQUIRE(tokens.size() == properTokens.size());
    if (tokens.size() == properTokens.size()) {
        unsigned long currentTokenNum = 0;
        for (; currentTokenNum < tokens.size(); currentTokenNum++) {
            REQUIRE(tokens[currentTokenNum].Type == properTokens[currentTokenNum].Type);
            REQUIRE(tokens[currentTokenNum].Value == properTokens[currentTokenNum].Value);
        }
    }
}

TEST_CASE("Multiplication Tokenizing", "[Lexer][Math operations tokenizing]") {
    std::string expr = "5 * 123 * 3464";
    expr.push_back(EOF);

    AnalysisContainer data = LexerTestsLexer.tokenize(expr);

    const std::vector<Token>& tokens = data.GetTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{data.Num, "5"});
    properTokens.emplace_back(Token{data.Mul, "*"});
    properTokens.emplace_back(Token{data.Num, "123"});
    properTokens.emplace_back(Token{data.Mul, "*"});
    properTokens.emplace_back(Token{data.Num, "3464"});
    properTokens.emplace_back(Token{data.eof, "EOF"});

    REQUIRE(tokens.size() == properTokens.size());
    if (tokens.size() == properTokens.size()) {
        unsigned long currentTokenNum = 0;
        for (; currentTokenNum < tokens.size(); currentTokenNum++) {
            REQUIRE(tokens[currentTokenNum].Type == properTokens[currentTokenNum].Type);
            REQUIRE(tokens[currentTokenNum].Value == properTokens[currentTokenNum].Value);
        }
    }
}

TEST_CASE("Division Tokenizing", "[Lexer][Math operations tokenizing]") {
    std::string expr = "2500 / 5 / 10";
    expr.push_back(EOF);
    
    AnalysisContainer data = LexerTestsLexer.tokenize(expr);

    const std::vector<Token>& tokens = data.GetTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{data.Num, "2500"});
    properTokens.emplace_back(Token{data.Div, "/"});
    properTokens.emplace_back(Token{data.Num, "5"});
    properTokens.emplace_back(Token{data.Div, "/"});
    properTokens.emplace_back(Token{data.Num, "10"});
    properTokens.emplace_back(Token{data.eof, "EOF"});

    REQUIRE(tokens.size() == properTokens.size());
    if (tokens.size() == properTokens.size()) {
        unsigned long currentTokenNum = 0;
        for (; currentTokenNum < tokens.size(); currentTokenNum++) {
            REQUIRE(tokens[currentTokenNum].Type == properTokens[currentTokenNum].Type);
            REQUIRE(tokens[currentTokenNum].Value == properTokens[currentTokenNum].Value);
        }
    }
}

TEST_CASE("Multiple operations", "[Lexer][Math operations tokenizing]") {
    std::string expr = "2500 * 5 / 10 + 1000";
    expr.push_back(EOF);

    AnalysisContainer data = LexerTestsLexer.tokenize(expr);

    const std::vector<Token>& tokens = data.GetTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{data.Num, "2500"});
    properTokens.emplace_back(Token{data.Mul, "*"});
    properTokens.emplace_back(Token{data.Num, "5"});
    properTokens.emplace_back(Token{data.Div, "/"});
    properTokens.emplace_back(Token{data.Num, "10"});
    properTokens.emplace_back(Token{data.Add, "+"});
    properTokens.emplace_back(Token{data.Num, "1000"});
    properTokens.emplace_back(Token{data.eof, "EOF"});

    REQUIRE(tokens.size() == properTokens.size());
    if (tokens.size() == properTokens.size()) {
        unsigned long currentTokenNum = 0;
        for (; currentTokenNum < tokens.size(); currentTokenNum++) {
            REQUIRE(tokens[currentTokenNum].Type == properTokens[currentTokenNum].Type);
            REQUIRE(tokens[currentTokenNum].Value == properTokens[currentTokenNum].Value);
        }
    }
}

TEST_CASE("Operations with using of round brackets", "[Lexer][Math operations tokenizing]") {
    std::string expr = "2500 * 5 / (1500 + 1000)";
    expr.push_back(EOF);

    AnalysisContainer data = LexerTestsLexer.tokenize(expr);

    const std::vector<Token>& tokens = data.GetTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{data.Num, "2500"});
    properTokens.emplace_back(Token{data.Mul, "*"});
    properTokens.emplace_back(Token{data.Num, "5"});
    properTokens.emplace_back(Token{data.Div, "/"});
    properTokens.emplace_back(Token{data.ROUND_BRACKET_START, "("});
    properTokens.emplace_back(Token{data.Num, "1500"});
    properTokens.emplace_back(Token{data.Add, "+"});
    properTokens.emplace_back(Token{data.Num, "1000"});
    properTokens.emplace_back(Token{data.ROUND_BRACKET_END, ")"});
    properTokens.emplace_back(Token{data.eof, "EOF"});

    REQUIRE(tokens.size() == properTokens.size());
    if (tokens.size() == properTokens.size()) {
        unsigned long currentTokenNum = 0;
        for (; currentTokenNum < tokens.size(); currentTokenNum++) {
            REQUIRE(tokens[currentTokenNum].Type == properTokens[currentTokenNum].Type);
            REQUIRE(tokens[currentTokenNum].Value == properTokens[currentTokenNum].Value);
        }
    }
}