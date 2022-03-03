#include "token.h"
#include "Expr.h"
#include "ExpressionIndicator.h"
#include "stdio.h"

class ExpressionProcessor {
public:

	Expr stringToExpr(std::string query);

	bool fullfillsMatching(Expr exp1, Expr exp2, ExpressionIndicator expressionIndicator);
};



