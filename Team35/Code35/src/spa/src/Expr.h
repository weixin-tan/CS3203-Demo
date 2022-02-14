#include "Term.h"
#include "Operator.h"

#ifndef EXPR_H
#define EXPR_H

class Expr {
private:
	Operator op;
	Expr *expr;
	Term term;
public:
	Expr();
	Operator getOperator() const;
	Expr getExpr() const;
	Term getTerm() const;
	void setOperator(Operator op);
	void setExpr(Expr expr);
	void setTerm(Term term);
};

#endif