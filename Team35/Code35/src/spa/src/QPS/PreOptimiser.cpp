#include "PreOptimiser.h"

PreOptimiser::PreOptimiser() = default;

std::unordered_map<RelationshipType, int> PreOptimiser::easeOfRelationshipMap = {
        {RelationshipType::WITH, 8},
        {RelationshipType::CALLS, 7},
        {RelationshipType::CALLS_T, 7},
        {RelationshipType::PATTERN, 6},
        {RelationshipType::FOLLOWS, 5},
        {RelationshipType::PARENT, 5},
        {RelationshipType::NEXT, 5},
        {RelationshipType::FOLLOWS_T, 4},
        {RelationshipType::PARENT_T, 4},
        {RelationshipType::USES, 3},
        {RelationshipType::MODIFIES, 3},
        {RelationshipType::NEXT_T, 2},
        {RelationshipType::AFFECTS, 2},
        {RelationshipType::AFFECTS_T, 1}
};

bool PreOptimiser::rankingPredicate(const RelationshipInfo& a, const RelationshipInfo& b) {
    if (a.visited < b.visited) {
        return true;
    } else if (a.visited > b.visited) {
        return false;
    }
    if (a.numberOfCommonSynoymns > b.numberOfCommonSynoymns) {
        return true;
    } else if (a.numberOfCommonSynoymns < b.numberOfCommonSynoymns) {
        return false;
    }
    if (a.numberOfFixedEntities > b.numberOfFixedEntities) {
        return true;
    } else if (a.numberOfFixedEntities < b.numberOfFixedEntities) {
        return false;
    }
    if (a.relationshipEase > b.relationshipEase) {
        return true;
    } else if (a.relationshipEase < b.relationshipEase) {
        return false;
    }
    if (a.relationshipNumber < b.relationshipNumber) {
        return true;
    } else {
        return false;
    }
}

int PreOptimiser::numberOfCommonSynoymnsIndex = 0;
int PreOptimiser::numberOfFixedEntitiesIndex = 1;
int PreOptimiser::relationshipEaseIndex = 2;
int PreOptimiser::relationshipNumberIndex = 3;

bool PreOptimiser::priorityQueueComparator(const std::vector<int>& a, const std::vector<int>& b) {
    if (a[numberOfCommonSynoymnsIndex] < b[numberOfCommonSynoymnsIndex]) {
        return true;
    } else if (a[numberOfCommonSynoymnsIndex] > b[numberOfCommonSynoymnsIndex]) {
        return false;
    }
    if (a[numberOfFixedEntitiesIndex] < b[numberOfFixedEntitiesIndex]) {
        return true;
    } else if (a[numberOfFixedEntitiesIndex] > b[numberOfFixedEntitiesIndex]) {
        return false;
    }
    if (a[relationshipEaseIndex] < b[relationshipEaseIndex]) {
        return true;
    } else if (a[relationshipEaseIndex] > b[relationshipEaseIndex]) {
        return false;
    }
    if (a[relationshipNumberIndex] > b[relationshipNumberIndex]) {
        return true;
    } else {
        return false;
    }
}

GroupedClause PreOptimiser::optimise(const std::vector<Clause>& clauses) {
    GroupedClause toReturn = GroupedClause();
    Clause c = clauses[0];
    toReturn.setEntityToFindList(c.entityToFindList);

    if (!c.refList.empty()) {
        toReturn.setRelationshipRefGroup(groupRelationships(c.refList));
    }

    return toReturn;
}

