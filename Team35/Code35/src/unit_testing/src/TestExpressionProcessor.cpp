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

	tokenQueue = tokeniser.putInQueue("z - 8 - (x * y) * z;");
	Expr c = validator.parseExpr(tokenQueue);

	REQUIRE(expressionProcessor.fullfillsMatching(a, c, ExpressionIndicator::FULL_MATCH) == true);
	REQUIRE(expressionProcessor.fullfillsMatching(a, b, ExpressionIndicator::PARTIAL_MATCH) == true); 
	

}

TEST_CASE("Check Similarity 2") {
	//Creating expression for z - 8 - (x * y) * z
	// We will use the tokeniser to create the tokenqueue

	ExpressionProcessor expressionProcessor;

	// TO BE REPLACED WITH STRING TO EXPR
	Tokeniser tokeniser;
	std::queue<Token> tokenQueue = tokeniser.putInQueue("v+x*y+z*t;");

	// Using the concretesyntax validator to parse the expression
	ConcreteSyntaxWithValidation validator;
	Expr a = validator.parseExpr(tokenQueue);

	tokenQueue = tokeniser.putInQueue("x*y;");
	Expr b = validator.parseExpr(tokenQueue);

	tokenQueue = tokeniser.putInQueue("v+x*y+z*t;");
	Expr c = validator.parseExpr(tokenQueue);

	REQUIRE(expressionProcessor.fullfillsMatching(a, c, ExpressionIndicator::FULL_MATCH) == true);
	REQUIRE(expressionProcessor.fullfillsMatching(a, b, ExpressionIndicator::PARTIAL_MATCH) == true);


}

TEST_CASE("Check Similarity 3") {
	//Creating expression for z - 8 - (x * y) * z
	// We will use the tokeniser to create the tokenqueue

	ExpressionProcessor expressionProcessor;

	// TO BE REPLACED WITH STRING TO EXPR
	Tokeniser tokeniser;
	std::queue<Token> tokenQueue = tokeniser.putInQueue("x+(y+z)*5;");

	// Using the concretesyntax validator to parse the expression
	ConcreteSyntaxWithValidation validator;
	Expr a = validator.parseExpr(tokenQueue);

	tokenQueue = tokeniser.putInQueue("x*y;");
	Expr b = validator.parseExpr(tokenQueue);

	tokenQueue = tokeniser.putInQueue("x+(y+w)*5;");
	Expr c = validator.parseExpr(tokenQueue);

	tokenQueue = tokeniser.putInQueue("x+(y+z)*2;");
	Expr d = validator.parseExpr(tokenQueue);

	REQUIRE(expressionProcessor.fullfillsMatching(a, c, ExpressionIndicator::FULL_MATCH) == false);
	// REQUIRE(expressionProcessor.fullfillsMatching(a, b, ExpressionIndicator::PARTIAL_MATCH) == false);
	REQUIRE(expressionProcessor.fullfillsMatching(a, d, ExpressionIndicator::FULL_MATCH) == false);
}
