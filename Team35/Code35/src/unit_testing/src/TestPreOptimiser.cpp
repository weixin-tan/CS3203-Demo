#include <vector>
#include <iostream>
#include "PreOptimiser.h"
#include "catch.hpp"

using namespace std;

void printGroups(std::vector<RelationshipRefGroup> groups){
    int i = 0;
    for (auto group: groups){
        cout << "group " << i << "\n";
        for (auto relationship: group.relRefGroup){
            cout << relationship.toString();
        }
        i = i + 1;
    }
}

bool compareTwoLists(RelationshipRefGroup r1, RelationshipRefGroup r2){
    if (r1.relRefGroup.size() != r2.relRefGroup.size()){
        return false;
    }else{
        for (int i=0; i<r1.relRefGroup.size(); i++){
            if (!r1.relRefGroup[i].equals(r2.relRefGroup[i])){
                return false;
            }
        }
        return true;
    }
}

bool compareTwoGroupsLists(std::vector<RelationshipRefGroup> g1, std::vector<RelationshipRefGroup> g2){
    if (g1.size() != g2.size()){
        return false;
    }else{
        for (int i=0; i<g1.size(); i++){
            if (!compareTwoLists(g1[i], g2[i])){
                return false;
            }
        }
        return true;
    }
}

TEST_CASE("Testing Linear Graphs"){
    PreOptimiser preOp = PreOptimiser();
    Entity e1 = Entity(EntityType::FIXED_INTEGER, "1");
    Entity v = Entity(EntityType::VARIABLE, "v");
    RelationshipRef r1 = RelationshipRef(RelationshipType::USES, e1, v);

    Entity e2 = Entity(EntityType::FIXED_INTEGER, "2");
    RelationshipRef r2 = RelationshipRef(RelationshipType::MODIFIES, v, e2);

    Entity a1 = Entity(EntityType::ASSIGNMENT, "a1");
    Entity expr1 = Entity(EntityType::FIXED_STRING, "count + 1");
    RelationshipRef r3 = RelationshipRef(RelationshipType::PATTERN, v, expr1, a1);

    Entity a1Attribute = Entity(EntityType::ASSIGNMENT, "a1", EntityAttributeType::STMT);
    RelationshipRef r4 = RelationshipRef(RelationshipType::WITH, a1Attribute, e1);

    Entity a = Entity(EntityType::ASSIGNMENT, "a");
    Entity wildcard = Entity(EntityType::WILDCARD, "_");
    RelationshipRef r5 = RelationshipRef(RelationshipType::PATTERN, wildcard, expr1, a);

    Entity x = Entity(EntityType::FIXED_INTEGER, "x");
    Entity v1 = Entity(EntityType::VARIABLE, "v1");
    RelationshipRef r6 = RelationshipRef(RelationshipType::USES, a, v1);

    Entity v1Attribute = Entity(EntityType::VARIABLE, "v1", EntityAttributeType::VARNAME);
    RelationshipRef r7 = RelationshipRef(RelationshipType::WITH, e1, v1Attribute);

    RelationshipRef r8 = RelationshipRef(RelationshipType::WITH, e1, e2);

    vector<RelationshipRef> relationshipList1;
    relationshipList1.push_back(r1);
    relationshipList1.push_back(r5);
    relationshipList1.push_back(r2);
    relationshipList1.push_back(r6);
    relationshipList1.push_back(r3);
    relationshipList1.push_back(r7);
    relationshipList1.push_back(r4);
    relationshipList1.push_back(r8);

    RelationshipRefGroup group0;
    RelationshipRefGroup group1;
    RelationshipRefGroup group2;

    group0.addRelRef(r8);
    group1.addRelRef(r1);
    group1.addRelRef(r2);
    group1.addRelRef(r3);
    group1.addRelRef(r4);
    group2.addRelRef(r5);
    group2.addRelRef(r6);
    group2.addRelRef(r7);

    std::vector<RelationshipRefGroup> expected1;
    expected1.push_back(group0);
    expected1.push_back(group1);
    expected1.push_back(group2);

    std::vector<RelationshipRefGroup> output1 = preOp.groupRelationships(relationshipList1);
    printGroups(output1);

    SECTION("testing 2 linear graphs"){
        REQUIRE(compareTwoLists(expected1[0], output1[0]));
        REQUIRE(compareTwoLists(expected1[1], output1[1]));
        REQUIRE(compareTwoLists(expected1[2], output1[2]));
        REQUIRE(compareTwoGroupsLists(expected1, output1));
    }
}