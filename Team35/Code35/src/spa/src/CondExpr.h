#include "RelExpr.h"
#include "TokenType.h"

#ifndef CONDEXPR_H
#define CONDEXPR_H

class CondExpr {
private:
	TokenType tokenType;
	CondExpr* condExpr;
	RelExpr relExpr;
public:
	CondExpr();
	TokenType getOperator() const;
	CondExpr getCondExpr() const;
	RelExpr getRelExpr() const;
	void setOperator(TokenType tokenType);
	void setCondExpr(CondExpr condExpr);
	void setRelExpr(RelExpr relExpr);
};

#endif