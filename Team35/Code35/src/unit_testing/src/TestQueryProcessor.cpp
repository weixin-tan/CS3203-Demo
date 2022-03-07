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
  std::string s3 = "stmt\ns;\nSelect\ns\nsuch\nthat\nFollows*(6,s)";
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
  std::string invalidWord1 = "magic v; Select v"; //invalid declaration
  std::string invalidWord2 = "variable v; Select v such that Magic(v, 3)"; //invalid rel ref
  std::string invalidWord3 = "variable v; assign a; Select v such that Magic a(v, _)"; //invalid pattern word
  std::string invalidWord4 = "variable v; assign a; Select v such that pattern Magic(v, _)"; //invalid pattern assignment

  std::string invalidEntity1 = "variable 2a, x; Select x"; // invalid name
  std::string invalidEntity2 = "variable x; Select 2a"; // invalid name
  std::string invalidEntity3 = "variable x; assign a; Select a such that pattern a (\"x+1\", _)"; // left side invalid
  std::string invalidEntity4 = "variable x; assign a; Select a such that pattern a (123, _)"; // left side invalid
  std::string invalidEntity5 = "variable x; assign a; Select a such that pattern a (_, v)"; // right side invalid
  std::string invalidEntity6 = "variable x; assign a; Select x such that Follows(\"x\", x)"; // left side invalid
  std::string invalidEntity7 = "variable x; assign a; Select x such that Parent*(1, \"x\")"; // right side invalid
  std::string invalidEntity8 = "variable x; assign a; Select x such that Uses(x, 1)"; // right side invalid

  std::string missingComma1 = "variable x Select x"; // missing semicolon
  std::string missingComma2 = "variable x, v a; Select x "; // missing comma
  std::string missingComma3 = "variable v x; Select x"; // missing comma
  std::string missingComma4 = "variable x; assign a; Select a such that pattern a (_ _)"; // no comma
  std::string missingComma5 = "variable x; Select x such that Uses(x 3)"; //no comma

  std::string missingKeyWords1 = "variable x; Select x such that Uses(v, 3)"; //no declaration of v
  std::string missingKeyWords2 = "variable x; assign a; Select x a (v, \"x\")"; //missing pattern
  std::string missingKeyWords3 = "variable x; assign a; Select x pattern "; //missing pattern stuff
  std::string missingKeyWords4 = "variable x; Select x such that (x, 1)"; //missing relref
  std::string missingKeyWords5 = "variable x; Select x Uses(x, 3)"; //missing such that
  std::string missingKeyWords6 = "variable x; x such that Uses(x, 4)"; //missing pattern
  std::string missingKeyWords7 = "variable x; Select x such that";

  std::string wronglyCapitalize1 = "Assign a; while w; Select a such that Parent* (w, a) pattern a (\"count\", _)";
  std::string wronglyCapitalize2 = "assign a; While w; Select a such that Parent* (w, a) pattern a (\"count\", _)";
  std::string wronglyCapitalize3 = "assign a; while w; select a such that Parent* (w, a) pattern a (\"count\", _)";
  std::string wronglyCapitalize4 = "assign a; while w; Select A such that Parent* (w, a) pattern a (\"count\", _)";
  std::string wronglyCapitalize5 = "assign a; while w; Select a Such that Parent* (w, a) pattern a (\"count\", _)";
  std::string wronglyCapitalize6 = "assign a; while w; Select a such That Parent* (w, a) pattern a (\"count\", _)";
  std::string wronglyCapitalize7 = "assign a; while w; Select a such that parent* (w, a) pattern a (\"count\", _)";
  std::string wronglyCapitalize8 = "assign a; while w; Select a such that Parent* (w, a) Pattern a (\"count\", _)";

  vector<Clause> invalidWordOutput1 = qp.parsePQL(invalidWord1);
  vector<Clause> invalidWordOutput2 = qp.parsePQL(invalidWord2);
  vector<Clause> invalidWordOutput3 = qp.parsePQL(invalidWord3);
  vector<Clause> invalidWordOutput4 = qp.parsePQL(invalidWord4);

  vector<Clause> invalidEntityOutput1 = qp.parsePQL(invalidEntity1);
  vector<Clause> invalidEntityOutput2 = qp.parsePQL(invalidEntity2);
  vector<Clause> invalidEntityOutput3 = qp.parsePQL(invalidEntity3);
  vector<Clause> invalidEntityOutput4 = qp.parsePQL(invalidEntity4);
  vector<Clause> invalidEntityOutput5 = qp.parsePQL(invalidEntity5);
  vector<Clause> invalidEntityOutput6 = qp.parsePQL(invalidEntity6);
  vector<Clause> invalidEntityOutput7 = qp.parsePQL(invalidEntity7);
  vector<Clause> invalidEntityOutput8 = qp.parsePQL(invalidEntity8);

  vector<Clause> missingCommaOutput1 = qp.parsePQL(missingComma1);
  vector<Clause> missingCommaOutput2 = qp.parsePQL(missingComma2);
  vector<Clause> missingCommaOutput3 = qp.parsePQL(missingComma3);
  vector<Clause> missingCommaOutput4 = qp.parsePQL(missingComma4);
  vector<Clause> missingCommaOutput5 = qp.parsePQL(missingComma5);

  vector<Clause> missingKeyWordsOutput1 = qp.parsePQL(missingKeyWords1);
  vector<Clause> missingKeyWordsOutput2 = qp.parsePQL(missingKeyWords2);
  vector<Clause> missingKeyWordsOutput3 = qp.parsePQL(missingKeyWords3);
  vector<Clause> missingKeyWordsOutput4 = qp.parsePQL(missingKeyWords4);
  vector<Clause> missingKeyWordsOutput5 = qp.parsePQL(missingKeyWords5);
  vector<Clause> missingKeyWordsOutput6 = qp.parsePQL(missingKeyWords6);
  vector<Clause> missingKeyWordsOutput7 = qp.parsePQL(missingKeyWords7);

  vector<Clause> wronglyCapitalizeOutput1 = qp.parsePQL(wronglyCapitalize1);
  vector<Clause> wronglyCapitalizeOutput2 = qp.parsePQL(wronglyCapitalize2);
  vector<Clause> wronglyCapitalizeOutput3 = qp.parsePQL(wronglyCapitalize3);
  vector<Clause> wronglyCapitalizeOutput4 = qp.parsePQL(wronglyCapitalize4);
  vector<Clause> wronglyCapitalizeOutput5 = qp.parsePQL(wronglyCapitalize5);
  vector<Clause> wronglyCapitalizeOutput6 = qp.parsePQL(wronglyCapitalize6);
  vector<Clause> wronglyCapitalizeOutput7 = qp.parsePQL(wronglyCapitalize7);
  vector<Clause> wronglyCapitalizeOutput8 = qp.parsePQL(wronglyCapitalize8);

  SECTION("Invalid Word"){
    REQUIRE(invalidWordOutput1.empty());
    REQUIRE(invalidWordOutput2.empty());
    REQUIRE(invalidWordOutput3.empty());
    REQUIRE(invalidWordOutput4.empty());
  }
  SECTION("Invalid Entity"){
    REQUIRE(invalidEntityOutput1.empty());
    REQUIRE(invalidEntityOutput2.empty());
    REQUIRE(invalidEntityOutput3.empty());
    REQUIRE(invalidEntityOutput4.empty());
    REQUIRE(invalidEntityOutput5.empty());
    REQUIRE(invalidEntityOutput6.empty());
    REQUIRE(invalidEntityOutput7.empty());
    REQUIRE(invalidEntityOutput8.empty());
  }
  SECTION("missing Commas"){
    REQUIRE(missingCommaOutput1.empty());
    REQUIRE(missingCommaOutput2.empty());
    REQUIRE(missingCommaOutput3.empty());
    REQUIRE(missingCommaOutput4.empty());
    REQUIRE(missingCommaOutput5.empty());
  }
  SECTION("missing KeyWords"){
    REQUIRE(missingKeyWordsOutput1.empty());
    REQUIRE(missingKeyWordsOutput2.empty());
    REQUIRE(missingKeyWordsOutput3.empty());
    REQUIRE(missingKeyWordsOutput4.empty());
    REQUIRE(missingKeyWordsOutput5.empty());
    REQUIRE(missingKeyWordsOutput6.empty());
    REQUIRE(missingKeyWordsOutput7.empty());
  }
  SECTION("wrongly capitalize"){
    REQUIRE(wronglyCapitalizeOutput1.empty());
    REQUIRE(wronglyCapitalizeOutput2.empty());
    REQUIRE(wronglyCapitalizeOutput3.empty());
    REQUIRE(wronglyCapitalizeOutput4.empty());
    REQUIRE(wronglyCapitalizeOutput5.empty());
    REQUIRE(wronglyCapitalizeOutput6.empty());
    REQUIRE(wronglyCapitalizeOutput7.empty());
    REQUIRE(wronglyCapitalizeOutput8.empty());
  }
}

