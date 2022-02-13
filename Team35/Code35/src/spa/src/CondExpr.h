#include "RelExpr.h"

#ifndef CONDEXPR_H
#define CONDEXPR_H

enum Operator{AND, OR};

class CondExpr {
private:
	Operator operator;
	CondExpr condExpr;
	RelExpr relExpr;
public:
	CondExpr();
	Operator getOperator() const;
	CondExpr getCondExpr() const;
	RelExpr getRelExpr() const;
	void setOperator(Operator operator);
	void setCondExpr(CondExpr condExpr);
	void setRelExpr(RelExpr relExpr);
};

#endif