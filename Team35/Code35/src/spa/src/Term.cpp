#include "Term.h"

Term::Term() {
	this->termFlag = false;
	this->factorFlag = false; 
}

TokenType Term::getOperator() const {
	return this->tokenType;
}

Term Term::getTerm() const {
	return *term;
}

Term* Term::getTermPtr() const {
	if (!this->hasTerm()) {
		return nullptr; 
	}
	return this->term;
}

Factor Term::getFactor() const {
	return *factor;
}

Factor* Term::getFactorPtr() const{
	if (!this->hasFactor()) {
		return nullptr;
	}
	return factor;
}

bool Term::hasTerm() const {
	return this->termFlag;
}

bool Term::hasFactor() const {
	return this->factorFlag;
}

void Term::setOperator(TokenType tokenType) {
	this->tokenType = tokenType;
}

void Term::setTerm(Term term) {
	this->term = &term;
	this->termFlag = true;
}

void Term::setFactor(Factor factor) {
	this->factor = &factor;
	this->factorFlag = true; 
}