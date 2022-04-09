#include <vector>

#include "catch.hpp"
#include "QPSHandler.h"
#include "FormattedResult.h"
#include "PkbGetterStub.h"
#include "QPSUtil.h"

// Source for testing
//procedure f {
//  x = 5;  // 1
//  y = x;  // 2
//  x = 5;  // 3
//  while (x > 4) {  // 4
//    read z;  // 5
//    if (z < 10) then {  // 6
//      x = 100;  // 7
//      z = 5;  // 8
//    } else {
//      y = 100;  // 9
//      z = 0;  // 10
//    }
//  }
//  print x; // 11
//  call g; // 12
//}

QPS_GENERAL_DATA_INFO qpsDataH;
QPS_SOURCE_INFO sourceInfoH;

TEST_CASE("Testing Cases with No Clauses") {
    // Test query 1: print p; Select p
    // Test query 2: stmt s; Select BOOLEAN
    // Test query 3: while w; assign a; Select <w, a>
    // Test query 4: print p; Select p.varName
    // Test query 5: call c; read r; Select <c.procName, r.varName>
    DB db;
    PkbGetter *pg = new PkbGetterStub(&db);
    QPSHandler *qpsHandler = new QPSHandler(pg);

    GroupedClause query1;
    query1.entityToFindList = {qpsDataH.printSyn};

    GroupedClause query2;
    query2.entityToFindList = {qpsDataH.boolean};

    GroupedClause query3;
    query3.entityToFindList = {qpsDataH.whileSyn, qpsDataH.assignSyn};

    GroupedClause query4;
    query4.entityToFindList = {qpsDataH.printSynVarAttr};

    GroupedClause query5;
    query5.entityToFindList = {qpsDataH.callSynProcAttr, qpsDataH.readSynVarAttr};

    FormattedResult expectedResult1;
    expectedResult1.setValid(true);
    expectedResult1.setEntityList({qpsDataH.printSyn});
    Result printResult;
    printResult.setValid(true);
    printResult.setOneSynEntity(qpsDataH.printSyn);
    printResult.setOneSynSet(sourceInfoH.prints);
    Table query1Table = Table(&printResult);
    expectedResult1.setFinalTable(query1Table);

    FormattedResult expectedResult2;
    expectedResult2.setValid(true);
    expectedResult2.setBoolReturn(true);

    FormattedResult expectedResult3;
    expectedResult3.setValid(true);
    expectedResult3.setEntityList({qpsDataH.whileSyn, qpsDataH.assignSyn});
    Result whileResult;
    whileResult.setValid(true);
    whileResult.setOneSynEntity(qpsDataH.whileSyn);
    whileResult.setOneSynSet(sourceInfoH.whiles);
    Table query3Table = Table(&whileResult);
    Result assignResult;
    assignResult.setValid(true);
    assignResult.setOneSynEntity(qpsDataH.assignSyn);
    assignResult.setOneSynSet(sourceInfoH.assigns);
    Table otherTable = Table(&assignResult);
    query3Table = Table(&query3Table, &otherTable);
    expectedResult3.setFinalTable(query3Table);

    FormattedResult expectedResult4;
    expectedResult4.setValid(true);
    expectedResult4.setEntityList({qpsDataH.printSynVarAttr});
    Table query4Table = Table(&printResult);
    expectedResult4.setFinalTable(query4Table);

    FormattedResult expectedResult5;
    expectedResult5.setValid(true);
    expectedResult5.setEntityList({qpsDataH.callSynProcAttr, qpsDataH.readSynVarAttr});
    Result callResult;
    callResult.setValid(true);
    callResult.setOneSynEntity(qpsDataH.callSyn);
    callResult.setOneSynSet(sourceInfoH.calls);
    Result readResult;
    readResult.setValid(true);
    readResult.setOneSynEntity(qpsDataH.readSyn);
    readResult.setOneSynSet(sourceInfoH.reads);
    Table query5Table = Table(&callResult);
    Table query5OtherTable = Table(&readResult);
    query5Table = Table(&query5Table, &query5OtherTable);
    expectedResult5.setFinalTable(query5Table);

    FormattedResult testQuery1 = qpsHandler->processClause(query1);
    FormattedResult testQuery2 = qpsHandler->processClause(query2);
    FormattedResult testQuery3 = qpsHandler->processClause(query3);
    FormattedResult testQuery4 = qpsHandler->processClause(query4);
    FormattedResult testQuery5 = qpsHandler->processClause(query5);

    REQUIRE(testQuery1 == expectedResult1);
    REQUIRE(testQuery2 == expectedResult2);
    REQUIRE(testQuery3 == expectedResult3);
    REQUIRE(testQuery4 == expectedResult4);
    REQUIRE(testQuery5 == expectedResult5);
}

