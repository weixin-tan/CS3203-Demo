#include "Expr.h"

Expr::Expr() {

}

Operator Expr::getOperator() const {
	return this->op;
}

Expr Expr::getExpr() const {
	return *expr;
}

Term Expr::getTerm() const {
	return this->term;
}

void Expr::setOperator(Operator op) {
	this->op = op;
}

void Expr::setExpr(Expr expr) {
	this->expr = &expr;
}

void Expr::setTerm(Term term) {
	this->term = term;
}