#ifndef REPL_COMPILER_H
#define REPL_COMPILER_H

#include "ASTNode.h"
#include <string>
#include <cmath>

class BashGenerator {
private:
    std::string generateConstNumber(ConstNumberNode* node);

    std::string generateConstBool(ConstBoolNode* node);

    std::string generateId(IdentifierNode* node);

    std::string generateDeclVar(DeclVarNode* node);

    std::string generateMathExpr(BinOpNode* node);

    std::string generateBinaryExpr(BinOpNode* node);

    std::string generateStatement(ASTNode* node);
public:
    std::string generate(ProgramTranslationNode* root);
};


#endif //REPL_COMPILER_H
