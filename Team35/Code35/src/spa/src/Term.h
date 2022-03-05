#include "Factor.h"
#include "TokenType.h"

#ifndef TERM_H
#define TERM_H

class Term {
private:
	TokenType tokenType;
	Term* term;
	Factor* factor;
	bool termFlag; 
	bool factorFlag; 
public:
	Term();
	TokenType getOperator() const;
	Term getTerm() const;
	Term* getTermPtr() const; 
	Factor* getFactorPtr() const;
	Factor getFactor() const;
	bool hasTerm() const;
	bool hasFactor() const; 
	
	void setOperator(TokenType tokenType);
	void setTerm(Term term);
	void setFactor(Factor factor);
};

#endif