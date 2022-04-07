#include <regex>
#include <unordered_map>

#include "catch.hpp"
#include "Entity.h"
#include "Hashing.h"

//create entities
Entity procSyn1hash = Entity(EntityType::PROCEDURE, "p1");
Entity procSyn2hash = Entity(EntityType::PROCEDURE, "p2");

Entity varSynhash= Entity(EntityType::VARIABLE, "v");
Entity constSynhash = Entity(EntityType::CONSTANT, "c");

Entity stmtSynhash = Entity(EntityType::STATEMENT, "s");
Entity printSynhash = Entity(EntityType::PRINT, "p");
Entity readSynhash = Entity(EntityType::READ, "r");
Entity assignSynhash = Entity(EntityType::ASSIGNMENT, "a");
Entity callSynhash = Entity(EntityType::CALL, "call");
Entity whileSynhash = Entity(EntityType::WHILE, "w");
Entity ifSynhash = Entity(EntityType::IF, "ifs");

Entity fixedStrVarXhash = Entity(EntityType::FIXED_STRING, "x");
Entity fixedStrVarYhash = Entity(EntityType::FIXED_STRING, "y");
Entity fixedStrVarZhash = Entity(EntityType::FIXED_STRING, "z");
Entity fixedStrProcFhash = Entity(EntityType::FIXED_STRING, "f");
Entity fixedStrProcGhash = Entity(EntityType::FIXED_STRING, "g");

Entity fixedIntStmthash = Entity(EntityType::FIXED_INTEGER, "1");
Entity fixedIntPrinthash = Entity(EntityType::FIXED_INTEGER, "11");
Entity fixedIntReadhash = Entity(EntityType::FIXED_INTEGER, "5");
Entity fixedIntAssignhash = Entity(EntityType::FIXED_INTEGER, "2");
Entity fixedIntWhilehash = Entity(EntityType::FIXED_INTEGER, "4");
Entity fixedIntCallhash = Entity(EntityType::FIXED_INTEGER, "12");
Entity fixedIntIfhash = Entity(EntityType::FIXED_INTEGER, "6");

Entity wchash = Entity(EntityType::WILDCARD, "_");

//24 entities, let n=30

TEST_CASE("basic methods for hashing"){


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
        REQUIRE(hashing.hashFunctionForEntities(procSyn1hash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(procSyn1hash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(procSyn1hash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(procSyn2hash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(procSyn2hash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(procSyn2hash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(varSynhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(varSynhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(varSynhash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(constSynhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(constSynhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(constSynhash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(stmtSynhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(stmtSynhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(stmtSynhash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(printSynhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(printSynhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(printSynhash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(readSynhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(readSynhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(readSynhash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(assignSynhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(assignSynhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(assignSynhash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(callSynhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(callSynhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(callSynhash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(whileSynhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(whileSynhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(whileSynhash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(ifSynhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(ifSynhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(ifSynhash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedStrVarXhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedStrVarXhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedStrVarXhash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedStrVarYhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedStrVarYhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedStrVarYhash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedStrVarZhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedStrVarZhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedStrVarZhash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedStrProcFhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedStrProcFhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedStrProcFhash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedStrProcGhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedStrProcGhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedStrProcGhash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedIntStmthash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntStmthash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntStmthash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedIntPrinthash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntPrinthash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntPrinthash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedIntReadhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntReadhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntReadhash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedIntAssignhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntAssignhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntAssignhash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedIntWhilehash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntWhilehash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntWhilehash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedIntCallhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntCallhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntCallhash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(fixedIntIfhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntIfhash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(fixedIntIfhash) > 0);

        REQUIRE(hashing.hashFunctionForEntities(wchash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(wchash) > 0);
        REQUIRE(hashing.hashFunctionForEntities(wchash) > 0);

    }

}
