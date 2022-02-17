#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "Clause.h"
#include "QueryRegex.h"
#include "catch.hpp"
#include "QueryProcessor.h"

using namespace std;

TEST_CASE("testing valid queries on parseQuery"){
  QueryProcessor qp = QueryProcessor();
  Clause c1 = Clause();
  Clause c2 = Clause();
  Clause c3 = Clause();
  Clause c4 = Clause();
  Clause c5 = Clause();
  Clause c6 = Clause();
  Clause c7 = Clause();
  Clause c8 = Clause();
  Clause c9 = Clause();
  Clause c10 = Clause();
  Clause c11 = Clause();

  Entity a = Entity(EntityType::Assignment, "a");
  Entity s = Entity(EntityType::Statement, "s");
  Entity v = Entity(EntityType::Variable, "v");
  Entity w = Entity(EntityType::While, "w");
  Entity countEntity = Entity(EntityType::FixedString, "count");
  Entity fixedInt = Entity(EntityType::FixedInteger, "6");
  Entity wildcardEntity = Entity(EntityType::Wildcard, "_");
  Entity xEntity = Entity(EntityType::FixedString, "x");
  Entity countplus1Entity = Entity(EntityType::FixedStringWithinWildcard, "count + 1");

  RelationshipRef r3 = RelationshipRef(RelationshipType::FollowsT, fixedInt, s);
  RelationshipRef r4 = RelationshipRef(RelationshipType::Modifies, fixedInt, v);
  RelationshipRef r5 = RelationshipRef(RelationshipType::ParentT, w, a);
  RelationshipRef r6 = RelationshipRef(RelationshipType::Pattern, wildcardEntity, countplus1Entity, a);
  RelationshipRef r7 = RelationshipRef(RelationshipType::Pattern, xEntity, countplus1Entity, a);
  RelationshipRef r8_a = RelationshipRef(RelationshipType::Uses, a, v);
  RelationshipRef r8_b = RelationshipRef(RelationshipType::Pattern, v, wildcardEntity, a);
  RelationshipRef r9_a = RelationshipRef(RelationshipType::ParentT, w, a);
  RelationshipRef r9_b = RelationshipRef(RelationshipType::Pattern, countEntity, wildcardEntity, a);
  RelationshipRef r10_a = RelationshipRef(RelationshipType::Pattern, xEntity, wildcardEntity, a);
  RelationshipRef r10_b = RelationshipRef(RelationshipType::Uses, a, xEntity);
  RelationshipRef r11_a = RelationshipRef(RelationshipType::ParentT, w, a);
  RelationshipRef r11_b = RelationshipRef(RelationshipType::Pattern, countEntity, wildcardEntity, a);

  std::string s1 = "while w;Select w";
  std::string s2 = "variable      v     ; Select     v      ";
  std::string s3 = "stmt s;\nSelect\ns\nsuch\nthat\nFollows*(6,s)";
  std::string s4 = "    variable     v     ;    Select     v     such    that         Modifies (    6   ,   v   )  ";
  std::string s5 = "assign a;while w;Select a such that Parent*(w,a)";
  std::string s6 = "assign a; Select a pattern a ( _ , _\"count + 1\"_)";
  std::string s7 = "assign a; Select a pattern a ( \"x\" ,     _\"count + 1\"_      )";
  std::string s8 = "assign    a    ;    variable   v  ;   Select    a    such    that    Uses   (   a  ,  v   ) pattern a ( v ,  _   )";
  std::string s9 = "assign a;\n while w;\nSelect\nw\nsuch\nthat\nParent*(w, a)\npattern\na(\"count\", _)";
  std::string s10 = "assign a;\nSelect a pattern a (\"x\", _) such that Uses (a, \"x\")";
  std::string s11 = "assign a; while w; Select a such that Parent* (w, a) pattern a (\"count\", _)";

  Clause s1_output = qp.parsePQL(s1)[0];
  Clause s2_output = qp.parsePQL(s2)[0];
  Clause s3_output = qp.parsePQL(s3)[0];
  Clause s4_output = qp.parsePQL(s4)[0];
  Clause s5_output = qp.parsePQL(s5)[0];
  Clause s6_output = qp.parsePQL(s6)[0];
  Clause s7_output = qp.parsePQL(s7)[0];
  Clause s8_output = qp.parsePQL(s8)[0];
  Clause s9_output = qp.parsePQL(s9)[0];
  Clause s10_output = qp.parsePQL(s10)[0];
  Clause s11_output = qp.parsePQL(s11)[0];

  SECTION("parse query with 1 such-that clause"){
    c1.appendEntityToFind(w);
    c2.appendEntityToFind(v);
    REQUIRE((c1 == s1_output));
    REQUIRE((c2 == s2_output));
  }

  SECTION("parse query with 1 pattern clause"){
    c3.appendEntityToFind(s);
    c3.appendRef(r3);
    c4.appendEntityToFind(v);
    c4.appendRef(r4);
    c5.appendEntityToFind(a);
    c5.appendRef(r5);
    c6.appendEntityToFind(a);
    c6.appendRef(r6);
    c7.appendEntityToFind(a);
    c7.appendRef(r7);
    REQUIRE((c3 == s3_output));
    REQUIRE((c4 == s4_output));
    REQUIRE((c5 == s5_output));
    REQUIRE((c6 == s6_output));
    REQUIRE((c7 == s7_output));
  }

  SECTION("parse query with 1 pattern and 1 such-that clause"){
    c8.appendEntityToFind(a);
    c8.appendRef(r8_a);
    c8.appendRef(r8_b);
    c9.appendEntityToFind(w);
    c9.appendRef(r9_a);
    c9.appendRef(r9_b);
    c10.appendEntityToFind(a);
    c10.appendRef(r10_a);
    c10.appendRef(r10_b);
    c11.appendEntityToFind(a);
    c11.appendRef(r11_a);
    c11.appendRef(r11_b);
    REQUIRE((s8_output == c8));
    REQUIRE((s9_output == c9));
    REQUIRE((s10_output == c10));
    REQUIRE((s11_output == c11));
  }
}

