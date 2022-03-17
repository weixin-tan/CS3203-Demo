#ifndef RELFACTOR_H
#define RELFACTOR_H

#include "Expr.h"
#include "FactorType.h"

class RelFactor {
private:
    FactorType type;
    Token varName;
    Token constValue;
    Expr expr;
public:
    RelFactor();
    FactorType getType() const;
    Token getVarName() const;
    Token getConstValue() const;
    Expr getExpr() const;
    void setType(FactorType type);
    void setVarName(Token varName);
    void setConstValue(Token constValue);
    void setExpr(Expr expr);
};

#endif