TEST_CASE("trippy queries"){
  QueryProcessor qp = QueryProcessor();
  string s1 = "assign\tv;\tSelect\tv\tsuch\tthat\tParent*(v, 3)";
  string s2 = "assign a; Select a pattern a (\"\na\t\", _\"b\"_ )";
  string s3 = "assign a; Select a pattern a (\"a\", _\n\"b\"\t_ )";

  string s4 = "read read; while while; if if; variable variable; Select read such that Follows(while, if)";
  string s5 = "assign Uses; variable Modifies; Select Uses pattern Uses (Modifies, _)";
  string s6 = "stmt procName; constant value; Select procName with procName.stmt# = value.value";
  string s7 = "variable Select; Select Select such that Uses(_, Select)";
  string s8 = "stmt Next; Select Next such that Next (5, Next) and Next (Next, 12)";

  string s9 = "variable such; Select such such that Uses(_, such)";
  string s10 = "variable that; Select that such that Uses(_, that)";
  string s11 = "assign such; Select such pattern such(\"x\", _)";
  string s12 = "assign that; Select that pattern that(\"x\", _)";
  string s13 = "assign such, that; Select such with such.stmt# = that.stmt#";
  string s14 = "assign such, that; Select that with such.stmt# = that.stmt#";
  string s15 = "assign suchthat; Select suchthat such that Follows*(6,suchthat)";

  string s16 = "assign pattern; Select pattern pattern pattern (_, \"x\")";
  string s17 = "assign pattern; Select pattern pattern pattern (\"x\", _) and pattern(\"y\", _)";

  string s18 = "procedure with; Select with with with.procName = \"Third\"";

  string s19 = "variable and; Select and such that Uses(_,and) and Modifies(1,and)";
  string s20 = "stmt and; Select and such that Parent(and, 1) and Parent(and, 2)";
  string s21 = "assign and; Select and pattern and (\"x\", _) and and (\"y\", _)";
  string s22 = "assign and; Select and pattern and (_,\"x\") and and (_, \"y\")";
  string s23 = "procedure and; call c, c1; Select and with and.procName = c.procName and and.procName = c1.procName";

  SECTION("test that trippy queries are valid"){
    REQUIRE(!qp.parsePQL(s1).empty());
    REQUIRE(!qp.parsePQL(s2).empty());
    REQUIRE(!qp.parsePQL(s3).empty());
    REQUIRE(!qp.parsePQL(s4).empty());
    REQUIRE(!qp.parsePQL(s5).empty());
    REQUIRE(!qp.parsePQL(s6).empty());
    REQUIRE(!qp.parsePQL(s7).empty());
    REQUIRE(!qp.parsePQL(s8).empty());
    REQUIRE(!qp.parsePQL(s9).empty());
    REQUIRE(!qp.parsePQL(s10).empty());
    REQUIRE(!qp.parsePQL(s11).empty());
    REQUIRE(!qp.parsePQL(s12).empty());
    REQUIRE(!qp.parsePQL(s13).empty());
    REQUIRE(!qp.parsePQL(s14).empty());
    REQUIRE(!qp.parsePQL(s15).empty());
    REQUIRE(!qp.parsePQL(s16).empty());
    REQUIRE(!qp.parsePQL(s17).empty());
    REQUIRE(!qp.parsePQL(s18).empty());
    REQUIRE(!qp.parsePQL(s19).empty());
    REQUIRE(!qp.parsePQL(s20).empty());
    REQUIRE(!qp.parsePQL(s21).empty());
    REQUIRE(!qp.parsePQL(s22).empty());
    REQUIRE(!qp.parsePQL(s23).empty());
  }
}

