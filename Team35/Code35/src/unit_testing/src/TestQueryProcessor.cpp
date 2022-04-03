#include <vector>
#include <iostream>

#include "QPS/Clause.h"
#include "catch.hpp"
#include "QPS/QueryProcessor.h"

using namespace std;

TEST_CASE("testing valid queries on parseQuery") {
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

    Entity a = Entity(EntityType::ASSIGNMENT, "a");
    Entity s = Entity(EntityType::STATEMENT, "s");
    Entity v = Entity(EntityType::VARIABLE, "v");
    Entity w = Entity(EntityType::WHILE, "w");
    Entity countEntity = Entity(EntityType::FIXED_STRING, "count");
    Entity fixedInt = Entity(EntityType::FIXED_INTEGER, "6");
    Entity wildcardEntity = Entity(EntityType::WILDCARD, "_");
    Entity xEntity = Entity(EntityType::FIXED_STRING, "x");
    Entity countplus1Entity = Entity(EntityType::FIXED_STRING_WITHIN_WILDCARD, "count + 1");

    RelationshipRef r3 = RelationshipRef(RelationshipType::FOLLOWS_T, fixedInt, s);
    RelationshipRef r4 = RelationshipRef(RelationshipType::MODIFIES, fixedInt, v);
    RelationshipRef r5 = RelationshipRef(RelationshipType::PARENT_T, w, a);
    RelationshipRef r6 = RelationshipRef(RelationshipType::PATTERN, wildcardEntity, countplus1Entity, a);
    RelationshipRef r7 = RelationshipRef(RelationshipType::PATTERN, xEntity, countplus1Entity, a);
    RelationshipRef r8_a = RelationshipRef(RelationshipType::USES, a, v);
    RelationshipRef r8_b = RelationshipRef(RelationshipType::PATTERN, v, wildcardEntity, a);
    RelationshipRef r9_a = RelationshipRef(RelationshipType::PARENT_T, w, a);
    RelationshipRef r9_b = RelationshipRef(RelationshipType::PATTERN, countEntity, wildcardEntity, a);
    RelationshipRef r10_a = RelationshipRef(RelationshipType::PATTERN, xEntity, wildcardEntity, a);
    RelationshipRef r10_b = RelationshipRef(RelationshipType::USES, a, xEntity);
    RelationshipRef r11_a = RelationshipRef(RelationshipType::PARENT_T, w, a);
    RelationshipRef r11_b = RelationshipRef(RelationshipType::PATTERN, countEntity, wildcardEntity, a);

    std::string s1 = "while w;Select w";
    std::string s2 = "variable      v     ; Select     v      ";
    std::string s3 = "stmt\ns;\nSelect\ns\nsuch\nthat\nFollows*(6,s)";
    std::string s4 = "    variable     v     ;    Select     v     such    that         Modifies (    6   ,   v   )  ";
    std::string s5 = "assign a;while w;Select a such that Parent*(w,a)";
    std::string s6 = "assign a; Select a pattern a ( _ , _\"count + 1\"_)";
    std::string s7 = "assign a; Select a pattern a ( \"x\" ,     _\"count + 1\"_      )";
    std::string s8 =
            "assign    a    ;    variable   v  ;   Select    a    such    that    Uses   (   a  ,  v   ) pattern a ( v ,  _   )";
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

    SECTION("parse query with 1 such-that clause") {
        c1.appendEntityToFind(w);
        c2.appendEntityToFind(v);
        REQUIRE((c1 == s1_output));
        REQUIRE((c2 == s2_output));
    }

    SECTION("parse query with 1 pattern clause") {
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

    SECTION("parse query with 1 pattern and 1 such-that clause") {
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

TEST_CASE("invalid querys") {
    QueryProcessor qp = QueryProcessor();
    std::string invalidWord1 = "magic v; Select v"; //invalid declaration
    std::string invalidWord2 = "variable v; Select v such that Magic(v, 3)"; //invalid rel ref
    std::string invalidWord3 = "variable v; assign a; Select v such that Magic a(v, _)"; //invalid pattern word
    std::string
            invalidWord4 = "variable v; assign a; Select v such that pattern Magic(v, _)"; //invalid pattern assignment
    std::string invalidWord5 = "variable v; assign a; Select v Magic v.varName = \"hello\""; //invalid with word

    std::string invalidEntity1 = "variable 2a, x; Select x"; // invalid name
    std::string invalidEntity2 = "variable x; Select 2a"; // invalid name
    std::string invalidEntity3 = "variable x; assign a; Select a such that pattern a (\"x+1\", _)"; // left side invalid
    std::string invalidEntity4 = "variable x; assign a; Select a such that pattern a (123, _)"; // left side invalid
    std::string invalidEntity5 = "variable x; assign a; Select a such that pattern a (_, v)"; // right side invalid
    std::string invalidEntity6 = "variable x; assign a; Select x such that Follows(\"x\", x)"; // left side invalid
    std::string invalidEntity7 = "variable x; assign a; Select x such that Parent*(1, \"x\")"; // right side invalid
    std::string invalidEntity8 = "variable x; assign a; Select x such that Uses(x, 1)"; // right side invalid
    std::string invalidEntity9 = "procedure p; variable v; Select p with p.testing = v.varName"; //left side invalid
    std::string invalidEntity10 = "procedure p; variable v; Select p with p.procName = v.testing"; //right side invalid
    std::string invalidEntity11 = "procedure p; variable v; Select p with p.varName = v.procName"; //not valid attribute

    std::string missingComma1 = "variable x Select x"; // missing semicolon
    std::string missingComma2 = "variable x, v a; Select x "; // missing comma
    std::string missingComma3 = "variable v x; Select x"; // missing comma
    std::string missingComma4 = "variable x; assign a; Select a pattern a (_ _)"; // no comma
    std::string missingComma5 = "while w; Select w pattern w (\"x\" _)"; // no comma
    std::string missingComma6 = "if ifs; Select ifs pattern ifs (\"x\",_ _)";
    std::string missingComma7 = "if ifs; Select ifs pattern ifs (\"x\" _ , _)";
    std::string missingComma8 = "variable x; Select x such that Uses(x 3)"; //no comma

    std::string missingKeyWords1 = "variable x; Select x such that Uses(v, 3)"; //no declaration of v
    std::string missingKeyWords2 = "variable x; assign a; Select x a (x, \"x\")"; //missing pattern
    std::string missingKeyWords3 = "variable x; assign a; Select x pattern "; //missing pattern stuff
    std::string missingKeyWords4 = "variable x; Select x such that (x, 1)"; //missing relref
    std::string missingKeyWords5 = "variable x; Select x Uses(x, 3)"; //missing such that
    std::string missingKeyWords6 = "variable x; x such that Uses(x, 4)"; //missing select
    std::string missingKeyWords7 = "variable x; Select x such that"; //missing relref and entities
    std::string missingKeyWords8 = "variable x; Select x.varName with x.varName = "; //missing right side
    std::string missingKeyWords9 = "variable x; Select x.varName with = x.varName"; //missing left side
    std::string missingKeyWords10 = "variable x; Select x.varName \"hello\" = x.varName"; //missing with
    std::string missingKeyWords11 = "variable v; Select v such that Uses(v, 3) pattern "; //no pattern clause
    std::string missingKeyWords12 = "variable v; Select v such that Uses(v, 3) with "; //no with clause
    std::string missingKeyWords13 = "variable v; Select v such that Uses(v, 3) and "; //no such that clause
    std::string missingKeyWords14 = "assign a; Select a pattern a (_,_) and";//no pattern clause
    std::string missingKeyWords15 = "assign a; Select a pattern a (_,_) with ";//no with clause
    std::string missingKeyWords16 = "assign a; Select a pattern a (_,_) such that";//no such that clause
    std::string missingKeyWords17 = "assign a; Select a with a.stmt# = 3 pattern"; //no pattern clause
    std::string missingKeyWords18 = "assign a; Select a with a.stmt# = 3 and"; //no with clause
    std::string missingKeyWords19 = "assign a; Select a with a.stmt# = 3 such that"; //no such that

    std::string wronglyCapitalize1 = "Assign a; while w; Select a such that Parent* (w, a) pattern a (\"count\", _)";
    std::string wronglyCapitalize2 = "assign a; WHILE w; Select a such that Parent* (w, a) pattern a (\"count\", _)";
    std::string wronglyCapitalize3 = "assign a; while w; select a such that Parent* (w, a) pattern a (\"count\", _)";
    std::string wronglyCapitalize4 = "assign a; while w; Select A such that Parent* (w, a) pattern a (\"count\", _)";
    std::string wronglyCapitalize5 = "assign a; while w; Select a Such that Parent* (w, a) pattern a (\"count\", _)";
    std::string wronglyCapitalize6 = "assign a; while w; Select a such That Parent* (w, a) pattern a (\"count\", _)";
    std::string wronglyCapitalize7 = "assign a; while w; Select a such that parent* (w, a) pattern a (\"count\", _)";
    std::string wronglyCapitalize8 = "assign a; while w; Select a such that Parent* (w, a) PATTERN a (\"count\", _)";
    std::string wronglyCapitalize9 = "PROCEDURE p; variable v; Select p with p.procName = v.varName";
    std::string wronglyCapitalize10 = "procedure p; VARIABLE v; Select p with p.procName = v.varName";
    std::string wronglyCapitalize11 = "procedure p; variable v; select p with p.procName = v.varName";
    std::string wronglyCapitalize12 = "procedure p; variable v; Select P with p.procName = v.varName";
    std::string wronglyCapitalize13 = "procedure p; variable v; Select p WITH p.procName = v.varName";
    std::string wronglyCapitalize14 = "procedure p; variable v; Select p with P.procName = v.varName";
    std::string wronglyCapitalize15 = "procedure p; variable v; Select p with p.PROCNAME = v.varName";
    std::string wronglyCapitalize16 = "procedure p; variable v; Select p with p.procName = V.varName";
    std::string wronglyCapitalize17 = "procedure p; variable v; Select p with P.procName = v.VARNAME";

    std::string patternIf1 = "if ifs; while w; Select ifs pattern ifs(\"a\",_)";
    std::string patternIf2 = "if ifs; while w; Select ifs pattern ifs(\"a\",_,)";
    std::string patternIf3 = "if ifs; while w; Select ifs pattern ifs(\"a\",1,_)";
    std::string patternIf4 = "if ifs; while w; Select ifs pattern ifs(\"a\",ifs,_)";
    std::string patternIf5 = "if ifs; while w; Select ifs pattern ifs(\"a\",_,1)";
    std::string patternIf6 = "if ifs; while w; Select ifs pattern ifs(\"a\",_,ifs)";

    std::string boolean1 = "Select BOOLEAN.stmt#";
    std::string boolean2 = "Select BOOLEAN.varName";
    std::string boolean3 = "Select Boolean";
    std::string boolean4 = "Select BOOLEAN with \"true\"=BOOLEAN";
    std::string boolean5 = "Select BOOLEAN such that Uses(BOOLEAN, \"v\")";
    std::string boolean6 = "Select BOOLEAN with pattern BOOLEAN (_,_)";

    vector<Clause> invalidWordOutput1 = qp.parsePQL(invalidWord1);
    vector<Clause> invalidWordOutput2 = qp.parsePQL(invalidWord2);
    vector<Clause> invalidWordOutput3 = qp.parsePQL(invalidWord3);
    vector<Clause> invalidWordOutput4 = qp.parsePQL(invalidWord4);
    vector<Clause> invalidWordOutput5 = qp.parsePQL(invalidWord5);

    vector<Clause> invalidEntityOutput1 = qp.parsePQL(invalidEntity1);
    vector<Clause> invalidEntityOutput2 = qp.parsePQL(invalidEntity2);
    vector<Clause> invalidEntityOutput3 = qp.parsePQL(invalidEntity3);
    vector<Clause> invalidEntityOutput4 = qp.parsePQL(invalidEntity4);
    vector<Clause> invalidEntityOutput5 = qp.parsePQL(invalidEntity5);
    vector<Clause> invalidEntityOutput6 = qp.parsePQL(invalidEntity6);
    vector<Clause> invalidEntityOutput7 = qp.parsePQL(invalidEntity7);
    vector<Clause> invalidEntityOutput8 = qp.parsePQL(invalidEntity8);
    vector<Clause> invalidEntityOutput9 = qp.parsePQL(invalidEntity9);
    vector<Clause> invalidEntityOutput10 = qp.parsePQL(invalidEntity10);
    vector<Clause> invalidEntityOutput11 = qp.parsePQL(invalidEntity11);

    vector<Clause> missingCommaOutput1 = qp.parsePQL(missingComma1);
    vector<Clause> missingCommaOutput2 = qp.parsePQL(missingComma2);
    vector<Clause> missingCommaOutput3 = qp.parsePQL(missingComma3);
    vector<Clause> missingCommaOutput4 = qp.parsePQL(missingComma4);
    vector<Clause> missingCommaOutput5 = qp.parsePQL(missingComma5);
    vector<Clause> missingCommaOutput6 = qp.parsePQL(missingComma3);
    vector<Clause> missingCommaOutput7 = qp.parsePQL(missingComma4);
    vector<Clause> missingCommaOutput8 = qp.parsePQL(missingComma5);

    vector<Clause> missingKeyWordsOutput1 = qp.parsePQL(missingKeyWords1);
    vector<Clause> missingKeyWordsOutput2 = qp.parsePQL(missingKeyWords2);
    vector<Clause> missingKeyWordsOutput3 = qp.parsePQL(missingKeyWords3);
    vector<Clause> missingKeyWordsOutput4 = qp.parsePQL(missingKeyWords4);
    vector<Clause> missingKeyWordsOutput5 = qp.parsePQL(missingKeyWords5);
    vector<Clause> missingKeyWordsOutput6 = qp.parsePQL(missingKeyWords6);
    vector<Clause> missingKeyWordsOutput7 = qp.parsePQL(missingKeyWords7);
    vector<Clause> missingKeyWordsOutput8 = qp.parsePQL(missingKeyWords8);
    vector<Clause> missingKeyWordsOutput9 = qp.parsePQL(missingKeyWords9);
    vector<Clause> missingKeyWordsOutput10 = qp.parsePQL(missingKeyWords10);
    vector<Clause> missingKeyWordsOutput11 = qp.parsePQL(missingKeyWords11);
    vector<Clause> missingKeyWordsOutput12 = qp.parsePQL(missingKeyWords12);
    vector<Clause> missingKeyWordsOutput13 = qp.parsePQL(missingKeyWords13);
    vector<Clause> missingKeyWordsOutput14 = qp.parsePQL(missingKeyWords14);
    vector<Clause> missingKeyWordsOutput15 = qp.parsePQL(missingKeyWords15);
    vector<Clause> missingKeyWordsOutput16 = qp.parsePQL(missingKeyWords16);
    vector<Clause> missingKeyWordsOutput17 = qp.parsePQL(missingKeyWords17);

    vector<Clause> wronglyCapitalizeOutput1 = qp.parsePQL(wronglyCapitalize1);
    vector<Clause> wronglyCapitalizeOutput2 = qp.parsePQL(wronglyCapitalize2);
    vector<Clause> wronglyCapitalizeOutput3 = qp.parsePQL(wronglyCapitalize3);
    vector<Clause> wronglyCapitalizeOutput4 = qp.parsePQL(wronglyCapitalize4);
    vector<Clause> wronglyCapitalizeOutput5 = qp.parsePQL(wronglyCapitalize5);
    vector<Clause> wronglyCapitalizeOutput6 = qp.parsePQL(wronglyCapitalize6);
    vector<Clause> wronglyCapitalizeOutput7 = qp.parsePQL(wronglyCapitalize7);
    vector<Clause> wronglyCapitalizeOutput8 = qp.parsePQL(wronglyCapitalize8);
    vector<Clause> wronglyCapitalizeOutput9 = qp.parsePQL(wronglyCapitalize9);
    vector<Clause> wronglyCapitalizeOutput10 = qp.parsePQL(wronglyCapitalize10);
    vector<Clause> wronglyCapitalizeOutput11 = qp.parsePQL(wronglyCapitalize11);
    vector<Clause> wronglyCapitalizeOutput12 = qp.parsePQL(wronglyCapitalize12);
    vector<Clause> wronglyCapitalizeOutput13 = qp.parsePQL(wronglyCapitalize13);
    vector<Clause> wronglyCapitalizeOutput14 = qp.parsePQL(wronglyCapitalize14);
    vector<Clause> wronglyCapitalizeOutput15 = qp.parsePQL(wronglyCapitalize15);
    vector<Clause> wronglyCapitalizeOutput16 = qp.parsePQL(wronglyCapitalize16);
    vector<Clause> wronglyCapitalizeOutput17 = qp.parsePQL(wronglyCapitalize17);

    vector<Clause> patternIfOutput1 = qp.parsePQL(patternIf1);
    vector<Clause> patternIfOutput2 = qp.parsePQL(patternIf2);
    vector<Clause> patternIfOutput3 = qp.parsePQL(patternIf3);
    vector<Clause> patternIfOutput4 = qp.parsePQL(patternIf4);
    vector<Clause> patternIfOutput5 = qp.parsePQL(patternIf5);
    vector<Clause> patternIfOutput6 = qp.parsePQL(patternIf6);

    vector<Clause> booleanOutput1 = qp.parsePQL(boolean1);
    vector<Clause> booleanOutput2 = qp.parsePQL(boolean2);
    vector<Clause> booleanOutput3 = qp.parsePQL(boolean3);
    vector<Clause> booleanOutput4 = qp.parsePQL(boolean1);
    vector<Clause> booleanOutput5 = qp.parsePQL(boolean2);
    vector<Clause> booleanOutput6 = qp.parsePQL(boolean3);

    SECTION("Invalid Word") {
        REQUIRE(invalidWordOutput1.empty());
        REQUIRE(invalidWordOutput2.empty());
        REQUIRE(invalidWordOutput3.empty());
        REQUIRE(invalidWordOutput4.empty());
        REQUIRE(invalidWordOutput5.empty());
    }SECTION("Invalid Entity") {
        REQUIRE(invalidEntityOutput1.empty());
        REQUIRE(invalidEntityOutput2.empty());
        REQUIRE(invalidEntityOutput3.empty());
        REQUIRE(invalidEntityOutput4.empty());
        REQUIRE(invalidEntityOutput5.empty());
        REQUIRE(invalidEntityOutput6.empty());
        REQUIRE(invalidEntityOutput7.empty());
        REQUIRE(invalidEntityOutput8.empty());
        REQUIRE(invalidEntityOutput9.empty());
        REQUIRE(invalidEntityOutput10.empty());
        REQUIRE(invalidEntityOutput11.empty());
    }SECTION("missing Commas") {
        REQUIRE(missingCommaOutput1.empty());
        REQUIRE(missingCommaOutput2.empty());
        REQUIRE(missingCommaOutput3.empty());
        REQUIRE(missingCommaOutput4.empty());
        REQUIRE(missingCommaOutput5.empty());
        REQUIRE(missingCommaOutput6.empty());
        REQUIRE(missingCommaOutput7.empty());
        REQUIRE(missingCommaOutput8.empty());
    }SECTION("missing KeyWords") {
        REQUIRE(missingKeyWordsOutput1.empty());
        REQUIRE(missingKeyWordsOutput2.empty());
        REQUIRE(missingKeyWordsOutput3.empty());
        REQUIRE(missingKeyWordsOutput4.empty());
        REQUIRE(missingKeyWordsOutput5.empty());
        REQUIRE(missingKeyWordsOutput6.empty());
        REQUIRE(missingKeyWordsOutput7.empty());
        REQUIRE(missingKeyWordsOutput8.empty());
        REQUIRE(missingKeyWordsOutput9.empty());
        REQUIRE(missingKeyWordsOutput10.empty());
        REQUIRE(missingKeyWordsOutput11.empty());
        REQUIRE(missingKeyWordsOutput12.empty());
        REQUIRE(missingKeyWordsOutput13.empty());
        REQUIRE(missingKeyWordsOutput14.empty());
        REQUIRE(missingKeyWordsOutput15.empty());
        REQUIRE(missingKeyWordsOutput16.empty());
        REQUIRE(missingKeyWordsOutput17.empty());
    }SECTION("wrongly capitalize") {
        REQUIRE(wronglyCapitalizeOutput1.empty());
        REQUIRE(wronglyCapitalizeOutput2.empty());
        REQUIRE(wronglyCapitalizeOutput3.empty());
        REQUIRE(wronglyCapitalizeOutput4.empty());
        REQUIRE(wronglyCapitalizeOutput5.empty());
        REQUIRE(wronglyCapitalizeOutput6.empty());
        REQUIRE(wronglyCapitalizeOutput7.empty());
        REQUIRE(wronglyCapitalizeOutput8.empty());
        REQUIRE(wronglyCapitalizeOutput9.empty());
        REQUIRE(wronglyCapitalizeOutput10.empty());
        REQUIRE(wronglyCapitalizeOutput11.empty());
        REQUIRE(wronglyCapitalizeOutput12.empty());
        REQUIRE(wronglyCapitalizeOutput13.empty());
        REQUIRE(wronglyCapitalizeOutput14.empty());
        REQUIRE(wronglyCapitalizeOutput15.empty());
        REQUIRE(wronglyCapitalizeOutput16.empty());
        REQUIRE(wronglyCapitalizeOutput17.empty());
    }

    SECTION("something in pattern if is wrong") {
        REQUIRE(patternIfOutput1.empty());
        REQUIRE(patternIfOutput2.empty());
        REQUIRE(patternIfOutput3.empty());
        REQUIRE(patternIfOutput4.empty());
        REQUIRE(patternIfOutput5.empty());
        REQUIRE(patternIfOutput6.empty());
    }

    SECTION("invalid boolean queries") {
        REQUIRE(booleanOutput1.empty());
        REQUIRE(booleanOutput2.empty());
        REQUIRE(booleanOutput3.empty());
        REQUIRE(booleanOutput4.empty());
        REQUIRE(booleanOutput5.empty());
        REQUIRE(booleanOutput6.empty());
    }
}

TEST_CASE("trippy queries") {
    QueryProcessor qp = QueryProcessor();
    string s1 = "assign\tv;\tSelect\tv\tsuch\tthat\tParent*(v, 3)";
    string s2 = "assign a; Select a pattern a (\"\na\t\", _\"b\"_ )";
    string s3 = "assign a; Select a pattern a (\"a\", _\n\"b\"\t_ )";

    string s4 = "read read; while while; if if; variable variable; Select read such that Follows(while, if)";
    string s5 = "assign Uses; variable Modifies; Select Uses pattern Uses (Modifies, _)";
    string s6 = "stmt procName; constant value; Select procName with procName.stmt# = value.value";
    string s7 = "assign a; variable Select; Select Select such that Uses(a, Select)";
    string s8 = "stmt Next; Select Next such that Next (5, Next) and Next (Next, 12)";

    string s9 = "assign a; variable such; Select such such that Uses(a, such)";
    string s10 = "assign a; variable that; Select that such that Uses(a, that)";
    string s11 = "assign such; Select such pattern such(\"x\", _)";
    string s12 = "assign that; Select that pattern that(\"x\", _)";
    string s13 = "assign such, that; Select such with such.stmt# = that.stmt#";
    string s14 = "assign such, that; Select that with such.stmt# = that.stmt#";
    string s15 = "assign suchthat; Select suchthat such that Follows*(6,suchthat)";

    string s16 = "assign pattern; Select pattern pattern pattern (_, \"x\")";
    string s17 = "assign pattern; Select pattern pattern pattern (\"x\", _) and pattern(\"y\", _)";
    string s18 = "procedure with; Select with with with.procName = \"Third\"";

    string s19 = "variable and; Select and such that Uses(1,and) and Modifies(1,and)";
    string s20 = "stmt and; Select and such that Parent(and, 1) and Parent(and, 2)";
    string s21 = "assign and; Select and pattern and (\"x\", _) and and (\"y\", _)";
    string s22 = "assign and; Select and pattern and (_,\"x\") and and (_, \"y\")";
    string s23 = "procedure and; call c, c1; Select and with and.procName = c.procName and and.procName = c1.procName";

    string s24 =
            "assign and; while while; Select and such that Parent* (while, and) with while.stmt# = and.stmt# pattern and (\"x\", _)";
    string s25 =
            "assign pattern; while with; Select pattern such that Parent* (with, pattern) with with.stmt# = pattern.stmt# and pattern.stmt# = 1 pattern pattern (\"pattern\", _) ";
    string s26 =
            "assign such; while that; Select such such that Parent* (that, such) with that.stmt# = such.stmt# pattern such (\"x\", _)";

    string s27 =
            "assign Select; Select Select with Select.stmt# = 12 and Select.stmt# = 13 pattern Select (\"Select\", _)";
    string s28 = "assign and; Select and with and.stmt# = 12 and and.stmt# = 13 pattern and (\"and\", _)";
    string s29 = "assign such; Select such with such.stmt# = 12 and such.stmt# = 13 pattern such (\"such\", _)";
    string s30 = "assign that; Select that with that.stmt# = 12 and that.stmt# = 13 pattern that (\"that\", _)";
    string s31 =
            "assign pattern; Select pattern with pattern.stmt# = 12 and pattern.stmt# = 13 pattern pattern (\"pattern\", _)";
    string s32 = "assign with; Select with with with.stmt# = 12 and with.stmt# = 13 pattern with (\"with\", _)";

    string s33 = "stmt such;Select such with such.stmt# = 12 such that Next* (1, such) and Next* (such, 3)";
    string s34 = "stmt Select;Select Select with Select.stmt# = 12 such that Next* (1, Select) and Next* (Select, 3)";
    string s35 = "stmt and;Select and with and.stmt# = 12 such that Next* (1, and) and Next* (and, 3)";
    string s36 = "stmt that;Select that with that.stmt# = 12 such that Next* (1, that) and Next* (that, 3)";
    string s37 =
            "stmt pattern;Select pattern with pattern.stmt# = 12 such that Next* (1, pattern) and Next* (pattern, 3)";
    string s38 = "stmt with;Select with with with.stmt# = 12 such that Next* (1, with) and Next* (with, 3)";

    SECTION("test that trippy queries are valid") {
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
        REQUIRE(!qp.parsePQL(s24).empty());
        REQUIRE(!qp.parsePQL(s25).empty());
        REQUIRE(!qp.parsePQL(s26).empty());
        REQUIRE(!qp.parsePQL(s27).empty());
        REQUIRE(!qp.parsePQL(s28).empty());
        REQUIRE(!qp.parsePQL(s29).empty());
        REQUIRE(!qp.parsePQL(s30).empty());
        REQUIRE(!qp.parsePQL(s31).empty());
        REQUIRE(!qp.parsePQL(s32).empty());
        REQUIRE(!qp.parsePQL(s33).empty());
        REQUIRE(!qp.parsePQL(s34).empty());
        REQUIRE(!qp.parsePQL(s35).empty());
        REQUIRE(!qp.parsePQL(s36).empty());
        REQUIRE(!qp.parsePQL(s37).empty());
        REQUIRE(!qp.parsePQL(s38).empty());
    }
}

TEST_CASE("test advanced queries") {
    QueryProcessor qp = QueryProcessor();

    Entity a = Entity(EntityType::ASSIGNMENT, "a");
    Entity astmt = Entity(EntityType::ASSIGNMENT, "a", EntityAttributeType::STMT);
    Entity a1 = Entity(EntityType::ASSIGNMENT, "a1");
    Entity a1stmt = Entity(EntityType::ASSIGNMENT, "a1", EntityAttributeType::STMT);
    Entity a2 = Entity(EntityType::ASSIGNMENT, "a2");
    Entity a2stmt = Entity(EntityType::ASSIGNMENT, "a2", EntityAttributeType::STMT);
    Entity ifEntity = Entity(EntityType::IF, "ifs");
    Entity s = Entity(EntityType::STATEMENT, "s");
    Entity whileEntity = Entity(EntityType::WHILE, "w");
    Entity wildcard = Entity(EntityType::WILDCARD, "_");
    Entity boolean = Entity(EntityType::BOOLEAN, "BOOLEAN");
    Entity int1 = Entity(EntityType::FIXED_INTEGER, "1");
    Entity int2 = Entity(EntityType::FIXED_INTEGER, "2");
    Entity int3 = Entity(EntityType::FIXED_INTEGER, "3");
    Entity x = Entity(EntityType::FIXED_STRING, "x");
    Entity patternString = Entity(EntityType::FIXED_STRING, "pattern");
    Entity cProcname = Entity(EntityType::CALL, "c", EntityAttributeType::PROCNAME);
    Entity pProcname = Entity(EntityType::PROCEDURE, "p", EntityAttributeType::PROCNAME);

    string s1 = "\nSelect       BOOLEAN\t\t\t\tsuch\n\n\n\nthat     Next* (\t1\t,\t2\t)\n\n";
    string s2 = "assign a1, a2;Select <a1, a2\n.\nstmt#> such that Affects (a1, a2)";
    string s3 = "procedure p; call c; Select c.procName with c\t.\tprocName = p    .    procName";
    string s4 = "while w; if ifs; Select w pattern w (\"x\", _) and ifs (\"x\", _, _)";
    string s5 =
            "stmt s;Select s such that Next*\n(\n1\n,\ns\n) such that Next*\t(\ts\t,\t3\t)\tsuch that Follows*(1,3)";
    string s6 = "stmt s;Select s such that Next* (1, s) and Next* (s, 3) such that Follows*(1,3)";
    string s7 = "stmt s;Select s such that Next* (1, s) and Next* (s, 3) and Follows*(1,3)";
    string s8 =
            "assign a; while w; if ifs; Select a pattern a (\"pattern\", _) pattern w (\"pattern\", _) pattern ifs (\"pattern\", _, _)";
    string s9 =
            "assign a; while w; if ifs; Select a pattern a (\"pattern\", _) and w (\"pattern\", _) pattern ifs (\"pattern\", _, _)";
    string s10 =
            "assign a; while w; if ifs; Select a pattern a (\"pattern\", _) and w (\"pattern\", _) and ifs (\"pattern\", _, _)";
    string s11 = "assign a, a1; Select a1 with a.stmt# = 1 with a1.stmt# = 3 with 1 = 3";
    string s12 = "assign a, a1; Select a1 with a.stmt# = 1 with a1.stmt# = 3 and 1 = 3";
    string s13 = "assign a, a1; Select a1 with a.stmt# = 1 and a1.stmt# = 3 and 1 = 3";
    string s14 =
            "stmt s; assign a, a1; Select a with a.stmt# = 1 and a1.stmt# = 3 pattern a (\"pattern\", _) such that Next* (1, s)";
    string s15 =
            "stmt s; assign a, a1; while w; Select s with a1.stmt# = 3 such that Next* (1, s) and Next* (s, 3) pattern w (\"pattern\", _)";
    string s16 =
            "stmt s; assign a, a1; while w; if ifs; Select a with 1 = 3 pattern w (\"pattern\", _) and ifs (\"pattern\", _, _) such that Follows*(1,3) ";

    Clause c1 = Clause();
    Clause c2 = Clause();
    Clause c3 = Clause();
    Clause c4 = Clause();
    Clause c5 = Clause();
    Clause c8 = Clause();
    Clause c11 = Clause();
    Clause c14 = Clause();
    Clause c15 = Clause();
    Clause c16 = Clause();

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
    Clause s12_output = qp.parsePQL(s12)[0];
    Clause s13_output = qp.parsePQL(s13)[0];
    Clause s14_output = qp.parsePQL(s14)[0];
    Clause s15_output = qp.parsePQL(s15)[0];
    Clause s16_output = qp.parsePQL(s16)[0];

    RelationshipRef r1 = RelationshipRef(RelationshipType::NEXT_T, int1, int2);
    RelationshipRef r2 = RelationshipRef(RelationshipType::AFFECTS, a1, a2);
    RelationshipRef r3 = RelationshipRef(RelationshipType::WITH, cProcname, pProcname);
    RelationshipRef r4a = RelationshipRef(RelationshipType::PATTERN, x, wildcard, whileEntity);
    RelationshipRef r4b = RelationshipRef(RelationshipType::PATTERN, x, wildcard, ifEntity);
    RelationshipRef r5_a = RelationshipRef(RelationshipType::NEXT_T, int1, s);
    RelationshipRef r5_b = RelationshipRef(RelationshipType::NEXT_T, s, int3);
    RelationshipRef r5_c = RelationshipRef(RelationshipType::FOLLOWS_T, int1, int3);
    RelationshipRef r8_a = RelationshipRef(RelationshipType::PATTERN, patternString, wildcard, a);
    RelationshipRef r8_b = RelationshipRef(RelationshipType::PATTERN, patternString, wildcard, whileEntity);
    RelationshipRef r8_c = RelationshipRef(RelationshipType::PATTERN, patternString, wildcard, ifEntity);
    RelationshipRef r11_a = RelationshipRef(RelationshipType::WITH, astmt, int1);
    RelationshipRef r11_b = RelationshipRef(RelationshipType::WITH, a1stmt, int3);
    RelationshipRef r11_c = RelationshipRef(RelationshipType::WITH, int1, int3);


    SECTION("no mixed clauses query") {
        c1.appendEntityToFind(boolean);
        c1.appendRef(r1);
        REQUIRE(s1_output.equals(c1));

        c2.appendEntityToFind(a1);
        c2.appendEntityToFind(a2stmt);
        c2.appendRef(r2);
        REQUIRE(s2_output.equals(c2));

        c3.appendEntityToFind(cProcname);
        c3.appendRef(r3);
        REQUIRE(s3_output.equals(c3));

        c4.appendEntityToFind(whileEntity);
        c4.appendRef(r4a);
        c4.appendRef(r4b);
        REQUIRE(s4_output.equals(c4));
    }SECTION("multiple such that queries") {
        c5.appendEntityToFind(s);
        c5.appendRef(r5_a);
        c5.appendRef(r5_b);
        c5.appendRef(r5_c);
        REQUIRE(s5_output.equals(c5));
        REQUIRE(s6_output.equals(c5));
        REQUIRE(s7_output.equals(c5));
    }

    SECTION("multiple pattern queries") {
        c8.appendEntityToFind(a);
        c8.appendRef(r8_a);
        c8.appendRef(r8_b);
        c8.appendRef(r8_c);
        REQUIRE(s8_output.equals(c8));
        REQUIRE(s9_output.equals(c8));
        REQUIRE(s10_output.equals(c8));
    }

    SECTION("multiple such that queries") {
        c11.appendEntityToFind(a1);
        c11.appendRef(r11_a);
        c11.appendRef(r11_b);
        c11.appendRef(r11_c);
        REQUIRE(s11_output.equals(c11));
        REQUIRE(s12_output.equals(c11));
        REQUIRE(s13_output.equals(c11));
    }SECTION("mixed queries") {
        c14.appendEntityToFind(a);
        c14.appendRef(r11_a);
        c14.appendRef(r11_b);
        c14.appendRef(r8_a);
        c14.appendRef(r5_a);
        REQUIRE(s14_output.equals(c14));

        c15.appendEntityToFind(s);
        c15.appendRef(r11_b);
        c15.appendRef(r5_a);
        c15.appendRef(r5_b);
        c15.appendRef(r8_b);
        REQUIRE(s15_output.equals(c15));

        c16.appendEntityToFind(a);
        c16.appendRef(r11_c);
        c16.appendRef(r8_b);
        c16.appendRef(r8_c);
        c16.appendRef(r5_c);
        REQUIRE(s16_output.equals(c16));
    }
}

TEST_CASE("advanced trippy queries") {
    QueryProcessor qp = QueryProcessor();
    //default query
    string s1 = "assign a, b; variable v; "
                "Select <a.stmt#, b.stmt#, v.varName> "
                "such that Next(a, b) and Next(b, a) "
                "pattern a (v, \"x+1\") and b (v, \"(x+y)\") "
                "with b.stmt# = a.stmt# and v.varName = a.stmt#";

    //with -> such that -> pattern
    string s2 = "assign pattern, with; variable and; "
                "Select <pattern.stmt#, pattern.stmt#, with.stmt#, with.stmt#, and.varName>"
                "with with.stmt# = pattern.stmt# and and.varName = pattern.stmt# "
                "such that Next(pattern, with) and Next(with, pattern) "
                "pattern pattern (and, \"(x)*(y)*(z)\") and with (and, \"(1*(1+2)%3)\")";

    //with -> pattern -> such that
    string s3 = "assign pattern, with; variable and;"
                "Select <pattern.stmt#, with.stmt#, and.varName> "
                "with with.stmt# = pattern.stmt# and and.varName = pattern.stmt# "
                "pattern pattern (and, \"such + that\") and with (and, _\"suchthat\"_) "
                "such that Next(pattern, with) and Next(with, pattern)";

    //such that -> with -> pattern
    string s4 = "assign pattern, with; variable and; "
                "Select <pattern.stmt#, with.stmt#, and.varName> "
                "such that Next(pattern, with) and Next(with, pattern) "
                "with with.stmt# = pattern.stmt# and and.varName = pattern.stmt# "
                "pattern pattern (and, \"pattern\") and with (and, _\"and\"_)";

    //interleave
    string s5 = "assign pattern, with; variable and;\n"
                "Select <pattern.stmt#, with.stmt#, and.varName>\n"
                "with pattern.stmt# = with.stmt# pattern pattern (and, \"Modifies\") such that Next(pattern, with)\n"
                "such that Next(with, pattern) with and.varName = pattern.stmt# pattern with (and, \"Follows\")";

    //such that -> pattern -> with
    string s6 = "assign with, and; variable pattern;\n"
                "Select with\n"
                "such that Next(with, and) and Next(and, with)\n"
                "pattern with (pattern, \"(idea) * ()\") and and (pattern, \"(((testing)))\")\n"
                "with and.stmt# = and.stmt# and pattern.varName = and.stmt#";

    //with -> such that -> pattern
    string s7 = "assign with, and; variable pattern;\n"
                "Select with\n"
                "with and.stmt# = and.stmt# and pattern.varName = and.stmt#\n"
                "such that Next(with, and) and Next(and, with)\n"
                "pattern with (pattern, \"_(((x+1)+(y+1)))_\") and and (pattern, _\"(((x*y)))\"_)";

    //with -> pattern -> such that
    string s8 = "assign with, and; variable pattern;\n"
                "Select with\n"
                "with and.stmt# = and.stmt# and pattern.varName = and.stmt#\n"
                "pattern with (pattern, _\"1+2\"_) and and (pattern, _\"3*4\"_)\n"
                "such that Next(with, and) and Next(and, with)";

    //pattern -> with -> such that
    string s9 = "assign with, and; variable pattern;\n"
                "Select with\n"
                "pattern with (pattern, \"(1%2-10)\") and and (pattern, _\"(3*4-5)\"_)\n"
                "with and.stmt# = and.stmt# and pattern.varName = and.stmt#\n"
                "such that Next(with, and) and Next(and, with)";

    //interleave
    string s10 = "assign with, and; variable pattern;\n"
                 "Select with\n"
                 "with and.stmt# = and.stmt# "
                 "pattern and (pattern, _\"(((((x)))))*((((y)))))\"_)\n"
                 "such that Next(and, with)"
                 "pattern with (pattern, \"((pattern)+(with)+(and)*(such)(that)(wow)(hello))\") "
                 "with pattern.varName = and.stmt#\n"
                 "such that Next(with, and) ";

    SECTION("test not empty aka valid") {
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
    }
}

TEST_CASE("remove duplicate relationships") {
    QueryProcessor qp = QueryProcessor();
    string s1 = "procedure p, q; Select p.procName such that Calls (p, q)  and Calls (p, q)"; // 1
    string s2 =
            "procedure p, q; Select p.procName such that Calls (p, q) and Modifies (p, \"i\") and Calls (p, q)"; // 2
    string s3 = "assign a, a1; variable v; Select a pattern a (v, _) and a1 (v, _) and a (v, _)"; // 2
    string s4 = "assign a; Select a pattern a (\"x\", _) and a (\"x\", _) and a (\"y\", _)"; // 2
    string s5 =
            "assign a, a1; variable v, v1; Select a pattern a (v, \"x+1\") and a (v, \"x+1\") and a (v, \"y+1\")"; // 2
    string s6 =
            "procedure p, q; Select p with p.procName = \"Third\" with q.procName = \"Third\" and \"Third\" = q.procName"; //2
    string s7 =
            "procedure p, q; Select p with p.procName = \"Third\" with q.procName = \"Third\" and q.procName = \"Third\""; //2

    vector<RelationshipRef> r1 = qp.parsePQL(s1)[0].refList;
    vector<RelationshipRef> r2 = qp.parsePQL(s2)[0].refList;
    vector<RelationshipRef> r3 = qp.parsePQL(s3)[0].refList;
    vector<RelationshipRef> r4 = qp.parsePQL(s4)[0].refList;
    vector<RelationshipRef> r5 = qp.parsePQL(s5)[0].refList;
    vector<RelationshipRef> r6 = qp.parsePQL(s6)[0].refList;
    vector<RelationshipRef> r7 = qp.parsePQL(s7)[0].refList;

    SECTION("test number of relationships") {
        REQUIRE(r1.size() == 1);
        REQUIRE(r2.size() == 2);
        REQUIRE(r3.size() == 2);
        REQUIRE(r4.size() == 2);
        REQUIRE(r5.size() == 2);
        REQUIRE(r6.size() == 2);
        REQUIRE(r7.size() == 2);
    }
}

TEST_CASE("edge cases") {
    QueryProcessor qp = QueryProcessor();
    string s1 = "variable v; Select v with v   .   varName = \"   x   \"";
    string s2 = "variable v; Select v with v\n.\tvarName = \"x   \"";
    string s3 = "variable v; Select v with v.varName = \"   x\"";
    string s4 = "variable v; Select v with v.varName = \"\tx\n\"";
    string s5 = "variable v; Select v with v.varName = \"x\"";
    string s6 = "stmt s1, s2; Select s1 such that Affects(s1, s2)";

    SECTION("valid") {
        REQUIRE(!qp.parsePQL(s1).empty());
        REQUIRE(!qp.parsePQL(s2).empty());
        REQUIRE(!qp.parsePQL(s3).empty());
        REQUIRE(!qp.parsePQL(s4).empty());
        REQUIRE(!qp.parsePQL(s5).empty());
        REQUIRE(!qp.parsePQL(s6).empty());
    }

    string in1 = "variable v; assign v; Select v";
    string in2 = "stmt s; Select s.procName with s.stmt# = 5";
    string in3 = "procedure p; Select p.varName with p.procName = 3";
    string in4 = "variable v; Select BOOLEAN such that Uses(_, v)";
    string in5 = "variable v; Select BOOLEAN such that Modifies(_, v)";
    string in6 = "while w; Select w;";
    string in7 = "Select BOOLEAN with 3 = \"3\"";
    string in8 = "Select BOOLEAN with \"3\" = 3";
    string in9 = "assign a; Select BOOLEAN with a.stmt# = a";
    string in10 = "stmt s; Select BOOLEAN with \"First\" = s.procName";
    string in11 = "assign a; Select BOOLEAN pattern a (_,\"y\") and pattern a (_, \"x\")";

    SECTION("invalid") {
        REQUIRE(qp.parsePQL(in1).empty());
        REQUIRE(qp.parsePQL(in2).empty());
        REQUIRE(qp.parsePQL(in3).empty());
        REQUIRE(qp.parsePQL(in4)[0].entityToFindList[0].name == "FALSE");
        REQUIRE(qp.parsePQL(in5)[0].entityToFindList[0].name == "FALSE");
        REQUIRE(qp.parsePQL(in6).empty());
        REQUIRE(qp.parsePQL(in7).empty());
        REQUIRE(qp.parsePQL(in8).empty());
        REQUIRE(qp.parsePQL(in9).empty());
        REQUIRE(qp.parsePQL(in10)[0].entityToFindList[0].name == "FALSE");
        REQUIRE(qp.parsePQL(in11).empty());
    }

    string pat1 = " variable v; assign a; stmt s; if ifs; while w; Select BOOLEAN pattern ifs (v, _)";
    string pat2 = " variable v; assign a; stmt s; if ifs; while w; Select BOOLEAN pattern a (v, _, _)";
    string pat3 = " variable v; assign a; stmt s; if ifs; while w; Select BOOLEAN pattern w (v, _, _)";
    string pat4 = " variable v; assign a; stmt s; if ifs; while w; Select BOOLEAN pattern s (v, _)";
    string pat5 = " variable v; assign a; stmt s; if ifs; while w; Select BOOLEAN pattern s (v, _, _)";
    string pat6 = " variable v; assign a; stmt s; if ifs; while w; Select BOOLEAN pattern s (v, _, _)";
    SECTION("pattern edge cases") {
        REQUIRE(qp.parsePQL(pat1).empty());
        REQUIRE(qp.parsePQL(pat2).empty());
        REQUIRE(qp.parsePQL(pat3).empty());
        REQUIRE(qp.parsePQL(pat4)[0].entityToFindList[0].name == "FALSE");
        REQUIRE(qp.parsePQL(pat5)[0].entityToFindList[0].name == "FALSE");
        REQUIRE(qp.parsePQL(pat6)[0].entityToFindList[0].name == "FALSE");
    }

    string space1 = "while w; Selectw";
    string space2 = "if i; variable v; Select BOOLEAN such thatModifies(i,v)";
    string space3 = "assign a; Select BOOLEAN patterna (_,_)";
    string space4 = "Select BOOLEAN with1=1";

    SECTION("no space edge cases") {
        REQUIRE(qp.parsePQL(space1).empty());
        REQUIRE(qp.parsePQL(space2).empty());
        REQUIRE(qp.parsePQL(space3).empty());
        REQUIRE(qp.parsePQL(space4).empty());
    }
}

TEST_CASE("debugging") {
    QueryProcessor qp = QueryProcessor();
    string s1 = "assign a; Select BOOLEAN pattern a (_,\"y\") and pattern a (_, \"x\")" ;
    vector<Clause> c = qp.parsePQL(s1);

    if (c.empty()) {
        cout << "invalid!" << "\n";
    } else {
        cout << c[0].toString() << "\n";
    }

}