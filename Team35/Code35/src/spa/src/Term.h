#include "Factor.h"
#include "Operator.h"

#ifndef TERM_H
#define TERM_H

class Term {
private:
	Operator op;
	Term* term;
	Factor factor;
public:
	Term();
	Operator getOperator() const;
	Term getTerm() const;
	Factor getFactor() const;
	void setOperator(Operator op);
	void setTerm(Term term);
	void setFactor(Factor factor);
};

#endif