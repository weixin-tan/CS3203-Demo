//
// Created by Prerthan Munireternam on 27/03/2022.
//
#include <vector>
#include <set>
#include "catch.hpp"
#include "Table.h"
#include "TableRow.h"
#include "Result.h"


//create entities

Entity procSyn1Table = Entity(EntityType::PROCEDURE, "p1");
Entity procSyn2Table = Entity(EntityType::PROCEDURE, "p2");

Entity varSynTable = Entity(EntityType::VARIABLE, "v");
Entity constSynTable = Entity(EntityType::CONSTANT, "c");

Entity stmtSynTable = Entity(EntityType::STATEMENT, "s");
Entity printSynTable = Entity(EntityType::PRINT, "p");
Entity readSynTable = Entity(EntityType::READ, "r");
Entity assignSynTable = Entity(EntityType::ASSIGNMENT, "a");
Entity callSynTable = Entity(EntityType::CALL, "call");
Entity whileSynTable = Entity(EntityType::WHILE, "w");
Entity ifSynTable = Entity(EntityType::IF, "ifs");

Entity fixedStrVarXTable = Entity(EntityType::FIXED_STRING, "x");
Entity fixedStrVarYTable = Entity(EntityType::FIXED_STRING, "y");
Entity fixedStrVarZTable = Entity(EntityType::FIXED_STRING, "z");
Entity fixedStrProcFTable = Entity(EntityType::FIXED_STRING, "f");
Entity fixedStrProcGTable = Entity(EntityType::FIXED_STRING, "g");

Entity fixedIntStmtTable = Entity(EntityType::FIXED_INTEGER, "1");
Entity fixedIntPrintTable = Entity(EntityType::FIXED_INTEGER, "11");
Entity fixedIntReadTable = Entity(EntityType::FIXED_INTEGER, "5");
Entity fixedIntAssignTable = Entity(EntityType::FIXED_INTEGER, "2");
Entity fixedIntWhileTable = Entity(EntityType::FIXED_INTEGER, "4");
Entity fixedIntCallTable = Entity(EntityType::FIXED_INTEGER, "12");
Entity fixedIntIfTable = Entity(EntityType::FIXED_INTEGER, "6");

Entity wcTable = Entity(EntityType::WILDCARD, "_");

//create ProgramElements
ProgramElement s1Table = ProgramElement::createStatement(ElementType::ASSIGNMENT, 1);
ProgramElement s2Table = ProgramElement::createStatement(ElementType::ASSIGNMENT, 2);
ProgramElement s3Table = ProgramElement::createStatement(ElementType::ASSIGNMENT, 3);
ProgramElement s4Table = ProgramElement::createStatement(ElementType::WHILE, 4);
ProgramElement s5Table = ProgramElement::createStatement(ElementType::READ, 5, "z");
ProgramElement s6Table = ProgramElement::createStatement(ElementType::IF, 6);
ProgramElement s7Table = ProgramElement::createStatement(ElementType::ASSIGNMENT, 7);
ProgramElement s8Table = ProgramElement::createStatement(ElementType::ASSIGNMENT, 8);
ProgramElement s9Table = ProgramElement::createStatement(ElementType::ASSIGNMENT, 9);
ProgramElement s10Table = ProgramElement::createStatement(ElementType::ASSIGNMENT, 10);
ProgramElement s11Table = ProgramElement::createStatement(ElementType::PRINT, 11, "x");


