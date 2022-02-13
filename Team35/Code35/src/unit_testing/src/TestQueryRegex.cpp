#include <regex>
#include <unordered_map>

#include "Entity.h"
#include "QueryRegex.h"
#include "catch.hpp"

using namespace std;

TEST_CASE("test basic boolean methods"){
  string ident1 = "a";
  string ident2 = "a1";
  string ident3 = "abcdefg";
  string ident4 = "a123456";
  string falseident1 = "$1";
  string falseident2 = "1bc";

  string integer1 = "1";
  string integer2 = "123456789012309830912";
  string falseinteger1 = "-1";
  string falseinteger2 = "1abc";
  string falseinteger3 = "$13";

  string wildcard1 = "_";
  string falsewildcard1 = "__";
  string falsewildcard2 = "_\"x\"_";

  string quotationident1 = "\"a\"";
  string quotationident2 = "\"a123\"";
  string quotationident3 = "\"abc\"";
  string falsequotationident1 = "\"123\"";
  string falsequotationident2 = "\"$@!\"";

  string quotationwildcard1 = "_\"abc + % & @ cde\"_";
  string quotationwildcard2 = "_\"1234567\"_";

  SECTION("checking isIdent Method"){
    REQUIRE(isIdent(ident1) == true);
    REQUIRE(isIdent(ident2) == true);
    REQUIRE(isIdent(ident3) == true);
    REQUIRE(isIdent(ident4) == true);
    REQUIRE(isIdent(falseident1) == false);
    REQUIRE(isIdent(falseident2) == false);
  };

  SECTION("checking isInteger Method"){
    REQUIRE(isInteger(integer1) == true);
    REQUIRE(isInteger(integer2) == true);
    REQUIRE(isInteger(falseinteger1) == false);
    REQUIRE(isInteger(falseinteger2) == false);
    REQUIRE(isInteger(falseinteger3) == false);
  };

  SECTION("checking isWildCard Method"){
    REQUIRE(isWildCard(wildcard1) == true);
    REQUIRE(isWildCard(falsewildcard1) == false);
    REQUIRE(isWildCard(integer1) == false);
    REQUIRE(isWildCard(ident1) == false);
  };

  SECTION("checking isQuotationIdent Method"){
    REQUIRE(isQuotationIdent(quotationident1) == true);
    REQUIRE(isQuotationIdent(quotationident2) == true);
    REQUIRE(isQuotationIdent(quotationident3) == true);
    REQUIRE(isQuotationIdent(ident1) == false);
    REQUIRE(isQuotationIdent(integer1) == false);
    REQUIRE(isQuotationIdent(wildcard1) == false);
    REQUIRE(isQuotationIdent(falsequotationident1) == false);
    REQUIRE(isQuotationIdent(falsequotationident2) == false);
  };

  SECTION("checking isStringWithinWildCard Method"){
    REQUIRE(isStringWithinWildCard(quotationwildcard1) == true);
    REQUIRE(isStringWithinWildCard(quotationwildcard2) == true);
    REQUIRE(isStringWithinWildCard(ident3) == false);
    REQUIRE(isStringWithinWildCard(integer2) == false);
    REQUIRE(isStringWithinWildCard(wildcard1) == false);
    REQUIRE(isStringWithinWildCard(quotationident2) == false);
  }


}
/*
METHODS TO TEST
bool isStmtRef(const std::string& s);
bool isEntRef(const std::string& s);

bool checkDesignEntitySynonyms(std::vector<std::string> sArr);
bool isSelect(const std::string& s);
bool isPattern(const std::string& s);

bool checkRelRefList(std::vector<std::string> s);
bool checkPatternList(std::vector<std::string> patternList, std::unordered_map<std::string, Entity>* entityMap);
bool entityMapContains(const std::string& s, std::unordered_map<std::string, Entity>* entityMap);

std::string stripString(std::string s);
std::vector<std::string> splitString(const std::string& s, const std::string& delimiter);

std::vector<std::string> splitDeclarationAndSelect(const std::string& s);
std::vector<std::string> extractSelect(const std::string& s);
std::vector<std::string> extractDeclaration(const std::string& s);

std::vector<std::string> extractDesignEntityAndSynonyms(const std::string& s);

std::vector<std::string> splitVariablesAndClauses(const std::string& s);
std::vector<std::string> splitPatternAndSuchThatClauses(std::string s);
std::vector<std::string> extractVariablesToSelect(const std::string& s);
std::vector<std::string> extractSuchThatClauses(const std::string& s);
std::vector<std::string> extractPatternClauses(const std::string& s);

std::vector<std::string> extractItemsInBrackets(const std::string& s);

std::string removePattern(const std::string& s);
 */