TEST_CASE("Testing cases with only fixed entity groups") {
    // Test query true: assign a; Select a such that Follows("1", "2") and Follows("2", "3")
    // Test query false: assign a; Select a such that Follows("1", "2") and Follows("1", "3")
    DB db;
    PkbGetter *pg = new PkbGetterStub(&db);
    QPSHandler *qpsHandler = new QPSHandler(pg);

    GroupedClause queryTrue;
    queryTrue.entityToFindList = {qpsDataH.assignSyn};
    RelationshipRefGroup group1;
    group1.relRefGroup = {qpsDataH.follows12, qpsDataH.follows23};
    queryTrue.relRefGroups = {group1};

    GroupedClause queryFalse;
    queryFalse.entityToFindList = {qpsDataH.assignSyn};
    RelationshipRefGroup group2;
    group2.relRefGroup = {qpsDataH.follows12, qpsDataH.follows13};
    queryFalse.relRefGroups = {group2};

    FormattedResult expectedTrue;
    expectedTrue.setValid(true);
    expectedTrue.setEntityList({qpsDataH.assignSyn});
    Result assignResult;
    assignResult.setValid(true);
    assignResult.setOneSynEntity(qpsDataH.assignSyn);
    assignResult.setOneSynSet(sourceInfoH.assigns);
    Table finalTableTrue = Table(&assignResult);
    expectedTrue.setFinalTable(finalTableTrue);

    FormattedResult expectedFalse;
    expectedFalse.setValid(false);

    FormattedResult testQueryTrue = qpsHandler->processClause(queryTrue);
    FormattedResult testQueryFalse = qpsHandler->processClause(queryFalse);

    REQUIRE(testQueryTrue == expectedTrue);
    REQUIRE(testQueryFalse == expectedFalse);
}

TEST_CASE("Testing cases with Empty Intermediate Tables") {
    // Test query 1: assign a; Select a pattern a ("x", _) pattern a (_, "x")
    // Test query 2: assign a; Select BOOLEAN pattern a ("x", _) pattern a (_, "x")
    // Test query 3: assign a; Select a.stmt# pattern a ("x", _) pattern a (_, "x")
    // Test query 4: assign a; assign a1; Select a pattern a("x", _) pattern a(_, "x") and a1(_, _)
    // Test query 5: assign a; assign a1; Select <a, a1> pattern a("x", _) pattern a(_, "x") and a1(_, _)
    DB db;
    PkbGetter *pg = new PkbGetterStub(&db);
    QPSHandler *qpsHandler = new QPSHandler(pg);

    GroupedClause query1;
    query1.setEntityToFindList({qpsDataH.assignSyn});
    RelationshipRefGroup group1;
    group1.relRefGroup = {qpsDataH.patternAXWc, qpsDataH.patternAWcX};
    query1.relRefGroups = {group1};

    GroupedClause query2;
    query2.setEntityToFindList({qpsDataH.boolean});
    query2.relRefGroups = {group1};

    GroupedClause query3;
    query3.setEntityToFindList({qpsDataH.assignSynStmtAttr});
    query3.relRefGroups = {group1};

    GroupedClause query4;
    query4.setEntityToFindList({qpsDataH.assignSyn});
    RelationshipRefGroup group4;
    group4.relRefGroup = {qpsDataH.patternA1WcWc};
    query4.relRefGroups = {group1, group4};

    GroupedClause query5;
    query5.setEntityToFindList({qpsDataH.assignSyn, qpsDataH.assignSyn1});
    query5.relRefGroups = {group1, group4};

    FormattedResult expectedResult;
    FormattedResult expectedBoolResult;
    expectedBoolResult.setBoolReturn(true);

    FormattedResult testQuery1 = qpsHandler->processClause(query1);
    FormattedResult testQuery2 = qpsHandler->processClause(query2);
    FormattedResult testQuery3 = qpsHandler->processClause(query3);
    FormattedResult testQuery4 = qpsHandler->processClause(query4);
    FormattedResult testQuery5 = qpsHandler->processClause(query5);

    REQUIRE(testQuery1 == expectedResult);
    REQUIRE(testQuery2 == expectedBoolResult);
    REQUIRE(testQuery3 == expectedResult);
    REQUIRE(testQuery4 == expectedResult);
    REQUIRE(testQuery5 == expectedResult);
}