/*
TEST_CASE("creating results"){

    //create results
    Result result1;
    result1.setResultType(ResultType::SUCH_THAT_CLAUSE);
    result1.setValid(true);
    result1.setTwoSynEntities(std::pair<Entity, Entity>(procSyn1Table, varSynTable));
    std::set<std::pair<ProgramElement, ProgramElement>> result1Elements;
    result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createVariable("x")));
    result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createVariable("y")));
    result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createVariable("z")));
    result1.setTwoSynSet(result1Elements);

    Result result2;
    result2.setResultType(ResultType::SUCH_THAT_CLAUSE);
    result2.setValid(true);
    result2.setOneSynEntity(varSynTable);
    std::set<ProgramElement> result2Elements;
    result2Elements.insert(ProgramElement::createVariable("x"));
    result2Elements.insert(ProgramElement::createVariable("z"));
    result2.setOneSynSet(result2Elements);

    Result result3;
    result3.setResultType(ResultType::SUCH_THAT_CLAUSE);
    result3.setValid(true);
    result3.setTwoSynEntities(std::pair<Entity, Entity> (readSynTable, varSynTable));
    //result3.setTwoS2ynEntities(std::pair<Entity, Entity>(readSyn, varSyn));
    std::set<std::pair<ProgramElement, ProgramElement>> result3Elements;
    result3Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::READ, 5), ProgramElement::createVariable("z")));
    result3.setTwoSynSet(result3Elements);

    Result result4;
    result4.setResultType(ResultType::SUCH_THAT_CLAUSE);
    result4.setValid(true);
    result4.setOneSynEntity(varSynTable);
    std::set<ProgramElement> result4Elements;
    result4Elements.insert(ProgramElement::createVariable("x"));
    result4.setOneSynSet(result4Elements);

    Result result5;
    result5.setResultType(ResultType::SUCH_THAT_CLAUSE);
    result5.setValid(true);
    result5.setOneSynEntity(procSyn1Table);
    std::set<ProgramElement> result5Elements;
    result5Elements.insert(ProgramElement::createProcedure("f"));
    result5.setOneSynSet(result5Elements);

    Result result6;
    result6.setResultType(ResultType::SUCH_THAT_CLAUSE);
    result6.setValid(true);
    result6.setOneSynEntity(ifSynTable);
    std::set<ProgramElement> result6Elements;
    result6Elements.insert(ProgramElement::createStatement(ElementType::IF, 6));
    result6.setOneSynSet(result6Elements);

    Result result7;
    result7.setResultType(ResultType::SUCH_THAT_CLAUSE);
    result7.setValid(true);
    result7.setOneSynEntity(procSyn1Table);
    std::set<ProgramElement> result7Elements;
    result7Elements.insert(ProgramElement::createProcedure("f"));
    result7.setOneSynSet(result7Elements);

    Result result8;
    result8.setResultType(ResultType::SUCH_THAT_CLAUSE);
    result8.setValid(true);
    result8.setOneSynEntity(stmtSynTable);
    std::set<ProgramElement> result8Elements;
    result8Elements.insert(ProgramElement::createStatement(ElementType::STATEMENT, 1));
    result8Elements.insert(ProgramElement::createStatement(ElementType::STATEMENT, 2));
    result8Elements.insert(ProgramElement::createStatement(ElementType::STATEMENT, 3));
    result8Elements.insert(ProgramElement::createStatement(ElementType::STATEMENT, 5));
    result8Elements.insert(ProgramElement::createStatement(ElementType::STATEMENT, 7));
    result8Elements.insert(ProgramElement::createStatement(ElementType::STATEMENT, 8));
    result8Elements.insert(ProgramElement::createStatement(ElementType::STATEMENT, 9));
    result8Elements.insert(ProgramElement::createStatement(ElementType::STATEMENT, 10));
    result8.setOneSynSet(result8Elements);


    SECTION("Merger 2 1 syn"){
        Table t1(&result2);
        Table t2(&result2);
        Table tResult(&t1, &t2);
        std::unordered_set<TableRow, TableRowHash> foo = tResult.rows;
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row;
        std::set<ProgramElement> oneSynSet = result2.getOneSynSet();
        for (const ProgramElement& elem : oneSynSet) {
            result2.getOneSynEntity().clear_aType();
            row.insert({ {std::make_pair(result2.getOneSynEntity(), elem)} });
        }
        REQUIRE((foo.find(row) != foo.end()) == true );
    }


    SECTION("Merger 2 1 syn"){
        Table t1(&result4);
        Table t2(&result4);
        Table tResult(&t1, &t2);
        std::unordered_set<TableRow, TableRowHash> foo = tResult.rows;
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row;
        std::set<ProgramElement> oneSynSet = result4.getOneSynSet();
        for (const ProgramElement& elem : oneSynSet) {
            result4.getOneSynEntity().clear_aType();
            row.insert({ {std::make_pair(result4.getOneSynEntity(), elem)} });
        }
        REQUIRE((foo.find(row) != foo.end()) == true );
    }

    SECTION("Merger 2 1 syn"){
        Table t1(&result5);
        Table t2(&result5);
        Table tResult(&t1, &t2);
        std::unordered_set<TableRow, TableRowHash> foo = tResult.rows;
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row;
        std::set<ProgramElement> oneSynSet = result5.getOneSynSet();
        for (const ProgramElement& elem : oneSynSet) {
            result5.getOneSynEntity().clear_aType();
            row.insert({ {std::make_pair(result5.getOneSynEntity(), elem)} });
        }
        REQUIRE((foo.find(row) != foo.end()) == true );
    }

    SECTION("Merger 2 1 syn"){
        Table t1(&result6);
        Table t2(&result6);
        Table tResult(&t1, &t2);
        std::unordered_set<TableRow, TableRowHash> foo = tResult.rows;
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row;
        std::set<ProgramElement> oneSynSet = result6.getOneSynSet();
        for (const ProgramElement& elem : oneSynSet) {
            result6.getOneSynEntity().clear_aType();
            row.insert({ {std::make_pair(result6.getOneSynEntity(), elem)} });
        }
        REQUIRE((foo.find(row) != foo.end()) == true );
    }

    SECTION("Merger 2 1 syn"){
        Table t1(&result7);
        Table t2(&result7);
        Table tResult(&t1, &t2);
        std::unordered_set<TableRow, TableRowHash> foo = tResult.rows;
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row;
        std::set<ProgramElement> oneSynSet = result7.getOneSynSet();
        for (const ProgramElement& elem : oneSynSet) {
            result7.getOneSynEntity().clear_aType();
            row.insert({ {std::make_pair(result7.getOneSynEntity(), elem)} });
        }
        REQUIRE((foo.find(row) != foo.end()) == true );
    }

    SECTION("Merger 2 1 syn"){
        Table t1(&result8);
        Table t2(&result8);
        Table tResult(&t1, &t2);
        std::unordered_set<TableRow, TableRowHash> foo = tResult.rows;
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row;
        std::set<ProgramElement> oneSynSet = result8.getOneSynSet();
        for (const ProgramElement& elem : oneSynSet) {
            result8.getOneSynEntity().clear_aType();
            row.insert({ {std::make_pair(result8.getOneSynEntity(), elem)} });
        }
        REQUIRE((foo.find(row) != foo.end()) == true );
    }

    SECTION("Merger 2 1 syn"){
        Table t1(&result4);
        Table t2(&result5);
        Table tResult(&t1, &t2);
        std::unordered_set<TableRow, TableRowHash> foo = tResult.rows;
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row1;
        std::set<ProgramElement> oneSynSet1 = result4.getOneSynSet();
        for (const ProgramElement& elem : oneSynSet1) {
            result4.getOneSynEntity().clear_aType();
            row1.insert({ {std::make_pair(result4.getOneSynEntity(), elem)} });
        }
        REQUIRE((foo.find(row1) != foo.end()) == true );
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row2;
        std::set<ProgramElement> oneSynSet2 = result5.getOneSynSet();
        for (const ProgramElement& elem : oneSynSet2) {
            result5.getOneSynEntity().clear_aType();
            row2.insert({ {std::make_pair(result5.getOneSynEntity(), elem)} });
        }
        REQUIRE((foo.find(row2) != foo.end()) == true );
    }

    SECTION("Merger 2 1 syn"){
        Table t1(&result4);
        Table t2(&result6);
        Table tResult(&t1, &t2);
        std::unordered_set<TableRow, TableRowHash> foo = tResult.rows;
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row1;
        std::set<ProgramElement> oneSynSet1 = result4.getOneSynSet();
        for (const ProgramElement& elem : oneSynSet1) {
            result4.getOneSynEntity().clear_aType();
            row1.insert({ {std::make_pair(result8.getOneSynEntity(), elem)} });
        }
        REQUIRE((foo.find(row1) != foo.end()) == true );
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row2;
        std::set<ProgramElement> oneSynSet2 = result6.getOneSynSet();
        for (const ProgramElement& elem : oneSynSet2) {
            result6.getOneSynEntity().clear_aType();
            row2.insert({ {std::make_pair(result6.getOneSynEntity(), elem)} });
        }
        REQUIRE((foo.find(row2) != foo.end()) == true );
    }

    SECTION("Merger 2 1 syn"){
        Table t1(&result4);
        Table t2(&result7);
        Table tResult(&t1, &t2);
        std::unordered_set<TableRow, TableRowHash> foo = tResult.rows;
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row1;
        std::set<ProgramElement> oneSynSet1 = result4.getOneSynSet();
        for (const ProgramElement& elem : oneSynSet1) {
            result4.getOneSynEntity().clear_aType();
            row1.insert({ {std::make_pair(result8.getOneSynEntity(), elem)} });
        }
        REQUIRE((foo.find(row1) != foo.end()) == true );
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row2;
        std::set<ProgramElement> oneSynSet2 = result7.getOneSynSet();
        for (const ProgramElement& elem : oneSynSet2) {
            result7.getOneSynEntity().clear_aType();
            row2.insert({ {std::make_pair(result7.getOneSynEntity(), elem)} });
        }
        REQUIRE((foo.find(row2) != foo.end()) == true );
    }

    SECTION("Merger 2 1 syn"){
        Table t1(&result4);
        Table t2(&result8);
        Table tResult(&t1, &t2);
        std::unordered_set<TableRow, TableRowHash> foo = tResult.rows;
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row1;
        std::set<ProgramElement> oneSynSet1 = result4.getOneSynSet();
        for (const ProgramElement& elem : oneSynSet1) {
            result4.getOneSynEntity().clear_aType();
            row1.insert({ {std::make_pair(result8.getOneSynEntity(), elem)} });
        }
        REQUIRE((foo.find(row1) != foo.end()) == true );
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row2;
        std::set<ProgramElement> oneSynSet2 = result8.getOneSynSet();
        for (const ProgramElement& elem : oneSynSet2) {
            result8.getOneSynEntity().clear_aType();
            row2.insert({ {std::make_pair(result8.getOneSynEntity(), elem)} });
        }
        REQUIRE((foo.find(row2) != foo.end()) == true );
    }

    SECTION("Merger 2 1 syn"){
        Table t1(&result5);
        Table t2(&result6);
        Table tResult(&t1, &t2);
        std::unordered_set<TableRow, TableRowHash> foo = tResult.rows;
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row1;
        std::set<ProgramElement> oneSynSet1 = result5.getOneSynSet();
        for (const ProgramElement& elem : oneSynSet1) {
            result5.getOneSynEntity().clear_aType();
            row1.insert({ {std::make_pair(result5.getOneSynEntity(), elem)} });
        }
        REQUIRE((foo.find(row1) != foo.end()) == true );
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row2;
        std::set<ProgramElement> oneSynSet2 = result6.getOneSynSet();
        for (const ProgramElement& elem : oneSynSet2) {
            result6.getOneSynEntity().clear_aType();
            row2.insert({ {std::make_pair(result6.getOneSynEntity(), elem)} });
        }
        REQUIRE((foo.find(row2) != foo.end()) == true );
    }

    SECTION("Merger 2 1 syn"){
        Table t1(&result7);
        Table t2(&result5);
        Table tResult(&t1, &t2);
        std::unordered_set<TableRow, TableRowHash> foo = tResult.rows;
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row1;
        std::set<ProgramElement> oneSynSet1 = result7.getOneSynSet();
        for (const ProgramElement& elem : oneSynSet1) {
            result7.getOneSynEntity().clear_aType();
            row1.insert({ {std::make_pair(result7.getOneSynEntity(), elem)} });
        }
        REQUIRE((foo.find(row1) != foo.end()) == true );
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row2;
        std::set<ProgramElement> oneSynSet2 = result5.getOneSynSet();
        for (const ProgramElement& elem : oneSynSet2) {
            result5.getOneSynEntity().clear_aType();
            row2.insert({ {std::make_pair(result5.getOneSynEntity(), elem)} });
        }
        REQUIRE((foo.find(row2) != foo.end()) == true );
    }

    SECTION("Merger 2 1 syn"){
        Table t1(&result8);
        Table t2(&result5);
        Table tResult(&t1, &t2);
        std::unordered_set<TableRow, TableRowHash> foo = tResult.rows;
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row1;
        std::set<ProgramElement> oneSynSet1 = result8.getOneSynSet();
        for (const ProgramElement& elem : oneSynSet1) {
            result8.getOneSynEntity().clear_aType();
            row1.insert({ {std::make_pair(result8.getOneSynEntity(), elem)} });
        }
        REQUIRE((foo.find(row1) != foo.end()) == true );
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row2;
        std::set<ProgramElement> oneSynSet2 = result5.getOneSynSet();
        for (const ProgramElement& elem : oneSynSet2) {
            result5.getOneSynEntity().clear_aType();
            row2.insert({ {std::make_pair(result5.getOneSynEntity(), elem)} });
        }
        REQUIRE((foo.find(row2) != foo.end()) == true );
    }

    SECTION("Merger 2 1 syn") {
        Table t1(&result8);
        Table t2(&result6);
        Table tResult(&t1, &t2);
        std::unordered_set<TableRow, TableRowHash> foo = tResult.rows;
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row1;
        std::set<ProgramElement> oneSynSet1 = result8.getOneSynSet();
        for (const ProgramElement &elem: oneSynSet1) {
            result8.getOneSynEntity().clear_aType();
            row1.insert({{std::make_pair(result8.getOneSynEntity(), elem)}});
        }
        REQUIRE((foo.find(row1) != foo.end()) == true);
        std::unordered_map<Entity, ProgramElement, EntityHashFunction> row2;
        std::set<ProgramElement> oneSynSet2 = result6.getOneSynSet();
        for (const ProgramElement &elem: oneSynSet2) {
            result6.getOneSynEntity().clear_aType();
            row2.insert({{std::make_pair(result6.getOneSynEntity(), elem)}});
        }
        REQUIRE((foo.find(row2) != foo.end()) == true);
    }

}
*/