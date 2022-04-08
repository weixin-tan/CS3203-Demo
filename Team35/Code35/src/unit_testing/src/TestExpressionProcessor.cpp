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

TEST_CASE("Check brackets Similarity") {
    //Creating expression for x
    // We will use the tokeniser to create the tokenqueue

    ExpressionProcessor expressionProcessor;

    Expr a = expressionProcessor.stringToExpr("(x);");

    Expr b = expressionProcessor.stringToExpr("x;");

    Expr c = expressionProcessor.stringToExpr("((x))");

    REQUIRE(expressionProcessor.fullfillsMatching(b, a, ExpressionIndicator::PARTIAL_MATCH) == true);
    REQUIRE(expressionProcessor.fullfillsMatching(b, c, ExpressionIndicator::PARTIAL_MATCH) == true);
    REQUIRE(expressionProcessor.fullfillsMatching(a, c, ExpressionIndicator::PARTIAL_MATCH) == true);

}

//just to check if modulo is recognised. 
TEST_CASE("Check Modulo") {
    ExpressionProcessor expressionProcessor; 

    Expr a = expressionProcessor.stringToExpr("z - 8 - (x % y) % z + 35 - (7 * x)");

    Expr b = expressionProcessor.stringToExpr("x % y");

    Expr c = expressionProcessor.stringToExpr("(x%y)%z");

    REQUIRE(expressionProcessor.fullfillsMatching(b, a, ExpressionIndicator::PARTIAL_MATCH) == true);
    REQUIRE(expressionProcessor.fullfillsMatching(c, a, ExpressionIndicator::PARTIAL_MATCH) == true);
    REQUIRE(expressionProcessor.fullfillsMatching(a, a, ExpressionIndicator::FULL_MATCH) == true);

}

TEST_CASE("Nested Example - nested at front") {
    ExpressionProcessor expressionProcessor;

    Expr a = expressionProcessor.stringToExpr("(((((z -y)-x)- 1 - t)))");

    Expr b = expressionProcessor.stringToExpr("z-y");

    Expr c = expressionProcessor.stringToExpr("z-y-x");

    Expr d = expressionProcessor.stringToExpr("(z-y)-x");

    Expr e = expressionProcessor.stringToExpr("1-t");

    REQUIRE(expressionProcessor.fullfillsMatching(b, a, ExpressionIndicator::PARTIAL_MATCH) == true);
    REQUIRE(expressionProcessor.fullfillsMatching(c, a, ExpressionIndicator::PARTIAL_MATCH) == false);
    REQUIRE(expressionProcessor.fullfillsMatching(d, a, ExpressionIndicator::PARTIAL_MATCH) == true);
    REQUIRE(expressionProcessor.fullfillsMatching(e, a, ExpressionIndicator::PARTIAL_MATCH) == false);

}

TEST_CASE("Nested Example - nested at back") {
    ExpressionProcessor expressionProcessor;

    Expr a = expressionProcessor.stringToExpr("(z -y + (x + ( 3 + (1 - t))))");

    Expr b = expressionProcessor.stringToExpr("1-t");

    Expr c = expressionProcessor.stringToExpr("z-y");

    Expr d = expressionProcessor.stringToExpr("x + 3");

    Expr e = expressionProcessor.stringToExpr("y");

    REQUIRE(expressionProcessor.fullfillsMatching(b, a, ExpressionIndicator::PARTIAL_MATCH) == true);
    REQUIRE(expressionProcessor.fullfillsMatching(c, a, ExpressionIndicator::PARTIAL_MATCH) == true);
    REQUIRE(expressionProcessor.fullfillsMatching(d, a, ExpressionIndicator::PARTIAL_MATCH) == false);
    REQUIRE(expressionProcessor.fullfillsMatching(e, a, ExpressionIndicator::PARTIAL_MATCH) == true);

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