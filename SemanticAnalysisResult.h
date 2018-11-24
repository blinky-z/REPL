#ifndef REPL_SEMANTICANALYSIS_H
#define REPL_SEMANTICANALYSIS_H

#include <string>
#include <map>

class SemanticAnalysisResult {
public:
    enum Error {
        null,
        INCOMPATIBLE_OPERAND_TYPES,
        INVALID_AST,
        UNDECLARED_VAR,
        UNINITIALIZED_VAR,
        INVALID_LVALUE,
        INVALID_OPERATION,
        INVALID_VALUE_TYPE,
        VAR_REDEFINITION,
        FUNC_REDEFINITION,
        FUNC_DEFINITION_IS_NOT_ALLOWED,
        UNDECLARED_FUNC,
        NO_MATCHING_FUNC,
        RETURN_TYPE_MISMATCH,
    };
private:
    std::string message;

public:
    Error errorCode;

    bool isError() const;

    const std::string& what() const;

    SemanticAnalysisResult(Error errCode, std::string errMessage = "");

    SemanticAnalysisResult() {
        errorCode = Error::null;
    };
};

#endif //REPL_SEMANTICANALYSIS_H
