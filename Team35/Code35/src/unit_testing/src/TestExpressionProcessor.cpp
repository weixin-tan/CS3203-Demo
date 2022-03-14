#include "ExpressionProcessor.h"

#include "catch.hpp"

TEST_CASE("Check Similarity 1") {
    //Creating expression for z - 8 - (x * y) * z
    // We will use the tokeniser to create the tokenqueue

    ExpressionProcessor expressionProcessor;

    Expr a = expressionProcessor.stringToExpr("z - 8 - (x * y) * z;");

    Expr b = expressionProcessor.stringToExpr("x*y;");

    Expr c = expressionProcessor.stringToExpr("z - 8 - (x * y) * z;");

    Expr d = expressionProcessor.stringToExpr("x;");

    Expr e = expressionProcessor.stringToExpr("x * z;");

    Expr f = expressionProcessor.stringToExpr("z - 8;");

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

    Expr a = expressionProcessor.stringToExpr("v+x*y+z*t;");

    Expr b = expressionProcessor.stringToExpr("x*y;");

    Expr c = expressionProcessor.stringToExpr("v+x*y+z*t;");

    Expr d = expressionProcessor.stringToExpr("x*y+z;");

    REQUIRE(expressionProcessor.fullfillsMatching(a, c, ExpressionIndicator::FULL_MATCH) == true);
    REQUIRE(expressionProcessor.fullfillsMatching(b, a, ExpressionIndicator::PARTIAL_MATCH) == true);
    REQUIRE(expressionProcessor.fullfillsMatching(d, a, ExpressionIndicator::PARTIAL_MATCH) == false);


}

TEST_CASE("Check Similarity 3") {
    //Creating expression for z - 8 - (x * y) * z
    // We will use the tokeniser to create the tokenqueue

    ExpressionProcessor expressionProcessor;

    Expr a = expressionProcessor.stringToExpr("x+(y+z)*5;");

    Expr b = expressionProcessor.stringToExpr("x*y;");

    Expr c = expressionProcessor.stringToExpr("x+(y+w)*5;");

    Expr d = expressionProcessor.stringToExpr("x+(y+z)*2;");

    REQUIRE(expressionProcessor.fullfillsMatching(b, a, ExpressionIndicator::PARTIAL_MATCH) == false);
    REQUIRE(expressionProcessor.fullfillsMatching(a, c, ExpressionIndicator::FULL_MATCH) == false);
    REQUIRE(expressionProcessor.fullfillsMatching(a, d, ExpressionIndicator::FULL_MATCH) == false);
}

TEST_CASE("Check Similarity 4") {
    //Creating expression for x
    // We will use the tokeniser to create the tokenqueue

    ExpressionProcessor expressionProcessor;

    Expr a = expressionProcessor.stringToExpr("x;");

    Expr b = expressionProcessor.stringToExpr("x;");

    Expr c = expressionProcessor.stringToExpr("x+(y+w)*5;");

    REQUIRE(expressionProcessor.fullfillsMatching(b, a, ExpressionIndicator::PARTIAL_MATCH) == true);
    REQUIRE(expressionProcessor.fullfillsMatching(c, a, ExpressionIndicator::PARTIAL_MATCH) == false);
}

TEST_CASE("Check integration") {

    ExpressionProcessor expressionProcessor;

    Expr a = expressionProcessor.stringToExpr("z - 8 - (x * y) * z");

    Expr b = expressionProcessor.stringToExpr("x*y");

    Expr c = expressionProcessor.stringToExpr("z - 8 - (x * y) * z");

    Expr d = expressionProcessor.stringToExpr("x");

    Expr e = expressionProcessor.stringToExpr("x * z");

    Expr f = expressionProcessor.stringToExpr("z - 8;");

    REQUIRE(expressionProcessor.fullfillsMatching(a, c, ExpressionIndicator::FULL_MATCH) == true);
    REQUIRE(expressionProcessor.fullfillsMatching(b, a, ExpressionIndicator::PARTIAL_MATCH) == true);
    REQUIRE(expressionProcessor.fullfillsMatching(d, a, ExpressionIndicator::PARTIAL_MATCH) == true);
    REQUIRE(expressionProcessor.fullfillsMatching(e, a, ExpressionIndicator::PARTIAL_MATCH) == false);
    REQUIRE(expressionProcessor.fullfillsMatching(f, a, ExpressionIndicator::PARTIAL_MATCH) == true);
    
}