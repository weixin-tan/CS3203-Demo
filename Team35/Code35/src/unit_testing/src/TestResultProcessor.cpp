#include <vector>

#include "catch.hpp"
#include "ResultProcessor.h"
#include "FormattedResult.h"

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
//  call h; // 13
//}

Entity boolEntity = Entity(EntityType::BOOLEAN, "");
Entity printEntity = Entity(EntityType::PRINT, "p");
Entity printVar = Entity(EntityType::PRINT, "p", EntityAttributeType::VARNAME);
Entity whileEntity = Entity(EntityType::WHILE, "w");
Entity assignEntity = Entity(EntityType::ASSIGNMENT, "a");
Entity assignStmt = Entity(EntityType::ASSIGNMENT, "a", EntityAttributeType::STMT);
Entity assignOneEntity = Entity(EntityType::ASSIGNMENT, "a1");
Entity assignTwoEntity = Entity(EntityType::ASSIGNMENT, "a2");
Entity assignThreeEntity = Entity(EntityType::ASSIGNMENT, "a3");
Entity readEntity = Entity(EntityType::READ, "r");
Entity readVar = Entity(EntityType::READ, "r", EntityAttributeType::VARNAME);
Entity callEntity = Entity(EntityType::CALL, "c");
Entity callProc = Entity(EntityType::CALL, "c", EntityAttributeType::PROCNAME);

ProgramElement printElem = ProgramElement::createStatement(ElementType::PRINT, 11);
std::set<ProgramElement> prints = {printElem};

ProgramElement whileElem = ProgramElement::createStatement(ElementType::WHILE, 4);
std::set<ProgramElement> whiles = {whileElem};

ProgramElement callElem12 = ProgramElement::createStatement(ElementType::CALL, 12);
ProgramElement callElem13 = ProgramElement::createStatement(ElementType::CALL, 13);
std::set<ProgramElement> calls = {callElem12, callElem13};

ProgramElement readElem = ProgramElement::createStatement(ElementType::READ, 5);
std::set<ProgramElement> reads = {readElem};

ProgramElement assignElem1 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 1);
ProgramElement assignElem2 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 2);
ProgramElement assignElem3 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 3);
ProgramElement assignElem7 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 7);
ProgramElement assignElem8 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 8);
ProgramElement assignElem9 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 9);
ProgramElement assignElem10 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 10);
std::set<ProgramElement> assigns = {assignElem1, assignElem2, assignElem3, assignElem7, assignElem8, assignElem9, assignElem10};

