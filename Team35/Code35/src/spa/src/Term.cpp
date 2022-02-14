#include "Term.h"

Term::Term() {

}

Operator Term::getOperator() const {
	return this->op;
}

Term Term::getTerm() const {
	return *term;
}

Factor Term::getFactor() const {
	return this->factor;
}

void Term::setOperator(Operator op) {
	this->op = op;
}

void Term::setTerm(Term term) {
	this->term = &term;
}

void Term::setFactor(Factor factor) {
	this->factor = factor;
}