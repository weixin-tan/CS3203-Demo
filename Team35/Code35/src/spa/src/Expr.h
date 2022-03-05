#include "Term.h"
#include "TokenType.h"

#ifndef EXPR_H
#define EXPR_H

class Expr {
private:
	TokenType tokenType;
	Expr *expr;
	Term *term;
	bool termFlag; 
	bool exprFlag;
public:
	Expr();
	TokenType getOperator() const;
	Expr getExpr() const;
	Term getTerm() const;
	Expr* getExprPtr() const;
	bool hasExpr() const;
	Term* getTermPtr() const;
	bool hasTerm() const;
	void setOperator(TokenType tokenType);
	void setExpr(Expr expr);
	void setTerm(Term term);
};

#endif