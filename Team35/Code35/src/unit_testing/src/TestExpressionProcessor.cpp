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

	tokenQueue = tokeniser.putInQueue("x;");
	Expr d = validator.parseExpr(tokenQueue);

	tokenQueue = tokeniser.putInQueue("x * z;");
	Expr e = validator.parseExpr(tokenQueue);

	tokenQueue = tokeniser.putInQueue("z - 8;");
	Expr f = validator.parseExpr(tokenQueue);

	REQUIRE(expressionProcessor.fullfillsMatching(a, c, ExpressionIndicator::FULL_MATCH) == true);
	REQUIRE(expressionProcessor.fullfillsMatching(b, a, ExpressionIndicator::PARTIAL_MATCH) == true); 
	REQUIRE(expressionProcessor.fullfillsMatching(d, a, ExpressionIndicator::PARTIAL_MATCH) == true);
	REQUIRE(expressionProcessor.fullfillsMatching(e, a, ExpressionIndicator::PARTIAL_MATCH) == false);
	REQUIRE(expressionProcessor.fullfillsMatching(f, a, ExpressionIndicator::PARTIAL_MATCH) == true);
	

}

TEST_CASE("Check Similarity 2") {
	//Creating expression for v+x*y+z*t
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

	tokenQueue = tokeniser.putInQueue("x*y+z;");
	Expr d = validator.parseExpr(tokenQueue);

	REQUIRE(expressionProcessor.fullfillsMatching(a, c, ExpressionIndicator::FULL_MATCH) == true);
	REQUIRE(expressionProcessor.fullfillsMatching(b, a, ExpressionIndicator::PARTIAL_MATCH) == true);
	REQUIRE(expressionProcessor.fullfillsMatching(d, a, ExpressionIndicator::PARTIAL_MATCH) == false);


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

	REQUIRE(expressionProcessor.fullfillsMatching(b, a, ExpressionIndicator::PARTIAL_MATCH) == false);
	REQUIRE(expressionProcessor.fullfillsMatching(a, c, ExpressionIndicator::FULL_MATCH) == false);
	REQUIRE(expressionProcessor.fullfillsMatching(a, d, ExpressionIndicator::FULL_MATCH) == false);
}

TEST_CASE("Check Similarity 4") {
	//Creating expression for x
	// We will use the tokeniser to create the tokenqueue

	ExpressionProcessor expressionProcessor;

	// TO BE REPLACED WITH STRING TO EXPR
	Tokeniser tokeniser;
	std::queue<Token> tokenQueue = tokeniser.putInQueue("x;");

	// Using the concretesyntax validator to parse the expression
	ConcreteSyntaxWithValidation validator;
	Expr a = validator.parseExpr(tokenQueue);

	tokenQueue = tokeniser.putInQueue("x;");
	Expr b = validator.parseExpr(tokenQueue);

	tokenQueue = tokeniser.putInQueue("x+(y+w)*5;");
	Expr c = validator.parseExpr(tokenQueue);

	REQUIRE(expressionProcessor.fullfillsMatching(b, a, ExpressionIndicator::PARTIAL_MATCH) == true);
	REQUIRE(expressionProcessor.fullfillsMatching(c, a, ExpressionIndicator::PARTIAL_MATCH) == false);
}
