#include "Expr.h"

// There are a few ways to do this, but the one that would disrupt the least amount
// of code would be to just overwrite the basic expr constructor to set all
// variables to null/nullptr

//TODO: Refactor the code to call a new constructor that defaults everything to
//null or nullptr. 

void setOperator(TokenType);
void setExpr(Expr);
void setTerm(Term);

Expr::Expr() {
	this->termFlag = false; 
	this->exprFlag = false;
	this->tokenType = TokenType::NONE;
}

bool Expr::hasOperator() const {
	return this->tokenType != TokenType::NONE;
}

TokenType Expr::getOperator() const {
	return this->tokenType;
}

bool Expr::isNullExpr() const {
	return this->tokenType == TokenType::NONE && !this->hasTerm() && !this->hasExpr();
}

std::shared_ptr<Expr> Expr::getExpr() const {
	return expr;
}

Expr* Expr::getExprPtr() const {
	if (!this->hasExpr()) {
		return nullptr; 
	}
	return expr.get(); 
}

Term Expr::getTerm() const {
	return term;
}

Term* Expr::getTermPtr() {
	if (!this->hasTerm()) {
		return nullptr;
	}
	return &term;
}

bool Expr::hasExpr() const {
	return exprFlag; 
}

bool Expr::hasTerm() const {
	return termFlag; 
}

void Expr::setOperator(TokenType tokenType) {
	this->tokenType = tokenType;
}

void Expr::setExpr(std::shared_ptr<Expr> expr) {
	this->expr = expr;
	this->exprFlag = true;
}

void Expr::setTerm(Term term) {
	this->term = term;
	this->termFlag = true; 
}