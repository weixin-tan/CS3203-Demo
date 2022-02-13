#include "RelFactor.h"

#ifndef RELEXPR_H
#define RELEXPR_H

enum Operator{GREATER, GEQ, LESSER, LEQ, EQUAL, NOT_EQUAL};

class RelExpr {
private:
	Operator operator;
	RelFactor relFactorOne;
	RelFactor relFactorTwo;
public:
	RelExpr();
	Operator getOperator() const;
	RelFactor getRelFactorOne() const;
	RelFactor getRelFactorTwo() const;
	void setOperator(Operator operator);
	void setRelFactorOne(RelFactor relFactorOne);
	void setRelFactorTwo(RelFactor relFactorTwo);
};

#endif