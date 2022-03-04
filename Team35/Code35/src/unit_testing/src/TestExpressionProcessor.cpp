#include "ExpressionProcessor.h"
#include "ConcreteSyntaxWithValidation.h"
#include "Tokeniser.h"

#include "catch.hpp"

TEST_CASE("Check Similarity 1") {
	//Creating expression for z - 8 - (x * y) * z
	// We will use the tokeniser to create the tokenqueue

	ExpressionProcessor expressionProcessor;

	// TO BE REPLACED WITH STRING TO EXPR
	Tokeniser tokeniser;
	std::queue<Token> tokenQueue = tokeniser.putInQueue("z - 8 - (x * y) * z;");

	// Using the concretesyntax validator to parse the expression
	ConcreteSyntaxWithValidation validator;
	Expr a = validator.parseExpr(tokenQueue);

	tokenQueue = tokeniser.putInQueue("x*y;");
	Expr b = validator.parseExpr(tokenQueue);

	REQUIRE(expressionProcessor.fullfillsMatching(a, b, ExpressionIndicator::PARTIAL_MATCH) == true); 


}
