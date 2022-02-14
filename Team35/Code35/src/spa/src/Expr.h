#include "Term.h"
#include "Operator.h"

#ifndef EXPR_H
#define EXPR_H

class Expr {
private:
	Operator operator;
	Expr expr;
	Term term;
public:
	Expr();
	Operator getOperator() const;
	Expr getExpr() const;
	Term getTerm() const;
	void setOperator(Operator operator);
	void setExpr(Expr expr);
	void setTerm(Term term);
};

#endif