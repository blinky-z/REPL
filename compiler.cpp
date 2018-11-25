#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexer.h"
#include "Parser.h"
#include "SemanticAnalyzer.h"
#include "BashGenerator.h"

std::string readProgram(const std::string fileName) {
    std::string input;
    std::stringstream ss;
    std::ifstream ifs(fileName);
    ss << ifs.rdbuf();
    input = ss.str();
    ifs.close();
    return input;
}

int main(int argc, char** argv) {
    Lexer lexer;
    Parser parser;
    SemanticAnalyzer semanticAnalyzer;
    BashGenerator bashGenerator;

    std::string source = readProgram(argv[1]);
    source.push_back('\n');
    source.push_back(EOF);

    const TokenContainer& tokens = lexer.tokenize(source);
    ProgramTranslationNode* ast = parser.parse(tokens);
    const SemanticAnalysisResult& checkResult = semanticAnalyzer.checkProgram(ast);
    if (checkResult.isError()) {
        throw std::runtime_error(checkResult.what().c_str());
    }
    std::string bashCode = bashGenerator.generate(ast);

    delete ast;

    std::ofstream outFile("bash_program.sh");
    outFile << bashCode;
    outFile.close();

    return 0;
}