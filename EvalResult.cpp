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
    return error.errorCode != EvalErrorCode::SUCCESS;
}

const std::string& EvalError::what() {
    if (message.empty()) {
        static std::unordered_map<EvalErrorCode::Error, std::string> errorMessage;

        errorMessage[EvalErrorCode::INVALID_AST] = "Invalid AST";
        errorMessage[EvalErrorCode::INCOMPATIBLE_OPERANDS_TYPE] = "Incompatible Operands Type";
        errorMessage[EvalErrorCode::UNDECLARED_VAR] = "Use of undeclared variable";
        errorMessage[EvalErrorCode::UNINITIALIZED_VAR] = "Use of uninitialized variable";
    }
    return message;
}

EvalError::EvalError(EvalErrorCode::Error err, std::string errMessage) {
    errorCode = err;
    message = errMessage;
}
