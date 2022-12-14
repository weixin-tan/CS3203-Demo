#include <vector>
#include <iostream>
#include "PreOptimiser.h"
#include "catch.hpp"
#include "TestQueryPreprocessingAndPreOptimiserUtils.h"

using namespace std;

void printGroups(const std::vector<RelationshipRefGroup>& groups) {
    int i = 0;
    for (const auto& group : groups) {
        cout << "group " << i << "\n";
        for (const auto& relationship : group.relRefGroup) {
            cout << relationship.toString();
        }
        i = i + 1;
    }
}

bool compareTwoLists(RelationshipRefGroup r1, RelationshipRefGroup r2) {
    if (r1.relRefGroup.size() != r2.relRefGroup.size()) {
        return false;
    } else {
        for (int i = 0; i < r1.relRefGroup.size(); i++) {
            if (!r1.relRefGroup[i].equals(r2.relRefGroup[i])) {
                return false;
            }
        }
        return true;
    }
}

bool compareTwoGroupsLists(std::vector<RelationshipRefGroup> g1, std::vector<RelationshipRefGroup> g2) {
    if (g1.size() != g2.size()) {
        return false;
    } else {
        for (int i = 0; i < g1.size(); i++) {
            if (!compareTwoLists(g1[i], g2[i])) {
                return false;
            }
        }
        return true;
    }
}

TEST_CASE("Testing Linear Graphs") {
    PreOptimiser preOp = PreOptimiser();
    RelationshipRef r1 = RelationshipRef(RelationshipType::USES, int1, v);
    RelationshipRef r2 = RelationshipRef(RelationshipType::MODIFIES, v, int2);
    RelationshipRef r3 = RelationshipRef(RelationshipType::PATTERN, v, countplus1Entity, a1);
    RelationshipRef r4 = RelationshipRef(RelationshipType::WITH, a1stmt, int1);
    RelationshipRef r5 = RelationshipRef(RelationshipType::PATTERN, wildcard, countplus1Entity, a);
    RelationshipRef r6 = RelationshipRef(RelationshipType::USES, a, v1);
    RelationshipRef r7 = RelationshipRef(RelationshipType::WITH, int1, v1Attribute);
    RelationshipRef r8 = RelationshipRef(RelationshipType::WITH, int1, int2);

    vector<RelationshipRef> relationshipList1 = {r1, r2, r3, r4, r5, r6, r7, r8};
    RelationshipRefGroup group0;
    RelationshipRefGroup group1 = {};
    RelationshipRefGroup group2;

    group0.setRelRefGroup({r8});
    group1.setRelRefGroup({r4, r3, r1, r2});
    group2.setRelRefGroup({r7, r6, r5});
    std::vector<RelationshipRefGroup> expected1 = {group0, group1, group2};
    std::vector<RelationshipRefGroup> output1 = preOp.groupRelationships(relationshipList1);

    preOp.groupRelationships({});
    SECTION("testing 2 linear graphs") {
        REQUIRE(compareTwoGroupsLists(expected1, output1));
    }
}

TEST_CASE("testing more complicate graphs") {
    PreOptimiser preOp = PreOptimiser();
    RelationshipRef r1 = RelationshipRef(RelationshipType::AFFECTS_T, a, a1);
    RelationshipRef r2 = RelationshipRef(RelationshipType::PATTERN, v, expr1, a);
    RelationshipRef r3 = RelationshipRef(RelationshipType::PATTERN, v1, expr1, a1);
    RelationshipRef r4 = RelationshipRef(RelationshipType::WITH, vAttribute, pProcname);
    RelationshipRef r5 = RelationshipRef(RelationshipType::WITH, v1Attribute, p1Procname);
    RelationshipRef r6 = RelationshipRef(RelationshipType::CALLS, p1, p);
    RelationshipRef r7 = RelationshipRef(RelationshipType::FOLLOWS, int1, int2);
    RelationshipRef r8 = RelationshipRef(RelationshipType::MODIFIES, int3, countEntity);
    RelationshipRef r9 = RelationshipRef(RelationshipType::WITH, int1, countEntity);
    vector<RelationshipRef> relationshipList1 = {r1, r2, r3, r4, r5, r6, r7, r8, r9};

    RelationshipRefGroup group0;
    RelationshipRefGroup group1;
    group0.setRelRefGroup({r7, r8, r9});
    group1.setRelRefGroup({r4, r6, r5, r2, r3, r1});

    std::vector<RelationshipRefGroup> expected1 = {group0, group1};
    std::vector<RelationshipRefGroup> output1 = preOp.groupRelationships(relationshipList1);
    SECTION("testing complicated graph") {
        REQUIRE(compareTwoLists(expected1[0], output1[0]));
        REQUIRE(compareTwoLists(expected1[1], output1[1]));
        REQUIRE(compareTwoGroupsLists(expected1, output1));
    }
}

TEST_CASE("test new heuristics") {
    PreOptimiser preOp = PreOptimiser();
    RelationshipRef r1 = RelationshipRef(RelationshipType::USES, a, v);
    RelationshipRef r2 = RelationshipRef(RelationshipType::MODIFIES, int1, v);
    RelationshipRef r3 = RelationshipRef(RelationshipType::FOLLOWS, s, a1);
    RelationshipRef r4 = RelationshipRef(RelationshipType::PARENT_T, a2, s);
    RelationshipRef r5 = RelationshipRef(RelationshipType::FOLLOWS_T, a2, a3);
    RelationshipRef r6 = RelationshipRef(RelationshipType::AFFECTS, a1, a2);

    RelationshipRefGroup group0;
    RelationshipRefGroup group1;
    group0.setRelRefGroup({r2, r1});
    group1.setRelRefGroup({r3, r4, r6, r5});
    std::vector<RelationshipRefGroup> expected1 = {group0, group1};

    vector<RelationshipRef> relationshipList1 = {r1, r2, r3, r4, r5, r6};
    std::vector<RelationshipRefGroup> output1 = preOp.groupRelationships(relationshipList1);
    SECTION("relationships with 1 fixed entity go first, relationships with 2 matching entities go first") {
        REQUIRE(compareTwoGroupsLists(expected1, output1));
    }
}
