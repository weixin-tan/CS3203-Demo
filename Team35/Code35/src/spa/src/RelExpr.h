#include "RelFactor.h"
#include "Operator.h"

#ifndef RELEXPR_H
#define RELEXPR_H

class RelExpr {
private:
	Operator op;
	RelFactor relFactorOne;
	RelFactor relFactorTwo;
public:
	RelExpr();
	Operator getOperator() const;
	RelFactor getRelFactorOne() const;
	RelFactor getRelFactorTwo() const;
	void setOperator(Operator op);
	void setRelFactorOne(RelFactor relFactorOne);
	void setRelFactorTwo(RelFactor relFactorTwo);
};

#endif