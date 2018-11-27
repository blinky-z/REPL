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

void EvalResult::setVoidResult() {
    resultType = ValueType::Void;
}