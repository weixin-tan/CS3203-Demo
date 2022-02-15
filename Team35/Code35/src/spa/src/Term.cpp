#include "Term.h"

Term::Term() {

}

TokenType Term::getOperator() const {
	return this->tokenType;
}

Term Term::getTerm() const {
	return *term;
}

Factor Term::getFactor() const {
	return this->factor;
}

void Term::setOperator(TokenType tokenType) {
	this->tokenType = tokenType;
}

void Term::setTerm(Term term) {
	this->term = &term;
}

void Term::setFactor(Factor factor) {
	this->factor = factor;
}