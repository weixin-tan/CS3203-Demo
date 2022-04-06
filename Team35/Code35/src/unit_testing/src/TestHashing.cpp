#include <regex>
#include <unordered_map>

#include "catch.hpp"
#include "Entity.h"
#include "Hashing.h"

//create entities
Entity procSyn1 = Entity(EntityType::PROCEDURE, "p1");
Entity procSyn2 = Entity(EntityType::PROCEDURE, "p2");

Entity varSyn = Entity(EntityType::VARIABLE, "v");
Entity constSyn = Entity(EntityType::CONSTANT, "c");

Entity stmtSyn = Entity(EntityType::STATEMENT, "s");
Entity printSyn = Entity(EntityType::PRINT, "p");
Entity readSyn = Entity(EntityType::READ, "r");
Entity assignSyn = Entity(EntityType::ASSIGNMENT, "a");
Entity callSyn = Entity(EntityType::CALL, "call");
Entity whileSyn = Entity(EntityType::WHILE, "w");
Entity ifSyn = Entity(EntityType::IF, "ifs");

Entity fixedStrVarX = Entity(EntityType::FIXED_STRING, "x");
Entity fixedStrVarY = Entity(EntityType::FIXED_STRING, "y");
Entity fixedStrVarZ = Entity(EntityType::FIXED_STRING, "z");
Entity fixedStrProcF = Entity(EntityType::FIXED_STRING, "f");
Entity fixedStrProcG = Entity(EntityType::FIXED_STRING, "g");

Entity fixedIntStmt = Entity(EntityType::FIXED_INTEGER, "1");
Entity fixedIntPrint = Entity(EntityType::FIXED_INTEGER, "11");
Entity fixedIntRead = Entity(EntityType::FIXED_INTEGER, "5");
Entity fixedIntAssign = Entity(EntityType::FIXED_INTEGER, "2");
Entity fixedIntWhile = Entity(EntityType::FIXED_INTEGER, "4");
Entity fixedIntCall = Entity(EntityType::FIXED_INTEGER, "12");
Entity fixedIntIf = Entity(EntityType::FIXED_INTEGER, "6");

Entity wc = Entity(EntityType::WILDCARD, "_");

//24 entities, let n=30

