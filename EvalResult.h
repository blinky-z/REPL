#ifndef REPL_EVALRESULT_H
#define REPL_EVALRESULT_H

#include "Identifier.h"
#include <unordered_map>

class EvalError {
private:
    std::string message;
public:
    enum Error {
        null,
        INCOMPATIBLE_OPERANDS_TYPE,
        INVALID_AST,
        UNDECLARED_VAR,
        INVALID_LVALUE,
        UNINITIALIZED_VAR,
        INVALID_BIN_OPERATION,
        INVALID_VALUE_TYPE,
        VAR_REDEFINITION
    };

    const std::string& what();

    Error errorCode;

    EvalError(Error errCode, std::string errMessage = "");

    EvalError() {
        errorCode = Error::null;
    };
};

struct EvalResult {
private:
    bool resultBool;
    double resultDouble;
    std::string resultString;
    IdentifierValueType::ValueType resultType;
public:
    bool operator==(const EvalResult& other) {
        if (this->resultType == IdentifierValueType::Number) {
            return this->resultDouble == other.resultDouble;
        } else {
            return this->resultBool == other.resultBool;
        }
    }

    EvalError error;

    bool isError() const;

    IdentifierValueType::ValueType getResultType() const;

    double getResultDouble() const;

    bool getResultBool() const;

    std::string getResultString() const;

    void setValueDouble(double value);

    void setValueBool(bool value);

    void setValueString(const std::string value);

    EvalResult() {
        resultType = IdentifierValueType::Undefined;
    }
};

#endif //REPL_EVALRESULT_H