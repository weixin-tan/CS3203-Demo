#include "CondExpr.h"

CondExpr::CondExpr() {

}

Operator CondExpr::getOperator() const {
	return this->operator;
}

CondExpr CondExpr::getCondExpr() const {
	return this->condExpr;
}

RelExpr CondExpr::getRelExpr() const {
	return this->relExpr;
}

void CondExpr::setOperator(Operator operator) {
	this->operator = operator;
}

void CondExpr::setCondExpr(CondExpr condExpr) {
	this->condExpr = condExpr;
}

void CondExpr::setRelExpr(RelExpr relExpr) {
	this->relExpr = relExpr;
}