std::vector<RelationshipRefGroup> PreOptimiser::groupRelationships(const std::vector<RelationshipRef>& relationships) {
    std::vector<RelationshipRefGroup> groups;
    RelationshipRefGroup noSynonymGroup;
    std::unordered_map<int, std::vector<std::pair<int, int>>> adjacencyList;
    std::vector<RelationshipInfo> rankings;

    rankings.reserve(relationships.size());
    for (int i = 0; i < relationships.size(); i++) {
        RelationshipRef r1 = relationships[i];
        RelationshipInfo temp = RelationshipInfo(0, 0,
                                                 countNumberOfFixedEntities(r1),
                                                 easeOfRelationshipMap[r1.rType], i);
        rankings.push_back(temp);
    }

    for (int index1 = 0; index1 < relationships.size(); index1++) {
        RelationshipRef r1 = relationships[index1];
        if (rankings[index1].numberOfFixedEntities == 2) {
            noSynonymGroup.addRelRef(r1);
            rankings[index1].visited = 1;
        } else {
            findConnectedRelationshipsAndAdd(index1, &adjacencyList, relationships);
        }
    }

    if (!noSynonymGroup.relRefGroup.empty()) {
        groups.push_back(noSynonymGroup);
    }

    while (notVisitedYet(&rankings) != -1) {
        RelationshipRefGroup tempGroup = traverseGraph(relationships, &rankings, &adjacencyList);
        groups.push_back(tempGroup);
    }

    return groups;
}

bool PreOptimiser::isFixedEntity(const Entity& e) {
    return e.eType == EntityType::FIXED_STRING || e.eType == EntityType::FIXED_INTEGER
            || e.eType == EntityType::FIXED_STRING_WITHIN_WILDCARD;
}
bool PreOptimiser::isFixedEntityOrWildcard(const Entity& e) {
    return isFixedEntity(e) || e.eType == EntityType::WILDCARD;
}
bool PreOptimiser::entityMatchRelRef(const Entity& e, const RelationshipRef& r) {
    return e == r.leftEntity || e == r.rightEntity || e == r.AssignmentEntity;
}
int PreOptimiser::countNumberOfFixedEntities(const RelationshipRef& r) {
    int count = 0;
    if (isFixedEntity(r.leftEntity)) {
        count = count + 1;
    }
    if (isFixedEntity(r.rightEntity)) {
        count = count + 1;
    }
    if (r.rType == RelationshipType::PATTERN) {
        count = count - 1;
    }
    return count;
}

void PreOptimiser::findConnectedRelationshipsAndAdd(int index1,
                                                    std::unordered_map<int,
                                                                       std::vector<std::pair<int, int>>>* adjacencyList,
                                                    const std::vector<RelationshipRef>& relationships) {
    (*adjacencyList)[index1] = {};
    RelationshipRef r1 = relationships[index1];
    RelationshipRef r2;

    for (int index2 = 0; index2 < relationships.size(); index2++) {
        if (index1 != index2) {
            r2 = relationships[index2];
            int numberOfEntitiesRelated = checkRelationshipsConnected(r1, r2);
            addRelationshipIfConnected(index1, index2, numberOfEntitiesRelated, adjacencyList);
        }
    }
}
void PreOptimiser::addRelationshipIfConnected(int index1, int index2, int numberOfEntitiesRelated,
                                              std::unordered_map<int,
                                                                 std::vector<std::pair<int, int>>>* adjacencyList) {
    if (numberOfEntitiesRelated >= 1) {
        std::pair<int, int> tempPair = {numberOfEntitiesRelated, index2};
        (*adjacencyList)[index1].push_back(tempPair);
    }
}
int PreOptimiser::checkRelationshipsConnected(const RelationshipRef& r1, const RelationshipRef& r2) {
    int numEntityConnected = 0;
    if (!isFixedEntityOrWildcard(r1.leftEntity) && entityMatchRelRef(r1.leftEntity, r2)) {
        numEntityConnected = numEntityConnected + 1;
    }
    if (!isFixedEntityOrWildcard(r1.rightEntity) && entityMatchRelRef(r1.rightEntity, r2)) {
        numEntityConnected = numEntityConnected + 1;
    }
    if (r1.AssignmentEntity.eType != EntityType::NULL_ENTITY
            && !isFixedEntityOrWildcard(r1.AssignmentEntity)
            && entityMatchRelRef(r1.AssignmentEntity, r2)) {
        numEntityConnected = numEntityConnected + 1;
    }
    return numEntityConnected;
}

