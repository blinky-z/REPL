#ifndef REPL_EVALRESULT_H
#define REPL_EVALRESULT_H

#include "Identifier.h"
#include <unordered_map>

namespace EvalErrorCode {
    enum Error {
        SUCCESS,
        INCOMPATIBLE_OPERANDS_TYPE,
        INVALID_AST,
        UNDECLARED_VAR,
        UNDECLARED_VAR_LVALUE,
        UNINITIALIZED_VAR,
        INVALiD_BIN_OPERATION,
        INVALID_VALUE_TYPE,
        VAR_DEREFINITION
    };
}

class EvalError {
private:
    std::string message;
public:
    const std::string& what();

    EvalErrorCode::Error errorCode;

    EvalError(EvalErrorCode::Error err, std::string errMessage = "");

    EvalError() {
        errorCode = EvalErrorCode::SUCCESS;
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
