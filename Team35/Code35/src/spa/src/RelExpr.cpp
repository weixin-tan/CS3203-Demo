#include "RelExpr.h"

RelExpr::RelExpr() {

}

TokenType RelExpr::getOperator() const {
	return this->tokenType;
}

RelFactor RelExpr::getRelFactorOne() const {
	return this->relFactorOne;
}

RelFactor RelExpr::getRelFactorTwo() const {
	return this->relFactorTwo;
}

void RelExpr::setOperator(TokenType tokenType) {
	this->tokenType = tokenType;
}

void RelExpr::setRelFactorOne(RelFactor relFactorOne) {
	this->relFactorOne = relFactorOne;
}

void RelExpr::setRelFactorTwo(RelFactor relFactorTwo) {
	this->relFactorTwo = relFactorTwo;
}