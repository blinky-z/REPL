#include <fstream>
#include <sstream>
#include "catch.hpp"
#include "../Lexer.h"
#include "../Token.h"
#include "../Identifier.h"
#include "../Parser.h"
#include "../ASTNode.h"
#include "../TokenContainer.h"
#include "../SemanticAnalyzer.h"
#include "../SemanticAnalysisResult.h"
#include "../BashGenerator.h"

std::string readInput(const std::string fileName) {
    const std::string delimiter = "\n----\n";
    std::ifstream ifs(fileName);
    std::stringstream ss;
    ss << ifs.rdbuf();
    ifs.close();
    const std::string& input = ss.str();
    return input.substr(0, input.find(delimiter));
}

std::string readOutput(const std::string fileName) {
    const std::string delimiter = "\n----\n";
    std::ifstream ifs(fileName);
    std::stringstream ss;
    ss << ifs.rdbuf();
    ifs.close();
    const std::string& input = ss.str();
    unsigned long delimiterPos = input.find(delimiter);
    return input.substr(delimiterPos + delimiter.size(), input.size() - delimiterPos - delimiter.size());
}

std::string replaceUuid(const std::string& word, const std::string& properWord) {
    std::string result;
    for (unsigned long currentCharNum = 0; currentCharNum < word.size(); currentCharNum++) {
        if (word[currentCharNum] != '_') {
            result += word[currentCharNum];
        } else {
            for (int i = 0; i < 33; i++) {
                result += properWord[currentCharNum + i];
            }
            currentCharNum += 32;
        }
    }
    return result;
}

void matchResult(const std::string& result, const std::string& properResult) {
    std::vector<std::string> resultWords;
    std::vector<std::string> properResultWords;

    std::stringstream resultStream(result);
    std::stringstream properResultStream(properResult);

    std::string word;
    std::string properWord;
    while (resultStream >> word) {
        properResultStream >> properWord;
        if ((word.find('_') != word.npos) && word.size() >= 33 && word.size() == properWord.size()) {
            word = replaceUuid(word, properWord);
        }
        resultWords.emplace_back(word);
        properResultWords.emplace_back(properWord);
    }

    REQUIRE(resultWords.size() == properResultWords.size());
    for (unsigned long currentWord = 0; currentWord < properResultWords.size(); currentWord++) {
        REQUIRE(resultWords[currentWord] == properResultWords[currentWord]);
    }
}

class ExpressionHandler {
private:
    static Lexer lexer;

    static Parser parser;

    SemanticAnalyzer semanticAnalyzer = SemanticAnalyzer(1);

    static BashGenerator bashGenerator;
public:
    void handleExpression(const std::string& fileName) {
        std::string input = readInput(fileName);
        input.push_back('\n');
        input.push_back(EOF);

        const TokenContainer& tokens = lexer.tokenize(input);
        ProgramTranslationNode* root = parser.parse(tokens);
        SemanticAnalysisResult checkResult = semanticAnalyzer.checkProgram(root);
        if (checkResult.isError()) {
            throw std::runtime_error(checkResult.what());
        }
        const std::string& bashCode = bashGenerator.generate(root);

        delete root;

        const std::string& properResult = readOutput(fileName);
        matchResult(bashCode, properResult);
    }
};

Lexer ExpressionHandler::lexer = Lexer();
Parser ExpressionHandler::parser = Parser();
BashGenerator ExpressionHandler::bashGenerator = BashGenerator();

TEST_CASE("Variable Declaration", "[Bash Generator]") {
    ExpressionHandler expressionHandler;
    expressionHandler.handleExpression("./LanguageSamples/DeclAssignVar");
}

TEST_CASE("Function Call", "[Bash Generator]") {
    ExpressionHandler expressionHandler;
    expressionHandler.handleExpression("./LanguageSamples/FuncCalls");
}

TEST_CASE("If Statements", "[Bash Generator]") {
    ExpressionHandler expressionHandler;
    expressionHandler.handleExpression("./LanguageSamples/IfStatements");
}

TEST_CASE("For Loop Statements", "[Bash Generator]") {
    ExpressionHandler expressionHandler;
    expressionHandler.handleExpression("./LanguageSamples/ForLoopStatements");
}

TEST_CASE("Complain Test", "[Bash Generator]") {
    ExpressionHandler expressionHandler;
    expressionHandler.handleExpression("./LanguageSamples/ComplainTest");
}