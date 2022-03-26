#ifndef SPA_SRC_SPA_SRC_QPS_PREOPTIMISER_H_
#define SPA_SRC_SPA_SRC_QPS_PREOPTIMISER_H_

#include <iostream>
#include <vector>
#include <queue>
#include "Clause.h"
#include "RelationshipRef.h"
#include "RelationshipRefGroup.h"
#include "GroupedClause.h"

class PreOptimiser {
public:
    PreOptimiser();
    GroupedClause optimise(const std::vector<Clause>& clauses);
    std::vector<RelationshipRefGroup> groupRelationships(const std::vector<RelationshipRef> & relationships);
private:
    static static bool isFixedEntity(const Entity& e);
    static bool isFixedEntityOrWildcard(const Entity& e);
    static bool entityMatchRelRef(const Entity& e, const RelationshipRef& r);
    static bool checkRelationshipsConnected(const RelationshipRef& r1, const RelationshipRef& r2);
    static int notVisitedYet(std::vector<int> visited);
    static void addRelationshipToAdjacencyList(int index1, std::unordered_map<int, std::vector<int>>* adjacencyList, const std::vector<RelationshipRef>& relationships);
    static void traverseGraph(std::queue<int> *myQueue, std::vector<int> *visited, RelationshipRefGroup *tempGroup,
                       const std::vector<RelationshipRef>& relationships, std::unordered_map<int,
                       std::vector<int>> *adjacencyList);
    void sortGroup(RelationshipRefGroup *tempGroup);
    static std::string listToString(const std::vector<int>& ls);
    static void printAdjacencyList(const std::unordered_map<int, std::vector<int>>& adjacencyList);
    };

#endif //SPA_SRC_SPA_SRC_QPS_PREOPTIMISER_H_
