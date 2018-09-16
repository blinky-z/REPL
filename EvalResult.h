#ifndef REPL_EVALRESULT_H
#define REPL_EVALRESULT_H

#include "Identifier.h"
#include <unordered_map>
#include <vector>

class EvalError {
private:
    std::string message;
public:
    enum Error {
        null,
        INCOMPATIBLE_OPERAND_TYPES,
        INVALID_AST,
        UNDECLARED_VAR,
        INVALID_LVALUE,
        UNINITIALIZED_VAR,
        INVALID_OPERATION,
        INVALID_VALUE_TYPE,
        VAR_REDEFINITION
    };

    const std::string& what() const;

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

    ValueType::Type resultType;

    std::vector<EvalResult> resultBlock;
public:
    EvalError error;

    bool isError() const;

    ValueType::Type getResultType() const;

    double getResultDouble() const;

    bool getResultBool() const;

    std::string getResultString() const;

    std::vector<EvalResult> getResultBlock() const;

    void setValueDouble(double value);

    void setValueBool(bool value);

    void setValueString(const std::string value);

    void setBlockResult(const std::vector<EvalResult> results);

    EvalResult() {
        resultType = ValueType::Undefined;
    }
};

#endif //REPL_EVALRESULT_H