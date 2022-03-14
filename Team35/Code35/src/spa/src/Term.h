#include <memory>
#include "Factor.h"
#include "TokenType.h"

#ifndef TERM_H
#define TERM_H

class Term {
private:
    TokenType tokenType;
    std::shared_ptr<Term> term;
    Factor factor;
    bool termFlag; 
    bool factorFlag; 
public:
    Term();
    TokenType getOperator() const;
    std::shared_ptr<Term> getTerm() const;
    Term* getTermPtr() const; 
    Factor* getFactorPtr();
    Factor getFactor() const;
    bool hasTerm() const;
    bool hasFactor() const; 
    
    void setOperator(TokenType tokenType);
    void setTerm(std::shared_ptr<Term> term);
    void setFactor(Factor factor);
};

#endif