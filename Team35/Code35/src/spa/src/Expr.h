#include <memory>
#include "Term.h"
#include "TokenType.h"

#ifndef EXPR_H
#define EXPR_H

class Expr {
private:
	TokenType tokenType;
	std::shared_ptr<Expr> expr;
	Term term;
	bool termFlag; 
	bool exprFlag;
public:
	Expr();
	bool isNullExpr() const;
	TokenType getOperator() const;
	bool hasOperator() const;
	std::shared_ptr<Expr> getExpr() const;
	Term getTerm() const;
	Expr* getExprPtr() const;
	bool hasExpr() const;
	Term* getTermPtr();
	bool hasTerm() const;
	void setOperator(TokenType tokenType);
	void setExpr(std::shared_ptr<Expr> expr);
	void setTerm(Term term);
};

#endif