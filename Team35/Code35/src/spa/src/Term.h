#include "Factor.h"

#ifndef TERM_H
#define TERM_H

enum Operator{PLUS, MINUS};

class Term {
private:
	Operator operator;
	Term term;
	Factor factor;
public:
	Term();
	Operator getOperator() const;
	Term getTerm() const;
	Factor getFactor() const;
	void setOperator(Operator operator);
	void setTerm(Term term);
	void setFactor(Factor factor);
};

#endif