#include "Factor.h"

Factor::Factor() {

}

FactorType Factor::getType() const {
	return this->type;
}

Token Factor::getVarName() const {
	return this->varName;
}

Token Factor::getConstValue() const {
	return this->constValue;
}

Expr Factor::getExpr() const {
	return *expr;
}

void Factor::setType(FactorType type) {
	this->type = type;
}

void Factor::setVarName(Token varName) {
	this->varName = varName;
}

void Factor::setConstValue(Token constValue) {
	this->constValue = constValue;
}

void Factor::setExpr(Expr expr) {
	this->expr = &expr;
}