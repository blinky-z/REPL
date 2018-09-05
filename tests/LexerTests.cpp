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

TEST_CASE("Negative numbers tokenizing", "[Lexer][Math operations tokenizing]") {
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

TEST_CASE("Apply unary minus to variable", "[Lexer]") {
    std::string expr = "-a";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Sub, "-"});
    properTokens.emplace_back(Token{TokenTypes::Id, "a"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Declare & Assign variable to variable with unary minus", "[Lexer]") {
    std::string expr = "var a = -b";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::DeclareId, "var"});
    properTokens.emplace_back(Token{TokenTypes::Id, "a"});
    properTokens.emplace_back(Token{TokenTypes::Assign, "="});
    properTokens.emplace_back(Token{TokenTypes::Sub, "-"});
    properTokens.emplace_back(Token{TokenTypes::Id, "b"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Id declare & assign to constant num tokenizing", "[Lexer]") {
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

TEST_CASE("Expression with redundant whitespaces tokenizing", "[Lexer][Math operations tokenizing]") {
    std::string expr = "   1    + 2    ";
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

TEST_CASE("Expression with using of operator LESS THAN", "[Lexer]") {
    std::string expr = "1 < 2";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Num, "1"});
    properTokens.emplace_back(Token{TokenTypes::LESS, "<"});
    properTokens.emplace_back(Token{TokenTypes::Num, "2"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Expression with using of operator GREATER THAN", "[Lexer]") {
    std::string expr = "1 > 2";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Num, "1"});
    properTokens.emplace_back(Token{TokenTypes::GREATER, ">"});
    properTokens.emplace_back(Token{TokenTypes::Num, "2"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Declare bool variable and assign to: false", "[Lexer]") {
    std::string expr = "var a = false";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::DeclareId, "var"});
    properTokens.emplace_back(Token{TokenTypes::Id, "a"});
    properTokens.emplace_back(Token{TokenTypes::Assign, "="});
    properTokens.emplace_back(Token{TokenTypes::Bool, "0"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Declare bool variable and assign to: true", "[Lexer]") {
    std::string expr = "var a = true";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::DeclareId, "var"});
    properTokens.emplace_back(Token{TokenTypes::Id, "a"});
    properTokens.emplace_back(Token{TokenTypes::Assign, "="});
    properTokens.emplace_back(Token{TokenTypes::Bool, "1"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Expression with using of bool values and logical operators", "[Lexer]") {
    std::string expr = "true || false && false";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Bool, "1"});
    properTokens.emplace_back(Token{TokenTypes::BoolOR, "||"});
    properTokens.emplace_back(Token{TokenTypes::Bool, "0"});
    properTokens.emplace_back(Token{TokenTypes::BoolAND, "&&"});
    properTokens.emplace_back(Token{TokenTypes::Bool, "0"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Tokenize single bool value: true", "[Lexer]") {
    std::string expr = "true";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Bool, "1"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Tokenize single bool value: false", "[Lexer]") {
    std::string expr = "false";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Bool, "0"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Expression with using of variables of type bool and logical operators: Logical AND", "[Lexer]") {
    std::string expr = "a && false";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Id, "a"});
    properTokens.emplace_back(Token{TokenTypes::BoolAND, "&&"});
    properTokens.emplace_back(Token{TokenTypes::Bool, "0"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Expression with using of variables of type bool and logical operators: Logical OR", "[Lexer]") {
    std::string expr = "a || false";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Id, "a"});
    properTokens.emplace_back(Token{TokenTypes::BoolOR, "||"});
    properTokens.emplace_back(Token{TokenTypes::Bool, "0"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Id declare & assign math expression tokenizing", "[Lexer]") {
    std::string expr = "var a = 5 * 10 / 25";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::DeclareId, "var"});
    properTokens.emplace_back(Token{TokenTypes::Id, "a"});
    properTokens.emplace_back(Token{TokenTypes::Assign, "="});
    properTokens.emplace_back(Token{TokenTypes::Num, "5"});
    properTokens.emplace_back(Token{TokenTypes::Mul, "*"});
    properTokens.emplace_back(Token{TokenTypes::Num, "10"});
    properTokens.emplace_back(Token{TokenTypes::Div, "/"});
    properTokens.emplace_back(Token{TokenTypes::Num, "25"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Id declare & assign bool expression tokenizing", "[Lexer]") {
    std::string expr = "var a = true || false";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::DeclareId, "var"});
    properTokens.emplace_back(Token{TokenTypes::Id, "a"});
    properTokens.emplace_back(Token{TokenTypes::Assign, "="});
    properTokens.emplace_back(Token{TokenTypes::Bool, "1"});
    properTokens.emplace_back(Token{TokenTypes::BoolOR, "||"});
    properTokens.emplace_back(Token{TokenTypes::Bool, "0"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Expression with using of equal operator and bool constants", "[Lexer]") {
    std::string expr = "true == false";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Bool, "1"});
    properTokens.emplace_back(Token{TokenTypes::Equal, "=="});
    properTokens.emplace_back(Token{TokenTypes::Bool, "0"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}

TEST_CASE("Expression with using of equal operator and variables", "[Lexer]") {
    std::string expr = "a == 2";
    expr.push_back(EOF);

    const TokenContainer& data = LexerTestsLexer.tokenize(expr);
    const std::vector<Token>& tokens = data.getTokens();

    std::vector<Token> properTokens;
    properTokens.emplace_back(Token{TokenTypes::Id, "a"});
    properTokens.emplace_back(Token{TokenTypes::Equal, "=="});
    properTokens.emplace_back(Token{TokenTypes::Num, "2"});
    properTokens.emplace_back(Token{TokenTypes::eof, "EOF"});

    matchTokens(tokens, properTokens);
}