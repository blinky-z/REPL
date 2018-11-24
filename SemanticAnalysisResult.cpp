#include "SemanticAnalysisResult.h"

SemanticAnalysisResult::SemanticAnalysisResult(SemanticAnalysisResult::Error errCode, std::string errMessage) {
    errorCode = errCode;
    message = errMessage;
}

const std::string& SemanticAnalysisResult::what() const {
    static std::map<Error, std::string> errorMessage;

    errorMessage[INVALID_AST] = "Invalid AST";
    errorMessage[INCOMPATIBLE_OPERAND_TYPES] = "Incompatible Operand Types";
    errorMessage[UNDECLARED_VAR] = "Use of undeclared variable";
    errorMessage[UNINITIALIZED_VAR] = "Use of uninitialized variable";
    errorMessage[INVALID_LVALUE] = "Invalid Lvalue";
    errorMessage[INVALID_OPERATION] = "Invalid Operation";
    errorMessage[INVALID_VALUE_TYPE] = "Invalid value type";
    errorMessage[VAR_REDEFINITION] = "Variable Redefinition";
    errorMessage[FUNC_REDEFINITION] = "Function Redefinition";
    errorMessage[FUNC_DEFINITION_IS_NOT_ALLOWED] = "Function definition is not allowed here";
    errorMessage[UNDECLARED_FUNC] = "Use of undeclared function";
    errorMessage[NO_MATCHING_FUNC] = "No matching function to call";
    errorMessage[RETURN_TYPE_MISMATCH] = "Lack of or invalid return statement";
    errorMessage[null] = "No Error";

    if (message.empty()) {
        return errorMessage[errorCode];
    }
    return message;
}

bool SemanticAnalysisResult::isError() const {
    return errorCode != Error::null;
}
