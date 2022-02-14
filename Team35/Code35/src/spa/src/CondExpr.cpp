#include "CondExpr.h"

CondExpr::CondExpr() {

}

Operator CondExpr::getOperator() const {
	return this->op;
}

CondExpr CondExpr::getCondExpr() const {
	return *condExpr;
}

RelExpr CondExpr::getRelExpr() const {
	return this->relExpr;
}

void CondExpr::setOperator(Operator op) {
	this->op = op;
}

void CondExpr::setCondExpr(CondExpr condExpr) {
	this->condExpr = &condExpr;
}

void CondExpr::setRelExpr(RelExpr relExpr) {
	this->relExpr = relExpr;
}