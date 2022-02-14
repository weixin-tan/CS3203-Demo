#include "RelExpr.h"
#include "Operator.h"

#ifndef CONDEXPR_H
#define CONDEXPR_H

class CondExpr {
private:
	Operator op;
	CondExpr* condExpr;
	RelExpr relExpr;
public:
	CondExpr();
	Operator getOperator() const;
	CondExpr getCondExpr() const;
	RelExpr getRelExpr() const;
	void setOperator(Operator op);
	void setCondExpr(CondExpr condExpr);
	void setRelExpr(RelExpr relExpr);
};

#endif