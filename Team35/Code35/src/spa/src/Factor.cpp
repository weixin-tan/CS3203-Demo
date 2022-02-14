#include "Factor.h"
#include "Expr.h"

Factor::Factor() {

}

Type Factor::getType() const {
	return this->type;
}

Token Factor::getVarName() const {
	return this->varName;
}

Token Factor::getConstValue() const {
	return this->constValue;
}

Expr Factor::getExpr() const {
	return this->*expr;
}

void Factor::setType(Type type) {
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