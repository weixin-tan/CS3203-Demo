#include "Factor.h"

Factor::Factor() {
    this->exprFlag = false;
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

std::shared_ptr<Expr> Factor::getExpr() const {
    if (!hasExpr()) {
        return nullptr;
    }
    return expr;
}

bool Factor::hasExpr() const {
    return this->exprFlag;
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

void Factor::setExpr(std::shared_ptr<Expr> expr) {
    this->expr = expr;
    this->exprFlag = true;
}