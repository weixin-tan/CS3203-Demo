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
  string falseident3 = "0a";

  string integer1 = "1";
  string integer2 = "123456789012309830912";
  string falseinteger1 = "-1";
  string falseinteger2 = "1abc";
  string falseinteger3 = "$13";
  string falseinteger4 = "a0123";

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
    REQUIRE(isIdent(falseident3) == false);
  };

  SECTION("checking isInteger Method"){
    REQUIRE(isInteger(integer1) == true);
    REQUIRE(isInteger(integer2) == true);
    REQUIRE(isInteger(falseinteger1) == false);
    REQUIRE(isInteger(falseinteger2) == false);
    REQUIRE(isInteger(falseinteger3) == false);
    REQUIRE(isInteger(falseinteger4) == false);
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

  SECTION("checking isStmtRef"){
    REQUIRE(isStmtRef(ident2));
    REQUIRE(isStmtRef(wildcard1));
    REQUIRE(isStmtRef(integer1));
  }

  SECTION("checking isEntRef"){
    REQUIRE(isEntRef(ident3));
    REQUIRE(isEntRef(wildcard1));
    REQUIRE(isEntRef(quotationident2));
    REQUIRE(!(isEntRef(integer1)));
  }

  SECTION("checking isSelect"){
    REQUIRE(isSelect("Select something string"));
    REQUIRE(!isSelect("fSelect wrong"));
    REQUIRE(!isSelect("no such word"));
    REQUIRE(!isSelect("select wrong capitalize"));
  }

  SECTION("checking isPattern"){
    REQUIRE(isPattern("pattern some string"));
    REQUIRE(!isPattern("ipattern wrong string"));
    REQUIRE(!isPattern("Pattern wrong string"));
    REQUIRE(!isPattern("no such word"));
  }

  SECTION("checking existsSuchThat"){
    REQUIRE(existSuchThat("some string lalala such      \t\n that"));
    REQUIRE(!existSuchThat("such wrong that"));
    REQUIRE(!existSuchThat("that that such such such"));
    REQUIRE(!existSuchThat("no string"));
  }
}

TEST_CASE("test advanced boolean methods"){
  SECTION("checking checkDesignEntitySynonymsList"){
    REQUIRE(checkDesignEntitySynonymsList({"constant", "a", "b", "c"}));
    REQUIRE(!checkDesignEntitySynonymsList({"invalid", "a"}));
    REQUIRE(!checkDesignEntitySynonymsList({"variable", "1"}));
  }

  SECTION("checking checkRelRefList"){
    REQUIRE(checkRelRefList({"Modifies", "x", "_"}));
    REQUIRE(!checkRelRefList({"invalid", "x", "y"}));
    REQUIRE(!checkRelRefList({"Uses", "1a", "y"}));
    REQUIRE(!checkRelRefList({"Uses", "a", "_1"}));
  }
}