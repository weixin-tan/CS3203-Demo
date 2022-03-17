#include "Term.h"

Term::Term() {
    this->termFlag = false;
    this->factorFlag = false; 
}

TokenType Term::getOperator() const {
    return this->tokenType;
}

std::shared_ptr<Term> Term::getTerm() const {
    return term;
}

Term* Term::getTermPtr() const {
    if (!this->hasTerm()) {
        return nullptr; 
    }
    return this->term.get();
}

Factor Term::getFactor() const {
    return factor;
}

Factor* Term::getFactorPtr() {
    if (!this->hasFactor()) {
        return nullptr;
    }
    return &factor;
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

void Term::setTerm(std::shared_ptr<Term> term) {
    this->term = term;
    this->termFlag = true;
}

void Term::setFactor(Factor factor) {
    this->factor = factor;
    this->factorFlag = true; 
}