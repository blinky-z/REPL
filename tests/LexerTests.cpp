#include "catch.hpp"
#include "../Lexer.h"
#include "../TokenContainer.h"
#include "../Token.h"
#include "../TokenTypes.h"
#include "../Identifier.h"
#include <vector>

Lexer LexerTestsLexer;

void matchTokens(const std::vector<Token>& tokens, const std::vector<Token>& properTokens) {
    REQUIRE(tokens.size() == properTokens.size());

    if (tokens.size() == properTokens.size()) {
        unsigned long currentTokenNum = 0;
        for (; currentTokenNum < tokens.size(); currentTokenNum++) {
            REQUIRE(tokens[currentTokenNum].Type == properTokens[currentTokenNum].Type);
            REQUIRE(tokens[currentTokenNum].Value == properTokens[currentTokenNum].Value);
        }
    }
}

TEST_CASE("Addition Tokenizing", "[Lexer][Math operations tokenizing]") {
    std::string expr = "313 + 425 + 1131";
    expr.push_back(EOF);
    
    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Num, "313"});
    properTokens.emplace_back(Token{TokenTypes::Add, "+"});
    properTokens.emplace_back(Token{TokenTypes::Num, "425"});
    properTokens.emplace_back(Token{TokenTypes::Add, "+"});
    properTokens.emplace_back(Token{TokenTypes::Num, "1131"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Subtraction Tokenizing", "[Lexer][Math operations tokenizing]") {
    std::string expr = "1000 - 100 - 255";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Num, "1000"});
    properTokens.emplace_back(Token{TokenTypes::Sub, "-"});
    properTokens.emplace_back(Token{TokenTypes::Num, "100"});
    properTokens.emplace_back(Token{TokenTypes::Sub, "-"});
    properTokens.emplace_back(Token{TokenTypes::Num, "255"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Multiplication Tokenizing", "[Lexer][Math operations tokenizing]") {
    std::string expr = "5 * 123 * 3464";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Num, "5"});
    properTokens.emplace_back(Token{TokenTypes::Mul, "*"});
    properTokens.emplace_back(Token{TokenTypes::Num, "123"});
    properTokens.emplace_back(Token{TokenTypes::Mul, "*"});
    properTokens.emplace_back(Token{TokenTypes::Num, "3464"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Negative numbers multiplication Tokenizing", "[Lexer][Math operations tokenizing]") {
    std::string expr = "5 * -123 * -3464";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Num, "5"});
    properTokens.emplace_back(Token{TokenTypes::Mul, "*"});
    properTokens.emplace_back(Token{TokenTypes::Sub, "-"});
    properTokens.emplace_back(Token{TokenTypes::Num, "123"});
    properTokens.emplace_back(Token{TokenTypes::Mul, "*"});
    properTokens.emplace_back(Token{TokenTypes::Sub, "-"});
    properTokens.emplace_back(Token{TokenTypes::Num, "3464"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Division Tokenizing", "[Lexer][Math operations tokenizing]") {
    std::string expr = "2500 / 5 / 10";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Num, "2500"});
    properTokens.emplace_back(Token{TokenTypes::Div, "/"});
    properTokens.emplace_back(Token{TokenTypes::Num, "5"});
    properTokens.emplace_back(Token{TokenTypes::Div, "/"});
    properTokens.emplace_back(Token{TokenTypes::Num, "10"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Multiple operations", "[Lexer][Math operations tokenizing]") {
    std::string expr = "2500 * 5 / 10 + 1000";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Num, "2500"});
    properTokens.emplace_back(Token{TokenTypes::Mul, "*"});
    properTokens.emplace_back(Token{TokenTypes::Num, "5"});
    properTokens.emplace_back(Token{TokenTypes::Div, "/"});
    properTokens.emplace_back(Token{TokenTypes::Num, "10"});
    properTokens.emplace_back(Token{TokenTypes::Add, "+"});
    properTokens.emplace_back(Token{TokenTypes::Num, "1000"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Expressions with using of round brackets", "[Lexer][Math operations tokenizing]") {
    std::string expr = "2500 * 5 / (1500 + 1000)";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Num, "2500"});
    properTokens.emplace_back(Token{TokenTypes::Mul, "*"});
    properTokens.emplace_back(Token{TokenTypes::Num, "5"});
    properTokens.emplace_back(Token{TokenTypes::Div, "/"});
    properTokens.emplace_back(Token{TokenTypes::ROUND_BRACKET_START, "("});
    properTokens.emplace_back(Token{TokenTypes::Num, "1500"});
    properTokens.emplace_back(Token{TokenTypes::Add, "+"});
    properTokens.emplace_back(Token{TokenTypes::Num, "1000"});
    properTokens.emplace_back(Token{TokenTypes::ROUND_BRACKET_END, ")"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Id declare tokenizing", "[Lexer]") {
    std::string expr = "var a";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::DeclareId, "var"});
    properTokens.emplace_back(Token{TokenTypes::Id, "a"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Id assign tokenizing", "[Lexer]") {
    std::string expr = "a = 3";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Id, "a"});
    properTokens.emplace_back(Token{TokenTypes::Assign, "="});
    properTokens.emplace_back(Token{TokenTypes::Num, "3"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Id declare & assign tokenizing", "[Lexer]") {
    std::string expr = "var a = 3";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::DeclareId, "var"});
    properTokens.emplace_back(Token{TokenTypes::Id, "a"});
    properTokens.emplace_back(Token{TokenTypes::Assign, "="});
    properTokens.emplace_back(Token{TokenTypes::Num, "3"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Expressions with using of variables", "[Lexer][Math operations tokenizing]") {
    std::string expr = "a * 5 / (1500 + b)";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Id, "a"});
    properTokens.emplace_back(Token{TokenTypes::Mul, "*"});
    properTokens.emplace_back(Token{TokenTypes::Num, "5"});
    properTokens.emplace_back(Token{TokenTypes::Div, "/"});
    properTokens.emplace_back(Token{TokenTypes::ROUND_BRACKET_START, "("});
    properTokens.emplace_back(Token{TokenTypes::Num, "1500"});
    properTokens.emplace_back(Token{TokenTypes::Add, "+"});
    properTokens.emplace_back(Token{TokenTypes::Id, "b"});
    properTokens.emplace_back(Token{TokenTypes::ROUND_BRACKET_END, ")"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Expressions with using of variables and negative numbers", "[Lexer][Math operations tokenizing]") {
    std::string expr = "a * -5 / -(1500 + b)";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Id, "a"});
    properTokens.emplace_back(Token{TokenTypes::Mul, "*"});
    properTokens.emplace_back(Token{TokenTypes::Sub, "-"});
    properTokens.emplace_back(Token{TokenTypes::Num, "5"});
    properTokens.emplace_back(Token{TokenTypes::Div, "/"});
    properTokens.emplace_back(Token{TokenTypes::Sub, "-"});
    properTokens.emplace_back(Token{TokenTypes::ROUND_BRACKET_START, "("});
    properTokens.emplace_back(Token{TokenTypes::Num, "1500"});
    properTokens.emplace_back(Token{TokenTypes::Add, "+"});
    properTokens.emplace_back(Token{TokenTypes::Id, "b"});
    properTokens.emplace_back(Token{TokenTypes::ROUND_BRACKET_END, ")"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Tokenize float point number", "[Lexer][Math operations tokenizing]") {
    std::string expr = "2.2";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Num, "2.2"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Tokenize negative float point number", "[Lexer][Math operations tokenizing]") {
    std::string expr = "-2.2";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Sub, "-"});
    properTokens.emplace_back(Token{TokenTypes::Num, "2.2"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Expressions with using of float point numbers", "[Lexer][Math operations tokenizing]") {
    std::string expr = "a * -5.2 / 3.1";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Id, "a"});
    properTokens.emplace_back(Token{TokenTypes::Mul, "*"});
    properTokens.emplace_back(Token{TokenTypes::Sub, "-"});
    properTokens.emplace_back(Token{TokenTypes::Num, "5.2"});
    properTokens.emplace_back(Token{TokenTypes::Div, "/"});
    properTokens.emplace_back(Token{TokenTypes::Num, "3.1"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Id declare & assign float point number tokenizing", "[Lexer]") {
    std::string expr = "var a = 3.2";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::DeclareId, "var"});
    properTokens.emplace_back(Token{TokenTypes::Id, "a"});
    properTokens.emplace_back(Token{TokenTypes::Assign, "="});
    properTokens.emplace_back(Token{TokenTypes::Num, "3.2"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Id declare & assign negative float point number tokenizing", "[Lexer]") {
    std::string expr = "var a = -3.2";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::DeclareId, "var"});
    properTokens.emplace_back(Token{TokenTypes::Id, "a"});
    properTokens.emplace_back(Token{TokenTypes::Assign, "="});
    properTokens.emplace_back(Token{TokenTypes::Sub, "-"});
    properTokens.emplace_back(Token{TokenTypes::Num, "3.2"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Expression with whitespaces after all chars tokenizing", "[Lexer][Math operations tokenizing]") {
    std::string expr = "1 + 2    ";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Num, "1"});
    properTokens.emplace_back(Token{TokenTypes::Add, "+"});
    properTokens.emplace_back(Token{TokenTypes::Num, "2"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Get throw on expression that contains unknown char", "[Lexer][Math operations tokenizing]") {
    std::string expr = "1 + 2 ' ";
    expr.push_back(EOF);

    REQUIRE_THROWS(LexerTestsLexer.tokenize(expr));
}