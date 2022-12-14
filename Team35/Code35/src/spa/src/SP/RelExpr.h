#ifndef RELEXPR_H
#define RELEXPR_H

#include "RelFactor.h"
#include "TokenType.h"

class RelExpr {
private:
    TokenType tokenType;
    RelFactor relFactorOne;
    RelFactor relFactorTwo;
public:
    RelExpr();
    TokenType getOperator() const;
    RelFactor getRelFactorOne() const;
    RelFactor getRelFactorTwo() const;
    void setOperator(TokenType tokenType);
    void setRelFactorOne(RelFactor relFactorOne);
    void setRelFactorTwo(RelFactor relFactorTwo);
};

#endif