/*
TEST_CASE("Testing Empty Result from Short Circuit Optimisation"){
    ResultProcessor resultProcessor = ResultProcessor();
    ResultGroup emptyGroup;
    ResultGroup emptyGroupWithBoolReturn;
    emptyGroupWithBoolReturn.setBoolReturn(true);
    FormattedResult nonValidFormattedResult;
    FormattedResult nonValidBoolFormattedResult;
    nonValidBoolFormattedResult.setBoolReturn(true);

    FormattedResult testEmptyGroup = resultProcessor.processResults(emptyGroup);
    FormattedResult testEmptyBoolGroup = resultProcessor.processResults(emptyGroupWithBoolReturn);

    REQUIRE(testEmptyGroup == nonValidFormattedResult);
    REQUIRE(testEmptyBoolGroup == nonValidBoolFormattedResult);
}

TEST_CASE("Testing Cases with No Clauses") {
    // Test query 1: print p; Select p
    // Test query 2: stmt s; Select BOOLEAN
    // Test query 3: while w; assign a; Select <w, a>
    // Test query 4: print p; Select p.varName
    // Test query 5: call c; read r; Select <c.procName, r.varName>
    ResultProcessor resultProcessor = ResultProcessor();

    ResultGroup query1;
    query1.setValid(true);
    query1.setEntitiesToReturn({printEntity});
    Result printResult;
    printResult.setValid(true);
    printResult.setResultType(ResultType::NO_CLAUSE);
    printResult.setOneSynEntity(printEntity);
    printResult.setOneSynSet(prints);
    query1.addResultList({printResult});

    ResultGroup query2;
    query2.setValid(true);
    query2.setBoolReturn(true);
    query2.setEntitiesToReturn({boolEntity});
    Result boolResult;
    boolResult.setValid(true);
    boolResult.setResultType(ResultType::NO_CLAUSE);
    boolResult.setOneSynEntity(boolEntity);
    query2.addResultList({boolResult});

    ResultGroup query3;
    query3.setValid(true);
    query3.setEntitiesToReturn({whileEntity, assignEntity});
    Result assignResult;
    assignResult.setValid(true);
    assignResult.setResultType(ResultType::NO_CLAUSE);
    assignResult.setOneSynEntity(assignEntity);
    assignResult.setOneSynSet(assigns);
    Result whileResult;
    whileResult.setValid(true);
    whileResult.setResultType(ResultType::NO_CLAUSE);
    whileResult.setOneSynEntity(whileEntity);
    whileResult.setOneSynSet(whiles);
    query3.addResultList({whileResult, assignResult});


    ResultGroup query4;
    query4.setValid(true);
    query4.setEntitiesToReturn({printVar});
    Result printVarResult;
    printVarResult.setValid(true);
    printVarResult.setResultType(ResultType::NO_CLAUSE);
    printVarResult.setOneSynEntity(printVar);
    printVarResult.setOneSynSet(prints);
    query4.addResultList({printVarResult});

    ResultGroup query5;
    query5.setValid(true);
    query5.setEntitiesToReturn({callProc, readVar});
    Result callProcResult;
    callProcResult.setValid(true);
    callProcResult.setResultType(ResultType::NO_CLAUSE);
    callProcResult.setOneSynEntity(callProc);
    callProcResult.setOneSynSet(calls);
    Result readVarResult;
    readVarResult.setValid(true);
    readVarResult.setResultType(ResultType::NO_CLAUSE);
    readVarResult.setOneSynEntity(readVar);
    readVarResult.setOneSynSet(reads);
    query5.addResultList({callProcResult, readVarResult});

    FormattedResult expectedResult1;
    expectedResult1.setValid(true);
    expectedResult1.setEntityList({printEntity});
    Table query1Table = Table(&printResult);
    expectedResult1.setFinalTable(query1Table);

    FormattedResult expectedResult2;
    expectedResult2.setValid(true);
    expectedResult2.setBoolReturn(true);

    FormattedResult expectedResult3;
    expectedResult3.setValid(true);
    expectedResult3.setEntityList({whileEntity, assignEntity});
    Table query3Table = Table(&whileResult);
    Table otherTable = Table(&assignResult);
    query3Table = Table(&query3Table, &otherTable);
    expectedResult3.setFinalTable(query3Table);

    FormattedResult expectedResult4;
    expectedResult4.setValid(true);
    expectedResult4.setEntityList({printVar});
    Result scrubbedPrintResult = printResult;
    scrubbedPrintResult.setOneSynEntity(printEntity);
    Table query4Table = Table(&scrubbedPrintResult);
    expectedResult4.setFinalTable(query4Table);

    FormattedResult expectedResult5;
    expectedResult5.setValid(true);
    expectedResult5.setEntityList({callProc, readVar});
    Result scrubbedCallResult = callProcResult;
    scrubbedCallResult.setOneSynEntity(callEntity);
    Result scrubbedReadResult = readVarResult;
    readVarResult.setOneSynEntity(readEntity);
    Table query5Table = Table(&scrubbedCallResult);
    Table query5OtherTable = Table(&scrubbedReadResult);
    query5Table = Table(&query5Table, &query5OtherTable);
    expectedResult5.setFinalTable(query5Table);

    FormattedResult testQuery1 = resultProcessor.processResults(query1);
    FormattedResult testQuery2 = resultProcessor.processResults(query2);
    FormattedResult testQuery3 = resultProcessor.processResults(query3);
    FormattedResult testQuery4 = resultProcessor.processResults(query4);
    FormattedResult testQuery5 = resultProcessor.processResults(query5);

    REQUIRE(testQuery1 == expectedResult1);
    REQUIRE(testQuery2 == expectedResult2);
    REQUIRE(testQuery3 == expectedResult3);
    REQUIRE(testQuery4 == expectedResult4);
    REQUIRE(testQuery5 == expectedResult5);
}

TEST_CASE("Testing cases with Empty Intermediate Tables") {
    // Test query 1: assign a; Select a pattern a ("x", _) pattern a (_, "x")
    // Test query 2: assign a; Select BOOLEAN pattern a ("x", _) pattern a (_, "x")
    // Test query 3: assign a; Select a.stmt# pattern a ("x", _) pattern a (_, "x")
    // Test query 4: assign a; assign a1; Select a pattern a("x", _) pattern a(_, "x") and a1(_, _)
    // Test query 5: assign a; assign a1; Select <a, a1> pattern a("x", _) pattern a(_, "x") and a1(_, _)
    ResultProcessor resultProcessor = ResultProcessor();
    Result boolResult;
    boolResult.setValid(true);
    boolResult.setResultType(ResultType::NO_CLAUSE);
    boolResult.setOneSynEntity(boolEntity);
    Result assignResult;
    assignResult.setValid(true);
    assignResult.setResultType(ResultType::NO_CLAUSE);
    assignResult.setOneSynEntity(assignEntity);
    assignResult.setOneSynSet(assigns);
    Result assignOneResult = assignResult;
    assignOneResult.setOneSynEntity(assignOneEntity);
    Result patternX_;
    patternX_.setValid(true);
    patternX_.setResultType(ResultType::PATTERN_CLAUSE);
    patternX_.setOneSynEntity(assignEntity);
    patternX_.setOneSynSet({assignElem1, assignElem3, assignElem7});
    Result pattern_X = patternX_;
    pattern_X.setOneSynSet({assignElem2});
    Result assignOnePatternResult = assignResult;
    assignOnePatternResult.setResultType(ResultType::PATTERN_CLAUSE);


    ResultGroup query1;
    query1.setValid(true);
    query1.setEntitiesToReturn({assignEntity});
    query1.addResultList({assignResult});
    query1.addResultList({patternX_, pattern_X});

    ResultGroup query2;
    query2.setValid(true);
    query2.setBoolReturn(true);
    query2.setEntitiesToReturn({boolEntity});
    query2.addResultList({boolResult});
    query2.addResultList({patternX_, pattern_X});

    ResultGroup query3;
    query3.setValid(true);
    query3.setEntitiesToReturn({assignStmt});
    Result assignStmtResult = assignResult;
    assignStmtResult.setOneSynEntity(assignStmt);
    query3.addResultList({assignStmtResult});
    query3.addResultList({patternX_, pattern_X});

    ResultGroup query4;
    query4.setValid(true);
    query4.setEntitiesToReturn({assignEntity});
    query4.addResultList({assignResult});
    query4.addResultList({patternX_, pattern_X});
    query4.addResultList({assignOnePatternResult});

    ResultGroup query5;
    query5.setValid(true);
    query5.setEntitiesToReturn({assignEntity, assignOneEntity});
    query5.addResultList({assignResult, assignOneResult});
    query5.addResultList({patternX_, pattern_X});
    query5.addResultList({assignOnePatternResult});

    FormattedResult expectedResult;
    FormattedResult expectedBoolResult;
    expectedBoolResult.setBoolReturn(true);

    FormattedResult testQuery1 = resultProcessor.processResults(query1);
    FormattedResult testQuery2 = resultProcessor.processResults(query2);
    FormattedResult testQuery3 = resultProcessor.processResults(query3);
    FormattedResult testQuery4 = resultProcessor.processResults(query4);
    FormattedResult testQuery5 = resultProcessor.processResults(query5);

    REQUIRE(testQuery1 == expectedResult);
    REQUIRE(testQuery2 == expectedBoolResult);
    REQUIRE(testQuery3 == expectedResult);
    REQUIRE(testQuery4 == expectedResult);
    REQUIRE(testQuery5 == expectedResult);
}

TEST_CASE("Testing cases related to final table") {
    ResultProcessor resultProcessor = ResultProcessor();
    Result boolResult;
    boolResult.setValid(true);
    boolResult.setResultType(ResultType::NO_CLAUSE);
    boolResult.setOneSynEntity(boolEntity);
    Result assignResult;
    assignResult.setValid(true);
    assignResult.setResultType(ResultType::NO_CLAUSE);
    assignResult.setOneSynEntity(assignEntity);
    assignResult.setOneSynSet(assigns);
    Result assignResultA1 = assignResult;
    assignResultA1.setOneSynEntity(assignOneEntity);
    Result assignResultA2 = assignResult;
    assignResultA2.setOneSynEntity(assignTwoEntity);
    Result assignResultA3 = assignResult;
    assignResultA3.setOneSynEntity(assignThreeEntity);
    Result readResult;
    readResult.setValid(true);
    readResult.setResultType(ResultType::NO_CLAUSE);
    readResult.setOneSynEntity(readEntity);
    readResult.setOneSynSet(reads);
    Result followsResultAA1;
    followsResultAA1.setValid(true);
    followsResultAA1.setResultType(ResultType::SUCH_THAT_CLAUSE);
    followsResultAA1.setTwoSynEntities({assignEntity, assignOneEntity});
    followsResultAA1.setTwoSynSet({{assignElem1, assignElem2},
                                   {assignElem2, assignElem3},
                                   {assignElem7, assignElem8},
                                   {assignElem9, assignElem10}});
    Result followsResultA1A2 = followsResultAA1;
    followsResultA1A2.setTwoSynEntities({assignOneEntity, assignTwoEntity});
    Result followsResultA2A3 = followsResultAA1;
    followsResultA2A3.setTwoSynEntities({assignTwoEntity, assignThreeEntity});

    SECTION("Boolean short circuit if all intermediate tables not empty") {
        // Test query: assign a, a1, a2, a3; Select BOOLEAN such that Follows(a, a1) and Follows(a2, a3)
        ResultGroup query;
        query.setValid(true);
        query.setBoolReturn(true);
        query.setEntitiesToReturn({boolEntity});
        query.addResultList({boolResult});
        query.addResultList({followsResultAA1, followsResultA2A3});

        FormattedResult testQuery = resultProcessor.processResults(query);

        FormattedResult expectedResult;
        expectedResult.setValid(true);
        expectedResult.setBoolReturn(true);

        REQUIRE(expectedResult == testQuery);
    }

    SECTION("All entities being found in final table") {
        // Test query 1: assign a, a1, a2; Select <a, a1> such that Follows(a, a1) and Follows(a1, a2)
        // Test query 2: assign a, a1, a2; Select <a.stmt#, a1> such that Follows(a, a1) and Follows(a1, a2)
        // Test query 3: assign a, a1; Select a such that Follows(a, a1)
        ResultGroup query1;
        query1.setValid(true);
        query1.setEntitiesToReturn({assignEntity, assignOneEntity});
        query1.addResultList({assignResult, assignResultA1});
        query1.addResultList({followsResultAA1, followsResultA1A2});

        ResultGroup query2;
        query2.setValid(true);
        query2.setEntitiesToReturn({assignStmt, assignOneEntity});
        query2.addResultList({assignResult, assignResultA1});
        query2.addResultList({followsResultAA1, followsResultA1A2});

        ResultGroup query3;
        query3.setValid(true);
        query3.setEntitiesToReturn({assignEntity});
        query1.addResultList({assignResult});
        query3.addResultList({followsResultAA1});

        FormattedResult expectedResult1;
        expectedResult1.setValid(true);
        std::vector<Entity> entitiesQuery1 = {assignEntity, assignOneEntity};
        expectedResult1.setEntityList(entitiesQuery1);
        Table interTableQuery11 = Table(&followsResultAA1);
        Table interTableQuery12 = Table(&followsResultA1A2);
        Table interTableQuery13 = Table(&interTableQuery11, &interTableQuery12);
        Table finalTableQuery1 = interTableQuery13.extractColumns(&entitiesQuery1);
        expectedResult1.setFinalTable(finalTableQuery1);

        FormattedResult expectedResult2;
        expectedResult2 = expectedResult1;
        expectedResult2.setEntityList({assignStmt, assignOneEntity});

        FormattedResult expectedResult3;
        expectedResult3.setValid(true);
        std::vector<Entity> entitiesQuery3 = {assignEntity};
        expectedResult3.setEntityList(entitiesQuery3);
        Table interTableQuery31 = Table(&followsResultAA1);
        Table finalTableQuery3 = interTableQuery31.extractColumns(&entitiesQuery3);
        expectedResult3.setFinalTable(finalTableQuery3);

        FormattedResult testQuery1 = resultProcessor.processResults(query1);
        FormattedResult testQuery2 = resultProcessor.processResults(query2);
        FormattedResult testQuery3 = resultProcessor.processResults(query3);

        REQUIRE(testQuery1 == expectedResult1);
        REQUIRE(testQuery2 == expectedResult2);
        REQUIRE(testQuery3 == expectedResult3);
    }
    SECTION("All entities not in final table") {
        // Test query 1: read r; assign a, a1; Select r such that Follows(a, a1)
        // Test query 2: read r; assign a, a1; Select <r, a> such that Follows(a, a1)
        // Test query 3: read r; assign a, a1; Select r.varName such that Follows(a, a1)
        // Test query 4 read r; assign a, a1; Select <r.varName, a.stmt#> such that Follows(a, a1)
        ResultGroup query1;
        query1.setValid(true);
        query1.setEntitiesToReturn({readEntity});
        query1.addResultList({readResult});
        query1.addResultList({followsResultAA1});

        ResultGroup query2;
        query2.setValid(true);
        query2.setEntitiesToReturn({readEntity, assignEntity});
        query2.addResultList({readResult, assignResult});
        query2.addResultList({followsResultAA1});

        ResultGroup query3 = query1;
        query3.setEntitiesToReturn({readVar});

        ResultGroup query4 = query2;
        query2.setEntitiesToReturn({readVar, assignStmt});

        Table intermediateTable = Table(&followsResultAA1);
        Table readTable = Table(&readResult);
        Table finalTable = Table(&intermediateTable, &readTable);
        FormattedResult expectedResult1;
        expectedResult1.setValid(true);
        expectedResult1.setEntityList({readEntity});
        expectedResult1.setFinalTable(readTable);

        FormattedResult expectedResult2 = expectedResult1;
        expectedResult2.setEntityList({readEntity, assignEntity});
        expectedResult2.setFinalTable(finalTable);

        FormattedResult expectedResult3 = expectedResult1;
        expectedResult3.setEntityList({readVar});

        FormattedResult expectedResult4 = expectedResult2;
        expectedResult4.setEntityList({readVar, assignStmt});

        FormattedResult testQuery1 = resultProcessor.processResults(query1);
        FormattedResult testQuery2 = resultProcessor.processResults(query2);
        FormattedResult testQuery3 = resultProcessor.processResults(query3);
        FormattedResult testQuery4 = resultProcessor.processResults(query4);

        REQUIRE(testQuery1 == expectedResult1);
        REQUIRE(testQuery2 == expectedResult2);
        REQUIRE(testQuery3 == expectedResult3);
        REQUIRE(testQuery4 == expectedResult4);
    }
}
 */
