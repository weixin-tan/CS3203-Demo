#ifndef SPA_SRC_SPA_SRC_QPS_PREOPTIMISER_H_
#define SPA_SRC_SPA_SRC_QPS_PREOPTIMISER_H_

#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include "Clause.h"
#include "RelationshipRef.h"
#include "RelationshipRefGroup.h"
#include "GroupedClause.h"
#include "RelationshipInfo.h"
#include <set>

class PreOptimiser {
public:
    PreOptimiser();
    GroupedClause optimise(const std::vector<Clause>& clauses);
    std::vector<RelationshipRefGroup> groupRelationships(const std::vector<RelationshipRef>& relationships);
private:
    static std::unordered_map<RelationshipType, int> easeOfRelationshipMap;
    static int numberOfCommonSynoymnsIndex;
    static int numberOfFixedEntitiesIndex;
    static int relationshipEaseIndex;
    static int relationshipNumberIndex;

    static bool rankingPredicate(const RelationshipInfo& a, const RelationshipInfo& b);


    static bool priorityQueueComparator(const std::vector<int>& a, const std::vector<int>& b);

    static bool isFixedEntity(const Entity& e);
    static bool isFixedEntityOrWildcard(const Entity& e);
    static bool entityMatchRelRef(const Entity& e, const RelationshipRef& r);

    static int countNumberOfFixedEntities(const RelationshipRef& r);

    static void findConnectedRelationshipsAndAdd(int index1,
                                                 std::unordered_map<int, std::vector<std::pair<int,int>>>* adjacencyList,
                                                 const std::vector<RelationshipRef>& relationships);
    static void addRelationshipIfConnected(int index1, int index2, int numberOfEntitiesRelated,
                                           std::unordered_map<int, std::vector<std::pair<int,int>>>* adjacencyList);
    static int checkRelationshipsConnected(const RelationshipRef& r1, const RelationshipRef& r2);

    static int notVisitedYet(std::vector<RelationshipInfo>* rankings);

    static int findRelationshipInfo(std::vector<RelationshipInfo>* rankings, int relationshipNumber);

    static void traverseNeighbours(int currentRelationship,
                                          std::vector<RelationshipInfo>* rankings,
                                          std::unordered_map<int, std::vector<std::pair<int,int>>>* adjacencyList,
                                          std::priority_queue<std::vector<int>, std::vector<std::vector<int>>, bool (*)(const std::vector<int>&, const std::vector<int>&)>* priorityQueue);

    static RelationshipRefGroup traverseGraph(const std::vector<RelationshipRef>& relationships,
                                            std::vector<RelationshipInfo>* rankings,
                                            std::unordered_map<int, std::vector<std::pair<int,int>>>* adjacencyList);
};

#endif //SPA_SRC_SPA_SRC_QPS_PREOPTIMISER_H_
