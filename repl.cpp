#include <iostream>
#include "Lexer.h"
#include "Parser.h"
#include "Evaluator.h"
#include "EvalResult.h"
#include "SemanticAnalyzer.h"
#include "SemanticAnalysisResult.h"

bool isInputForLoop(const std::string& input) {
    return input.find("for") != std::string::npos;
}

bool isInputIfStmt(const std::string& input) {
    return input.find("if") != std::string::npos || input.find("else") != std::string::npos;
}

bool isInputFuncDecl(const std::string& input) {
    return input.find("func") != std::string::npos;
}

long compoundCnt;

void countOpenBrackets(const std::string stmt) {
    for (const auto& currentCh : stmt) {
        if (currentCh == '{') {
            compoundCnt++;
        }
    }
}

void countEndBrackets(const std::string stmt) {
    for (const auto& currentCh : stmt) {
        if (currentCh == '}') {
            compoundCnt--;
        }
    }
}

std::string readCompoundStatement(const std::string& input) {
    std::string stmt = input;

    std::string currentInput;
    while (true) {
        if (compoundCnt == 0) {
            break;
        }

        getline(std::cin, currentInput);
        if (std::cin.eof()) {
            exit(EXIT_SUCCESS);
        }
        countOpenBrackets(currentInput);
        countEndBrackets(currentInput);

        if (isInputIfStmt(currentInput) || isInputForLoop(currentInput) || isInputFuncDecl(currentInput)) {
            stmt += readCompoundStatement(currentInput);
        } else if (currentInput.empty()) {
            continue;
        } else {
            if (compoundCnt) {
                currentInput.push_back('\n');
            }
            stmt += currentInput;
        }
    }

    return stmt;
}

void printResult(const EvalResult& result, int tabCount = 0) {
    ValueType::Type resultType = result.getResultType();

    if (resultType == ValueType::Number) {
        std::cout << result.getResultDouble() << std::endl;
    } else if (resultType == ValueType::Bool) {
        std::cout << (result.getResultBool() ? "true" : "false") << std::endl;
    } else if (resultType == ValueType::Compound) {
        for (const auto& currentResult : result.getResultBlock()) {
            printResult(currentResult, tabCount);
        }
    } else if (resultType == ValueType::String) {
        std::cout << result.getResultString() << std::endl;
    }
}

int main() {
    Lexer lexer;
    Parser parser;
    SemanticAnalyzer semanticAnalyzer(0);
    Evaluator evaluator;

    std::vector<ProgramTranslationNode*> nodes;

    while (true) {
        std::string input;
        getline(std::cin, input);

        if (std::cin.eof()) {
            break;
        }

        if (input.size() != 0) {
            if (isInputIfStmt(input) || isInputForLoop(input) || isInputFuncDecl(input)) {
                countOpenBrackets(input);
                countEndBrackets(input);
                input = readCompoundStatement(input);
            }
            input.push_back('\n');
            input.push_back(EOF);

            const TokenContainer& tokens = lexer.tokenize(input);
            ProgramTranslationNode* root = parser.parse(tokens);
            SemanticAnalysisResult checkResult = semanticAnalyzer.checkProgram(root);
            if (checkResult.isError()) {
                std::cerr << checkResult.what() << std::endl;
            } else {
                EvalResult result = evaluator.Evaluate(root->statements[0]);
                printResult(result);
            }

            nodes.emplace_back(root);
        }
    }

    for (const auto& currentNode : nodes) {
        delete currentNode;
    }

    return 0;
}