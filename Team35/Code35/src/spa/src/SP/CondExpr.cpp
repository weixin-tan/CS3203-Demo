#include "CondExpr.h"

CondExpr::CondExpr() {

}

TokenType CondExpr::getOperator() const {
    return this->tokenType;
}

CondExpr CondExpr::getCondExpr() const {
    return *condExpr;
}

RelExpr CondExpr::getRelExpr() const {
    return this->relExpr;
}

void CondExpr::setOperator(TokenType tokenType) {
    this->tokenType = tokenType;
}

void CondExpr::setCondExpr(CondExpr condExpr) {
    this->condExpr = &condExpr;
}

void CondExpr::setRelExpr(RelExpr relExpr) {
    this->relExpr = relExpr;
}