TEST_CASE("Testing cases related to final table") {
    DB db;
    PkbGetter *pg = new PkbGetterStub(&db);
    QPSHandler *qpsHandler = new QPSHandler(pg);

    SECTION("Boolean short circuit if all intermediate tables not empty") {
        // Test query: assign a, a1, a2, a3; Select BOOLEAN such that Follows(a, a1) and Follows(a2, a3)
        GroupedClause query;
        query.entityToFindList = {qpsDataH.boolean};
        RelationshipRefGroup group1;
        group1.relRefGroup = {qpsDataH.followsAA1};
        RelationshipRefGroup group2;
        group2.relRefGroup = {qpsDataH.followsA2A3};
        query.relRefGroups = {group1, group2};

        FormattedResult testQuery = qpsHandler->processClause(query);

        FormattedResult expectedResult;
        expectedResult.setValid(true);
        expectedResult.setBoolReturn(true);
        expectedResult.setEntityList({qpsDataH.boolean});

        REQUIRE(expectedResult == testQuery);
    }

    SECTION("All entities being found in final table") {
        // Test query 1: assign a, a1, a2; Select <a, a1> such that Follows(a, a1) and Follows(a1, a2)
        // Test query 2: assign a, a1, a2; Select <a.stmt#, a1> such that Follows(a, a1) and Follows(a1, a2)
        // Test query 3: assign a, a1; Select a such that Follows(a, a1)
        GroupedClause query1;
        query1.entityToFindList = {qpsDataH.assignSyn, qpsDataH.assignSyn1};
        RelationshipRefGroup group1;
        group1.relRefGroup = {qpsDataH.followsAA1, qpsDataH.followsA1A2};
        query1.relRefGroups = {group1};

        FormattedResult expectedResult1;
        expectedResult1.setValid(true);
        std::vector<Entity> entitiesQuery1 = {qpsDataH.assignSyn, qpsDataH.assignSyn1};
        expectedResult1.setEntityList(entitiesQuery1);
        Result followsResultAA1;
        followsResultAA1.setValid(true);
        followsResultAA1.setTwoSynEntities({qpsDataH.assignSyn, qpsDataH.assignSyn1});
        followsResultAA1.setTwoSynSet(sourceInfoH.followsAssignAssign);
        Result followsResultA1A2;
        followsResultA1A2.setValid(true);
        followsResultA1A2.setTwoSynEntities({qpsDataH.assignSyn1, qpsDataH.assignSyn2});
        followsResultA1A2.setTwoSynSet(sourceInfoH.followsAssignAssign);
        Table interTableQuery11 = Table(&followsResultAA1);
        Table interTableQuery12 = Table(&followsResultA1A2);
        Table interTableQuery13 = Table(&interTableQuery11, &interTableQuery12);
        Table finalTableQuery1 = interTableQuery13.extractColumns(&entitiesQuery1);
        expectedResult1.setFinalTable(finalTableQuery1);

        GroupedClause query2;
        query2.entityToFindList = {qpsDataH.assignSynStmtAttr, qpsDataH.assignSyn1};
        query2.relRefGroups = {group1};

        FormattedResult expectedResult2;
        expectedResult2 = expectedResult1;
        expectedResult2.setEntityList({qpsDataH.assignSynStmtAttr, qpsDataH.assignSyn1});

        GroupedClause query3;
        query3.entityToFindList = {qpsDataH.assignSyn};
        RelationshipRefGroup group3;
        group3.relRefGroup = {qpsDataH.followsAA1};
        query3.relRefGroups = {group3};

        FormattedResult expectedResult3;
        expectedResult3.setValid(true);
        expectedResult3.setEntityList({qpsDataH.assignSyn});
        Table interTableQuery31 = Table(&followsResultAA1);
        std::vector<Entity> entitiesQuery3 = {qpsDataH.assignSyn};
        Table finalTableQuery3 = interTableQuery31.extractColumns(&entitiesQuery3);
        expectedResult3.setFinalTable(finalTableQuery3);

        FormattedResult testQuery1 = qpsHandler->processClause(query1);
        FormattedResult testQuery2 = qpsHandler->processClause(query2);
        FormattedResult testQuery3 = qpsHandler->processClause(query3);

        REQUIRE(testQuery1 == expectedResult1);
        REQUIRE(testQuery2 == expectedResult2);
        REQUIRE(testQuery3 == expectedResult3);
    }

    SECTION("All entities not in final table") {
        // Test query 1: read r; assign a, a1; Select r such that Follows(a, a1)
        // Test query 2: read r; assign a, a1; Select <r, a> such that Follows(a, a1)
        // Test query 3: read r; assign a, a1; Select r.varName such that Follows(a, a1)
        // Test query 4 read r; assign a, a1; Select <r.varName, a.stmt#> such that Follows(a, a1)
        GroupedClause query1;
        query1.entityToFindList = {qpsDataH.readSyn};
        RelationshipRefGroup group1;
        group1.relRefGroup = {qpsDataH.followsAA1};
        query1.relRefGroups = {group1};

        FormattedResult expectedResult1;
        expectedResult1.setValid(true);
        expectedResult1.setEntityList({qpsDataH.readSyn});
        Result readResult;
        readResult.setValid(true);
        readResult.setOneSynEntity(qpsDataH.readSyn);
        readResult.setOneSynSet(sourceInfoH.reads);
        Table readTable = Table(&readResult);
        expectedResult1.setFinalTable(readTable);

        GroupedClause query2 = query1;
        query2.entityToFindList = {qpsDataH.readSyn, qpsDataH.assignSyn};

        FormattedResult expectedResult2 = expectedResult1;
        expectedResult2.setEntityList({qpsDataH.readSyn, qpsDataH.assignSyn});
        Result followsResultAA1;
        followsResultAA1.setValid(true);
        followsResultAA1.setTwoSynEntities({qpsDataH.assignSyn, qpsDataH.assignSyn1});
        followsResultAA1.setTwoSynSet(sourceInfoH.followsAssignAssign);
        Table intermediateTable2 = Table(&followsResultAA1);
        std::vector<Entity> entities2 = {qpsDataH.assignSyn};
        intermediateTable2 = intermediateTable2.extractColumns(&entities2);
        Table finalTableReadAssign = Table(&intermediateTable2, &readTable);
        expectedResult2.setFinalTable(finalTableReadAssign);

        GroupedClause query3 = query1;
        query3.entityToFindList = {qpsDataH.readSynVarAttr};

        FormattedResult expectedResult3 = expectedResult1;
        expectedResult3.setEntityList({qpsDataH.readSynVarAttr});

        GroupedClause query4 = query1;
        query4.entityToFindList = {qpsDataH.readSynVarAttr, qpsDataH.assignSynStmtAttr};

        FormattedResult expectedResult4 = expectedResult2;
        expectedResult4.setEntityList({qpsDataH.readSynVarAttr, qpsDataH.assignSynStmtAttr});

        FormattedResult testQuery1 = qpsHandler->processClause(query1);
        FormattedResult testQuery2 = qpsHandler->processClause(query2);
        FormattedResult testQuery3 = qpsHandler->processClause(query3);
        FormattedResult testQuery4 = qpsHandler->processClause(query4);

        REQUIRE(testQuery1 == expectedResult1);
        REQUIRE(testQuery2 == expectedResult2);
        REQUIRE(testQuery3 == expectedResult3);
        REQUIRE(testQuery4 == expectedResult4);
    }
}