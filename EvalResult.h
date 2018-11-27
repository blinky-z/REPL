#ifndef REPL_EVALRESULT_H
#define REPL_EVALRESULT_H

#include "Identifier.h"
#include <map>
#include <vector>

struct EvalResult {
private:
    bool resultBool;

    double resultDouble;

    std::string resultString;

    ValueType::Type resultType;

    std::vector<EvalResult> resultBlock;
public:
    ValueType::Type getResultType() const;

    double getResultDouble() const;

    bool getResultBool() const;

    std::string getResultString() const;

    std::vector<EvalResult> getResultBlock() const;

    void setValueDouble(double value);

    void setValueBool(bool value);

    void setValueString(const std::string value);

    void setBlockResult(const std::vector<EvalResult> results);\

    void setVoidResult();

    EvalResult() {
        resultType = ValueType::Undefined;
    }
};

#endif //REPL_EVALRESULT_H