TEST_CASE("invalid querys"){
  QueryProcessor qp = QueryProcessor();
  std::string s1 = "magic x; Select x"; //invalid declaration
  std::string s2 = "variable 2a, x; Select x"; // invalid name
  std::string s3 = "variable x; Select 2a"; // invalid name
  std::string s4 = "variable x Select x"; // missing semicolon
  std::string s5 = "variable x, v a; Select x "; // missing comma
  std::string s6 = "variable v x; Select x"; // missing comma
  std::string s7 = "variable x; Select x such that Uses(v, 3)"; //no declaration of v
  std::string s8 = "variable x; Select x Uses(x, 3)"; //no such that
  std::string s9 = "variable x; Select x such that Magic(v, 3)"; //no such relref as Magic
  std::string s10 = "variable x; assign a; Select a such that pattern a (\"x+1\", _)"; // left side invalid
  std::string s11 = "variable x; assign a; Select a such that pattern a (_, tough)"; // right side invalid
  std::string s12 = "variable x; assign a; Select a such that pattern a (_ _)"; // no comma
  std::string s13 = "variable x; Select x such that Uses(x 3)"; //no comma
  std::string s14 = "variable x; Select x such that Uses(\"something long\", 3)"; //no declaration of v
  std::string s15 = "variable x; assign a; Select x such that a (v, \"x\")"; //missing pattern
  std::string s16 = "variable x; Select x such that (x, 1)"; //missing relref
  
  vector<Clause> s1_output = qp.parsePQL(s1);
  vector<Clause> s2_output = qp.parsePQL(s2);
  vector<Clause> s3_output = qp.parsePQL(s3);
  vector<Clause> s4_output = qp.parsePQL(s4);
  vector<Clause> s5_output = qp.parsePQL(s5);
  vector<Clause> s6_output = qp.parsePQL(s6);
  vector<Clause> s7_output = qp.parsePQL(s7);
  vector<Clause> s8_output = qp.parsePQL(s8);
  vector<Clause> s9_output = qp.parsePQL(s9);
  vector<Clause> s10_output = qp.parsePQL(s10);
  vector<Clause> s11_output = qp.parsePQL(s11);
  vector<Clause> s12_output = qp.parsePQL(s12);
  vector<Clause> s13_output = qp.parsePQL(s13);
  vector<Clause> s14_output = qp.parsePQL(s14);
  vector<Clause> s15_output = qp.parsePQL(s15);
  vector<Clause> s16_output = qp.parsePQL(s16);

  SECTION("test"){
    REQUIRE(s1_output.empty());
    REQUIRE(s2_output.empty());
    REQUIRE(s3_output.empty());
    REQUIRE(s4_output.empty());
    REQUIRE(s5_output.empty());
    REQUIRE(s6_output.empty());
    REQUIRE(s7_output.empty());
    REQUIRE(s8_output.empty());
    REQUIRE(s9_output.empty());
    REQUIRE(s10_output.empty());
    REQUIRE(s11_output.empty());
    REQUIRE(s12_output.empty());
    REQUIRE(s13_output.empty());
    REQUIRE(s14_output.empty());
    REQUIRE(s15_output.empty());
    REQUIRE(s16_output.empty());
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