TEST_CASE("test advanced queries without with clauses"){
  QueryProcessor qp = QueryProcessor();
  string s1 = "\nSelect       BOOLEAN\t\t\t\tsuch\n\n\n\nthat     Next* (\t1\t,\t2\t)\n\n";
  string s2 = "assign a1, a2;Select <a1, a2.stmt#, BOOLEAN> such that Affects (a1, a2)";
  string s3 = "procedure p; call c; Select c.procName with c.procName = p.procName";
  string s4 = "while w; if ifs; Select w pattern w (\"x\", _) and ifs (\"x\", _)";

  Clause c1 = Clause();
  Clause c2 = Clause();
  Clause c3 = Clause();
  Clause c4 = Clause();
  Clause s1_output = qp.parsePQL(s1)[0];
  Clause s2_output = qp.parsePQL(s2)[0];
  Clause s3_output = qp.parsePQL(s3)[0];
  Clause s4_output = qp.parsePQL(s4)[0];

  Entity a1 = Entity(EntityType::Assignment, "a1");
  Entity a2 = Entity(EntityType::Assignment, "a2");
  Entity a2stmt = Entity(EntityType::Assignment, "a2", EntityAttributeType::Stmt);
  Entity ifEntity = Entity(EntityType::If, "ifs");
  Entity whileEntity = Entity(EntityType::While, "w");
  Entity wildcard = Entity(EntityType::Wildcard, "_");

  Entity boolean = Entity(EntityType::Boolean, "BOOLEAN");
  Entity int1 = Entity(EntityType::FixedInteger, "1");
  Entity int2 = Entity(EntityType::FixedInteger, "2");
  Entity x = Entity(EntityType::FixedString, "x");

  Entity cProcname = Entity(EntityType::Call, "c", EntityAttributeType::ProcName);
  Entity pProcname = Entity(EntityType::Procedure, "p", EntityAttributeType::ProcName);


  RelationshipRef r1 = RelationshipRef(RelationshipType::NextT, int1, int2);
  RelationshipRef r2 = RelationshipRef(RelationshipType::Affects, a1, a2);
  RelationshipRef r3 = RelationshipRef(RelationshipType::With, cProcname, pProcname);
  RelationshipRef r4a = RelationshipRef(RelationshipType::Pattern, x, wildcard, whileEntity);
  RelationshipRef r4b = RelationshipRef(RelationshipType::Pattern, x, wildcard, ifEntity);

  SECTION("no mixed clauses query"){
    c1.appendEntityToFind(boolean);
    c1.appendRef(r1);
    REQUIRE(s1_output.equals(c1));

    c2.appendEntityToFind(a1);
    c2.appendEntityToFind(a2stmt);
    c2.appendEntityToFind(boolean);
    c2.appendRef(r2);
    REQUIRE(s2_output.equals(c2));

    c3.appendEntityToFind(cProcname);
    c3.appendRef(r3);
    REQUIRE(s3_output.equals(c3));

    c4.appendEntityToFind(whileEntity);
    c4.appendRef(r4a);
    c4.appendRef(r4b);
    REQUIRE(s4_output.equals(c4));
  }

  string s9 = "assign a; while w;Select a such that Modifies (a, \"x\") and Parent* (w, a) and Next* (1, a)";
  string s10 = "stmt s;Select s such that Next* (5, s) and Next* (s, 12) such that Follows(1,2)";

  string s11 = "assign a; Select a pattern a (\"\na\t\", _\"b\"_ ) and a (\"x\",_)";
  string s12 = "assign a; Select a pattern a (\"\na\t\", _\"b\"_ ) pattern a (\"x\",_)";

  string s13 = "assign a; while w;Select a pattern a (\"x\", _) such that Parent* (w, a) and Next* (1, a)";
  string s14 = "assign a1, a2; while w1, w2;Select a2 pattern a1 (\"x\", _) and a2 (\"x\", _\"x\"_) such that Affects (a1, a2) and Parent* (w2, a2) and Parent* (w1, w2)";

  qp.parsePQL(s14);
}

TEST_CASE("debugging"){
  QueryProcessor qp = QueryProcessor();
  Clause c = qp.parsePQL(  "stmt Next; Select Next such that Next (5, Next) and Next (Next, 12)")[0];
  //cout << c.toString() << "\n";
}
/*
METHODS TO TEST
static void createDeclarationObjects(std::vector<std::string> designEntityArr, std::unordered_map<std::string, Entity>* entityMap);
static RelationshipRef createRelationshipObject(std::vector<std::string> relRefList, std::unordered_map<std::string, Entity>* entityMap);
static Entity findRelationshipEntity(const std::string& s, std::unordered_map<std::string, Entity>* entityMap);
static RelationshipRef createPatternObject(std::vector<std::string> patternList, std::unordered_map<std::string, Entity>* entityMap);
*/