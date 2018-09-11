#include "EvalResult.h"

bool EvalResult::getResultBool() const {
    return resultBool;
}

double EvalResult::getResultDouble() const {
    return resultDouble;
}

void EvalResult::setValueDouble(double value) {
    resultType = IdentifierValueType::Number;
    resultDouble = value;
}

void EvalResult::setValueBool(bool value) {
    resultType = IdentifierValueType::Bool;
    resultBool = value;
}

IdentifierValueType::ValueType EvalResult::getResultType() const {
    return resultType;
}

std::string EvalResult::getResultString() const {
    return resultString;
}

void EvalResult::setValueString(const std::string value) {
    resultType = IdentifierValueType::String;
    resultString = value;
}

bool EvalResult::isError() const {
    return error.errorCode != EvalError::null;
}

const std::string& EvalError::what() {
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
