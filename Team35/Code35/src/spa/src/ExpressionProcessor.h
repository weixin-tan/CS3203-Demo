#include "token.h"
#include "Expr.h"
#include "ExpressionIndicator.h"
#include "stdio.h"

class ExpressionProcessor {

	bool areIdenticalExpr(Expr* root1, Expr* root2);
	bool areIdenticalTerm(Term* root1, Term* root2);
	bool areIdenticalFactor(Factor* root1, Factor* root2);
	bool isSubtree(Expr* root1, Expr* root2);
	std::shared_ptr<Expr> ExpressionProcessor::getNestedExpr(Expr* root2);

public:

	Expr stringToExpr(std::string query);
	bool fullfillsMatching(Expr exp1, Expr exp2, ExpressionIndicator expressionIndicator);
};
