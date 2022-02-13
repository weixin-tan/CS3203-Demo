#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "Clause.h"
#include "QueryRegex.h"
#include "catch.hpp"
#include "QueryProcessor.h"

using namespace std;
QueryProcessor qp = QueryProcessor();
Clause c1 = Clause();
Clause c2 = Clause();
Clause c3 = Clause();
Clause c11 = Clause();

std::string s1 = "while w; Select w";
std::string s2 = "variable v; Select v";
std::string s3 = "stmt s;            Select    s   such      that     Follows*(6,s)";
/*
std::string s4 = "variable v     ;Select v such that Modifies    (    6   ,   v   )  ";
std::string s5 = "assign a; while w;\nSelect a such that Parent* (w, a)";
std::string s6 = "assign a; Select a pattern a ( _ , \"count + 1\")";
std::string s7 = "assign newa; Select newa pattern newa ( \"normSq\" , _\"cenX * cenX\"_)";
std::string s8 = "assign a; while w;\nSelect w such that Parent* (w, a) pattern a (\"count\", _)";
std::string s9 = "assign a; variable v;Select a such that Uses (a, v) pattern a (v, _)";
std::string s10 = "assign a; Select a pattern a (\"x\", _) such that Uses (a, \"x\")";
*/
std::string s11 = "assign a; while w; Select a such that Parent* (w, a) pattern a (\"count\", _)";

Entity w = Entity(EntityType::While, "w");
Entity v = Entity(EntityType::Variable, "v");
Entity a = Entity(EntityType::Assignment, "a");
Entity s = Entity(EntityType::Statement, "s");
Entity wildcardEntity = Entity(EntityType::Wildcard, "_");
Entity countEntity = Entity(EntityType::FixedString, "count");
Entity fixedInt = Entity(EntityType::FixedInteger, "6");

RelationshipRef r3 = RelationshipRef(RelationshipType::FollowsT, fixedInt, s);
RelationshipRef r11_a = RelationshipRef(RelationshipType::ParentT, w, a);
RelationshipRef r11_b = RelationshipRef(RelationshipType::Pattern, countEntity, wildcardEntity, a);

Clause s1_output = qp.parsePQL(s1)[0];
Clause s2_output = qp.parsePQL(s2)[0];
Clause s3_output = qp.parsePQL(s3)[0];
Clause s11_output = qp.parsePQL(s11)[0];

TEST_CASE("testing valid queries on parseQuery"){
  c1.appendEntityToFind(w);

  c2.appendEntityToFind(v);

  c3.appendEntityToFind(s);
  c3.appendRef(r3);

  c11.appendEntityToFind(a);
  c11.appendRef(r11_a);
  c11.appendRef(r11_b);

  SECTION("parse query with 1 such-that clause"){
    REQUIRE((c1 == s1_output));
    REQUIRE((c2 == s2_output));
  }

  SECTION("parse query with 1 pattern clause"){
    REQUIRE((c3 == s3_output));
  }

  SECTION("parse query with 1 pattern and 1 such-that clause"){
    REQUIRE((s11_output == c11));
  }
}

TEST_CASE("testing object creation"){

}
/*
METHODS TO TEST
static void createDeclarationObjects(std::vector<std::string> designEntityArr, std::unordered_map<std::string, Entity>* entityMap);
static RelationshipRef createRelationshipObject(std::vector<std::string> relRefList, std::unordered_map<std::string, Entity>* entityMap);
static Entity findRelationshipEntity(const std::string& s, std::unordered_map<std::string, Entity>* entityMap);
static RelationshipRef createPatternObject(std::vector<std::string> patternList, std::unordered_map<std::string, Entity>* entityMap);
*/