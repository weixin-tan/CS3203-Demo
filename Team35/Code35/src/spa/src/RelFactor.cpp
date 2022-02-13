#include "RelFactor.h"

RelFactor::RelFactor() {

}

Type RelFactor::getType() const {
	return this->type;
}

Token RelFactor::getVarName() const {
	return this->varName;
}

Token RelFactor::getConstValue() const {
	return this->constValue;
}

Expr RelFactor::getExpr() const {
	return this->expr;
}

void RelFactor::setType(Type type) {
	this->type = type;
}

void RelFactor::setVarName(Token varName) {
	this->varName = varName;
}

void RelFactor::setConstValue(Token constValue) {
	this->constValue = constValue;
}

void RelFactor::setExpr(Expr expr) {
	this->expr = expr;
}