#ifndef SPA_EXPRESSIONPROCESSOR_H
#define SPA_EXPRESSIONPROCESSOR_H

#include "../SP/Token.h"
#include "../SP/Tokeniser.h"
#include "../SP/Expr.h"
#include "ExpressionIndicator.h"
#include <queue>
#include <stack>
#include <iostream>

class ExpressionProcessor {

    bool areIdenticalExpr(Expr* root1, Expr* root2) const;
    bool areIdenticalTerm(Term* root1, Term* root2) const;
    bool areIdenticalFactor(Factor* root1, Factor* root2) const;
    bool isSubtree(Expr* root1, Expr* root2) const;
    bool isSubtree(Expr* root1, Term* term2) const;
    bool isSubtree(Term* term1, Term* term2) const;
    bool isSubtree(Term* term1, Expr* root2) const;
    std::shared_ptr<Expr> getNestedExpr(Term* root2) const;
    int getPriority(Token token);

public:

    static Expr stringToExpr(std::string query);
    Expr tokenQueueToExpr(std::queue<Token> tokenQueue);
    Expr parseExpr(std::queue<Token>& tokensQueue);
    Expr parseExprRecursion(std::stack<Token>& exprStack);
    Term parseTerm(std::queue<Token>& termQueue);
    Factor parseFactor(std::queue<Token>& factorQueue);
    bool fullfillsMatching(Expr exp1, Expr exp2, ExpressionIndicator expressionIndicator) const;
    std::queue<Token> formatExpr(std::queue<Token> tokensQueue);

};

#endif
