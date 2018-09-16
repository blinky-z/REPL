#include "EvalResult.h"

ValueType::Type EvalResult::getResultType() const {
    return resultType;
}

double EvalResult::getResultDouble() const {
    return resultDouble;
}

bool EvalResult::getResultBool() const {
    return resultBool;
}

std::string EvalResult::getResultString() const {
    return resultString;
}

std::vector<EvalResult> EvalResult::getResultBlock() const {
    return resultBlock;
}

void EvalResult::setValueDouble(double value) {
    resultType = ValueType::Number;
    resultDouble = value;
}

void EvalResult::setValueBool(bool value) {
    resultType = ValueType::Bool;
    resultBool = value;
}

void EvalResult::setValueString(const std::string value) {
    resultType = ValueType::String;
    resultString = value;
}

void EvalResult::setBlockResult(const std::vector<EvalResult> results) {
    resultType = ValueType::Compound;
    resultBlock = results;
}

bool EvalResult::isError() const {
    return error.errorCode != EvalError::null;
}

const std::string& EvalError::what() const {
    static std::unordered_map<Error, std::string> errorMessage;

    errorMessage[INVALID_AST] = "Invalid AST";
    errorMessage[INCOMPATIBLE_OPERAND_TYPES] = "Incompatible Operand Types";
    errorMessage[UNDECLARED_VAR] = "Use of undeclared variable";
    errorMessage[UNINITIALIZED_VAR] = "Use of uninitialized variable";
    errorMessage[INVALID_LVALUE] = "Invalid Lvalue";
    errorMessage[INVALID_OPERATION] = "Invalid Operation";
    errorMessage[INVALID_VALUE_TYPE] = "Invalid value type";
    errorMessage[VAR_REDEFINITION] = "Variable Redefinition";
    errorMessage[null] = "No Error";

    if (message.empty()) {
        return errorMessage[errorCode];
    }
    return message;
}

EvalError::EvalError(Error errCode, std::string errMessage) {
    errorCode = errCode;
    message = errMessage;
}
