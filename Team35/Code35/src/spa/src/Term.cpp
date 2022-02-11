#include "Term.h"

Term::Term() {

}

Operator Term::getOperator() const {
	return this->operator;
}

Term Term::getTerm() const {
	return this->term;
}

Factor Term::getFactor() const {
	return this->factor;
}

void Term::setOperator(Operator operator) {
	this->operator = operator;
}

void Term::setTerm(Term term) {
	this->term = term;
}

void Term::setFactor(Factor factor) {
	this->factor = factor;
}