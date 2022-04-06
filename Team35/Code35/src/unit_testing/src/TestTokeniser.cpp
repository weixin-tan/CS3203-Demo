#include <regex>
#include <unordered_map>

#include "Tokeniser.h"
#include "Token.h"
#include "TokenType.h"
#include "catch.hpp"

using namespace std;

TEST_CASE("basic methods"){
    //testing isNumber method
    string isNumber0 = "01";
    string isNumber1 = "10";
    string isNumber2 = "0";
    string isNumber3 = "1";
    string isNumber4 = "001";
    string isNumber5 = "10";
    string isNumber6 = "99";
    string isNumber7 = "5";
    string isNumber8 = "62";
    string isNumber9 = "42";

    string isNumber10 = "a01";
    string isNumber11 = "10a";
    string isNumber12 = "a0";
    string isNumber13 = "1a";
    string isNumber14 = "b001";
    string isNumber15 = "10b";
    string isNumber16 = "b99";
    string isNumber17 = "b5";
    string isNumber18 = "62b";
    string isNumber19 = "v42";

    SECTION("checking isNumber method"){
        Tokeniser t;
        REQUIRE(t.isNumber(isNumber0) == false);
        REQUIRE(t.isNumber(isNumber1) == true);
        REQUIRE(t.isNumber(isNumber3) == true);
        REQUIRE(t.isNumber(isNumber4) == false);
        REQUIRE(t.isNumber(isNumber5) == true);
        REQUIRE(t.isNumber(isNumber6) == true);
        REQUIRE(t.isNumber(isNumber7) == true);
        REQUIRE(t.isNumber(isNumber8) == true);
        REQUIRE(t.isNumber(isNumber9) == true);

        REQUIRE(t.isNumber(isNumber10) == false);
        REQUIRE(t.isNumber(isNumber11) == false);
        REQUIRE(t.isNumber(isNumber12) == false);
        REQUIRE(t.isNumber(isNumber13) == false);
        REQUIRE(t.isNumber(isNumber14) == false);
        REQUIRE(t.isNumber(isNumber15) == false);
        REQUIRE(t.isNumber(isNumber16) == false);
        REQUIRE(t.isNumber(isNumber17) == false);
        REQUIRE(t.isNumber(isNumber18) == false);
        REQUIRE(t.isNumber(isNumber19) == false);
    }

    //testing isAlphabet
    string isName0 = "a";
    string isName1 = "A";
    string isName2 = "aA";
    string isName3 = "Aa";
    string isName4 = "a0";
    string isName5 = "A0";
    string isName6 = "a0a";
    string isName7 = "a0A";
    string isName8 = "A0a";
    string isName9 = "A0A";

    string isName10 = "0";
    string isName11 = "0a";
    string isName12 = "0A";
    string isName13 = "001";
    string isName14 = "1a";
    string isName15 = "1A";
    string isName16 = "0a0";
    string isName17 = "0A0";
    string isName18 = "01a";
    string isName19 = "0a1";
    string isName20 = "10a";

    SECTION("checking isName method"){
        Tokeniser t;
        REQUIRE(t.isName(isName0) == true);
        REQUIRE(t.isName(isName1) == true);
        REQUIRE(t.isName(isName2) == true);
        REQUIRE(t.isName(isName3) == true);
        REQUIRE(t.isName(isName4) == true);
        REQUIRE(t.isName(isName5) == true);
        REQUIRE(t.isName(isName6) == true);
        REQUIRE(t.isName(isName7) == true);
        REQUIRE(t.isName(isName8) == true);
        REQUIRE(t.isName(isName9) == true);

        REQUIRE(t.isName(isName10) == false);
        REQUIRE(t.isName(isName11) == false);
        REQUIRE(t.isName(isName12) == false);
        REQUIRE(t.isName(isName13) == false);
        REQUIRE(t.isName(isName14) == false);
        REQUIRE(t.isName(isName15) == false);
        REQUIRE(t.isName(isName16) == false);
        REQUIRE(t.isName(isName17) == false);
        REQUIRE(t.isName(isName18) == false);
        REQUIRE(t.isName(isName19) == false);
        REQUIRE(t.isName(isName20) == false);

    }

    //testing trim
    string trim0 = "a";
    string trim1 = " a";
    string trim2 = "a ";
    string trim3 = " a ";
    string trim4 = "  a";
    string trim5 = "a  ";
    string trim6 = "  a  ";

    string trim7 = "ab";
    string trim8 = " ab";
    string trim9 = "ab ";
    string trim10 = " ab ";
    string trim11 = "  ab";
    string trim12 = "ab  ";
    string trim13 = "  ab  ";

    SECTION("checking trim method"){
        Tokeniser t;
        REQUIRE(t.trim(trim0) == "a");
        REQUIRE(t.trim(trim1) == "a");
        REQUIRE(t.trim(trim2) == "a");
        REQUIRE(t.trim(trim3) == "a");
        REQUIRE(t.trim(trim4) == "a");
        REQUIRE(t.trim(trim5) == "a");
        REQUIRE(t.trim(trim6) == "a");

        REQUIRE(t.trim(trim7) == "ab");
        REQUIRE(t.trim(trim8) == "ab");
        REQUIRE(t.trim(trim9) == "ab");
        REQUIRE(t.trim(trim10) == "ab");
        REQUIRE(t.trim(trim11) == "ab");
        REQUIRE(t.trim(trim12) == "ab");
        REQUIRE(t.trim(trim13) == "ab");

    }

    //testing addSpace
    string addSpace0 = "!=";
    string addSpace1 = "==";
    string addSpace2 = ">=";
    string addSpace3 = "<=";
    string addSpace4 = "&&";
    string addSpace5 = "||";
    string addSpace6 = "0";
    string addSpace7 = "1";
    string addSpace8 = "a";
    string addSpace9 = "A";

    SECTION("checking addSpace method"){
        Tokeniser t;
        REQUIRE(t.addSpace(addSpace0) == " != ");
        REQUIRE(t.addSpace(addSpace1) == " == ");
        REQUIRE(t.addSpace(addSpace2) == " >= ");
        REQUIRE(t.addSpace(addSpace3) == " <= ");
        REQUIRE(t.addSpace(addSpace4) == " && ");
        REQUIRE(t.addSpace(addSpace5) == " || ");
        REQUIRE(t.addSpace(addSpace6) == "0");
        REQUIRE(t.addSpace(addSpace7) == "1");
        REQUIRE(t.addSpace(addSpace8) == "a");
        REQUIRE(t.addSpace(addSpace9) == "A");

    }

}

/*
methods to test
    bool isNumber(const std::string& str); check
    bool isAlphabet(const std::string& str); not needed, checked in isName
    bool isName(const std::string& str); check
    Token tokeniser(std::string input);
    std::queue<Token> putInQueue (std::string input);
    std::string printToken(Token token); not needed
    std::string lTrim(std::string s); not needed, checked in trim
    std::string rTrim(std::string s); not needed, checked in trim
    std::string trim(std::string s); check
    std::string addSpace(std::string s); check
    std::queue<Token> forbiddenWord(std::queue<Token> inputQueue);

 */