TEST_CASE("basic methods"){

    //create hashing object
    Hashing hashing;

    SECTION("testing set hashing objecst"){
        //test for positive
        hashing.setTotalEntitiesEstimate(1);
        REQUIRE(hashing.getTotalEntitiesEstimate() == 1);
        hashing.setTotalEntitiesEstimate(2);
        REQUIRE(hashing.getTotalEntitiesEstimate() == 2);
        hashing.setTotalEntitiesEstimate(3);
        REQUIRE(hashing.getTotalEntitiesEstimate() == 3);
        hashing.setTotalEntitiesEstimate(4);
        REQUIRE(hashing.getTotalEntitiesEstimate() == 4);
        hashing.setTotalEntitiesEstimate(5);
        REQUIRE(hashing.getTotalEntitiesEstimate() == 5);
        hashing.setTotalEntitiesEstimate(6);
        REQUIRE(hashing.getTotalEntitiesEstimate() == 6);
        hashing.setTotalEntitiesEstimate(7);
        REQUIRE(hashing.getTotalEntitiesEstimate() == 7);
        hashing.setTotalEntitiesEstimate(8);
        REQUIRE(hashing.getTotalEntitiesEstimate() == 8);
        hashing.setTotalEntitiesEstimate(9);
        REQUIRE(hashing.getTotalEntitiesEstimate() == 9);
        hashing.setTotalEntitiesEstimate(10);
        REQUIRE(hashing.getTotalEntitiesEstimate() == 10);
        hashing.setTotalEntitiesEstimate(11);

        //test for negative
        REQUIRE(hashing.getTotalEntitiesEstimate() != 10);
        hashing.setTotalEntitiesEstimate(12);
        REQUIRE(hashing.getTotalEntitiesEstimate() != 10);
        hashing.setTotalEntitiesEstimate(13);
        REQUIRE(hashing.getTotalEntitiesEstimate() != 10);
        hashing.setTotalEntitiesEstimate(14);
        REQUIRE(hashing.getTotalEntitiesEstimate() != 10);
        hashing.setTotalEntitiesEstimate(15);
        REQUIRE(hashing.getTotalEntitiesEstimate() != 10);
        hashing.setTotalEntitiesEstimate(16);
        REQUIRE(hashing.getTotalEntitiesEstimate() != 10);
        hashing.setTotalEntitiesEstimate(17);
        REQUIRE(hashing.getTotalEntitiesEstimate() != 10);
        hashing.setTotalEntitiesEstimate(18);
        REQUIRE(hashing.getTotalEntitiesEstimate() != 10);
        hashing.setTotalEntitiesEstimate(19);
        REQUIRE(hashing.getTotalEntitiesEstimate() != 10);
        hashing.setTotalEntitiesEstimate(20);
        REQUIRE(hashing.getTotalEntitiesEstimate() != 10);
    }

    SECTION("checking addSpace method"){
        hashing.setTotalEntitiesEstimate(30);
        REQUIRE(hashing.hashFunctionForEntities(procSyn1) > 0);
        REQUIRE(hashing.hashFunctionForEntities(procSyn1) > 0);
        REQUIRE(hashing.hashFunctionForEntities(procSyn1) > 0);

        REQUIRE(hashing.hashFunctionForEntities(procSyn2) > 0);
        REQUIRE(hashing.hashFunctionForEntities(procSyn2) > 0);
        REQUIRE(hashing.hashFunctionForEntities(procSyn2) > 0);

        REQUIRE(hashing.hashFunctionForEntities(varSyn) > 0);
        REQUIRE(hashing.hashFunctionForEntities(varSyn) > 0);
        REQUIRE(hashing.hashFunctionForEntities(varSyn) > 0);

        REQUIRE(hashing.hashFunctionForEntities(constSyn) > 0);
        REQUIRE(hashing.hashFunctionForEntities(constSyn) > 0);
        REQUIRE(hashing.hashFunctionForEntities(constSyn) > 0);

        REQUIRE(hashing.hashFunctionForEntities(stmtSyn) > 0);
        REQUIRE(hashing.hashFunctionForEntities(stmtSyn) > 0);
        REQUIRE(hashing.hashFunctionForEntities(stmtSyn) > 0);

        REQUIRE(hashing.hashFunctionForEntities(printSyn) > 0);
        REQUIRE(hashing.hashFunctionForEntities(printSyn) > 0);
        REQUIRE(hashing.hashFunctionForEntities(printSyn) > 0);

        REQUIRE(hashing.hashFunctionForEntities(readSyn) > 0);
        REQUIRE(hashing.hashFunctionForEntities(readSyn) > 0);
        REQUIRE(hashing.hashFunctionForEntities(readSyn) > 0);

        REQUIRE(hashing.hashFunctionForEntities(assignSyn) > 0);
        REQUIRE(hashing.hashFunctionForEntities(assignSyn) > 0);
        REQUIRE(hashing.hashFunctionForEntities(assignSyn) > 0);

        REQUIRE(hashing.hashFunctionForEntities(callSyn) > 0);
        REQUIRE(hashing.hashFunctionForEntities(callSyn) > 0);
        REQUIRE(hashing.hashFunctionForEntities(callSyn) > 0);

        REQUIRE(hashing.hashFunctionForEntities(whileSyn) > 0);
        REQUIRE(hashing.hashFunctionForEntities(whileSyn) > 0);
        REQUIRE(hashing.hashFunctionForEntities(whileSyn) > 0);

        REQUIRE(hashing.hashFunctionForEntities(ifSyn) > 0);
        REQUIRE(hashing.hashFunctionForEntities(ifSyn) > 0);
        REQUIRE(hashing.hashFunctionForEntities(ifSyn) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedStrVarX) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedStrVarX) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedStrVarX) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedStrVarY) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedStrVarY) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedStrVarY) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedStrVarZ) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedStrVarZ) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedStrVarZ) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedStrProcF) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedStrProcF) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedStrProcF) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedStrProcG) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedStrProcG) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedStrProcG) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedIntStmt) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntStmt) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntStmt) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedIntPrint) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntPrint) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntPrint) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedIntRead) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntRead) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntRead) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedIntAssign) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntAssign) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntAssign) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedIntWhile) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntWhile) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntWhile) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedIntCall) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntCall) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntCall) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedIntIf) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntIf) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntIf) > 0);

        REQUIRE(hashing.hashFunctionForEntities(wc) > 0);
        REQUIRE(hashing.hashFunctionForEntities(wc) > 0);
        REQUIRE(hashing.hashFunctionForEntities(wc) > 0);

    }

}
