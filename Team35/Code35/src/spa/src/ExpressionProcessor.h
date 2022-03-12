#include "token.h"
#include "Expr.h"
#include "ExpressionIndicator.h"
#include "stdio.h"

class ExpressionProcessor {

	bool areIdenticalExpr(Expr* root1, Expr* root2) const;
	bool areIdenticalTerm(Term* root1, Term* root2) const;
	bool areIdenticalFactor(Factor* root1, Factor* root2) const;
	bool isSubtree(Expr* root1, Expr* root2) const;
	bool isSubtree(Expr* root1, Term* term2) const;
	bool isSubtree(Term* term1, Term* term2) const;
	bool isSubtree(Term* term1, Expr* root2) const;
	std::shared_ptr<Expr> getNestedExpr(Term* root2) const;


public:

	Expr stringToExpr(std::string query) const;
	bool fullfillsMatching(Expr exp1, Expr exp2, ExpressionIndicator expressionIndicator) const;
};
