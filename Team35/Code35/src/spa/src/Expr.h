#include "Term.h"
#include "TokenType.h"

#ifndef EXPR_H
#define EXPR_H

class Expr {
private:
	TokenType tokenType;
	Expr *expr;
	Term term;
public:
	Expr();
	TokenType getOperator() const;
	Expr getExpr() const;
	Term getTerm() const;
	void setOperator(TokenType tokenType);
	void setExpr(Expr expr);
	void setTerm(Term term);
};

#endif