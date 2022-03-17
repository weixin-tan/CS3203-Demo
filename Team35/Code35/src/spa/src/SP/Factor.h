#include <memory>
#include "Token.h"
#include "FactorType.h"

#ifndef FACTOR_H
#define FACTOR_H

class Expr;
class Factor {
private:
    FactorType type;
    Token varName;
    Token constValue;
    std::shared_ptr<Expr> expr;
    bool exprFlag;
public:
    Factor();
    FactorType getType() const;
    Token getVarName() const;
    Token getConstValue() const;
    std::shared_ptr<Expr> getExpr() const;
    bool hasExpr() const;
    void setType(FactorType type);
    void setVarName(Token varName);
    void setConstValue(Token constValue);
    void setExpr(std::shared_ptr<Expr> expr);
};

#endif