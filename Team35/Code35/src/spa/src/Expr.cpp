#include "Expr.h"

// There are a few ways to do this, but the one that would disrupt the least amount
// of code would be to just overwrite the basic expr constructor to set all
// variables to null/nullptr

//TODO: Refactor the code to call a new constructor that defaults everything to
//null or nullptr. 

void setOperator(TokenType);
void setExpr(Expr);
void setTerm(Term);

Expr::Expr() {
	this->termFlag = false; 
	this->exprFlag = false;
}

TokenType Expr::getOperator() const {
	return this->tokenType;
}

Expr Expr::getExpr() const {
	return *expr;
}

Term Expr::getTerm() const {
	return this->term;
}

bool Expr::hasExpr() const {
	return exprFlag; 
}

bool Expr::hasTerm() const {
	return termFlag; 
}

void Expr::setOperator(TokenType tokenType) {
	this->tokenType = tokenType;
}

void Expr::setExpr(Expr expr) {
	this->expr = &expr;
	this->exprFlag = true;
}

void Expr::setTerm(Term term) {
	this->term = term;
	this->termFlag = true; 
}