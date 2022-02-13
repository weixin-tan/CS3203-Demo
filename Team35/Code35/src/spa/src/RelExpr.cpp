#include "RelExpr.h"

RelExpr::RelExpr() {

}

Operator RelExpr::getOperator() const {
	return this->operator;
}

RelFactor RelExpr::getRelFactorOne() const {
	return this->relFactorOne;
}

RelFactor RelExpr::getRelFactorTwo() const {
	return this->relFactorTwo;
}

void RelExpr::setOperator(Operator operator) {
	this->operator = operator;
}

void RelExpr::setRelFactorOne(RelFactor relFactorOne) {
	this->relFactorOne = relFactorOne;
}

void RelExpr::setRelFactorTwo(RelFactor relFactorTwo) {
	this->relFactorTwo = relFactorTwo;
}