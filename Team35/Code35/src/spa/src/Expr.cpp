#include "Expr.h"

Expr::Expr() {

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

void Expr::setOperator(TokenType tokenType) {
	this->tokenType = tokenType;
}

void Expr::setExpr(Expr expr) {
	this->expr = &expr;
}

void Expr::setTerm(Term term) {
	this->term = term;
}