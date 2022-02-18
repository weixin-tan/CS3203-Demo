#include "Factor.h"
#include "TokenType.h"

#ifndef TERM_H
#define TERM_H

class Term {
private:
	TokenType tokenType;
	Term* term;
	Factor factor;
public:
	Term();
	TokenType getOperator() const;
	Term getTerm() const;
	Factor getFactor() const;
	void setOperator(TokenType tokenType);
	void setTerm(Term term);
	void setFactor(Factor factor);
};

#endif