int PreOptimiser::notVisitedYet(std::vector<RelationshipInfo>* rankings) {
    if ((*rankings).empty()) {
        return -1;
    }
    std::sort((*rankings).begin(), (*rankings).end(), rankingPredicate);
    RelationshipInfo chosenRelationshipRanking = (*rankings)[0];
    if (chosenRelationshipRanking.visited == 1) {
        return -1;
    } else {
        return chosenRelationshipRanking.relationshipNumber;
    }
}

int PreOptimiser::findRelationshipInfo(std::vector<RelationshipInfo>* rankings, int relationshipNumber) {
    for (int i = 0; i < (*rankings).size(); i++) {
        if ((*rankings)[i].relationshipNumber == relationshipNumber) {
            return i;
        }
    }
    return -1;
}

RelationshipRefGroup PreOptimiser::traverseGraph(const std::vector<RelationshipRef>& relationships,
                                                 std::vector<RelationshipInfo>* rankings,
                                                 std::unordered_map<int,
                                                                    std::vector<std::pair<int, int>>>* adjacencyList) {
    RelationshipRefGroup tempGroup;
    std::priority_queue<std::vector<int>, std::vector<std::vector<int>>,
                        bool (*)(const std::vector<int>&, const std::vector<int>&)>
            priorityQueue(priorityQueueComparator);

    int notVisitedInfoIndex = findRelationshipInfo(rankings, notVisitedYet(rankings));

    priorityQueue.push({(*rankings)[notVisitedInfoIndex].numberOfCommonSynoymns,
                        (*rankings)[notVisitedInfoIndex].numberOfFixedEntities,
                        (*rankings)[notVisitedInfoIndex].relationshipEase,
                        (*rankings)[notVisitedInfoIndex].relationshipNumber});

    while (!priorityQueue.empty()) {
        std::vector<int> currentNode = priorityQueue.top();
        int currentRelationship = currentNode[relationshipNumberIndex];
        int currentRelationshipIndex = findRelationshipInfo(rankings, currentRelationship);
        priorityQueue.pop();

        if ((*rankings)[currentRelationshipIndex].visited == 0) {
            (*rankings)[currentRelationshipIndex].visited = 1;
            tempGroup.addRelRef(relationships[currentRelationship]);
            traverseNeighbours(currentRelationship, rankings, adjacencyList, &priorityQueue);
        }
    }
    return tempGroup;
}

void PreOptimiser::traverseNeighbours(int currentRelationship,
                                      std::vector<RelationshipInfo>* rankings,
                                      std::unordered_map<int, std::vector<std::pair<int, int>>>* adjacencyList,
                                      std::priority_queue<std::vector<int>,
                                                          std::vector<std::vector<int>>,
                                                          bool (*)(const std::vector<int>&,
                                                                   const std::vector<int>&)>* priorityQueue) {

    for (auto neighbour : (*adjacencyList)[currentRelationship]) {
        int numberOfEntitiesRelated = neighbour.first;
        int neighbourRelationship = neighbour.second;
        int neighbourIndex = findRelationshipInfo(rankings, neighbourRelationship);

        if ((*rankings)[neighbourIndex].visited == 0) {

            (*rankings)[neighbourIndex].numberOfCommonSynoymns =
                    (*rankings)[neighbourIndex].numberOfCommonSynoymns + numberOfEntitiesRelated;

            (*priorityQueue).push({(*rankings)[neighbourIndex].numberOfCommonSynoymns,
                                   (*rankings)[neighbourIndex].numberOfFixedEntities,
                                   (*rankings)[neighbourIndex].relationshipEase,
                                   (*rankings)[neighbourIndex].relationshipNumber});
        }
    }
}
