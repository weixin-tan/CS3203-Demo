#include "Expr.h"

Expr::Expr() {

}

Operator Expr::getOperator() const {
	return this->operator;
}

Expr Expr::getExpr() const {
	return this->expr;
}

Term Expr::getTerm() const {
	return this->term;
}

void Expr::setOperator(Operator operator) {
	this->operator = operator;
}

void Expr::setExpr(Expr expr) {
	this->expr = expr;
}

void Expr::setTerm(Term term) {
	this->term = term;
}