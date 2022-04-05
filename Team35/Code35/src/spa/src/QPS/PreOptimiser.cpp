#include "PreOptimiser.h"

PreOptimiser::PreOptimiser() = default;

GroupedClause PreOptimiser::optimise(const std::vector<Clause>& clauses) {
    // just 1 group for now as it just skeleton code
    GroupedClause toReturn = GroupedClause();
    Clause c = clauses[0];
    toReturn.setEntityToFindList(c.entityToFindList);
    toReturn.setRelationshipRefGroup(groupRelationships(c.refList));
    return toReturn;
}

std::vector<RelationshipRefGroup> PreOptimiser::groupRelationships(const std::vector<RelationshipRef>& relationships) {
    std::vector<RelationshipRefGroup> groups;
    RelationshipRefGroup noSynonymGroup;
    std::unordered_map<int, std::deque<int>> adjacencyList;

    std::vector<int> visited;
    visited.reserve(relationships.size());
    for (int i = 0; i < relationships.size(); i++) {
        visited.push_back(0);
    }

    RelationshipRef r1;
    for (int index1 = 0; index1 < relationships.size(); index1++) {
        r1 = relationships[index1];
        if (isFixedEntity(r1.rightEntity) && isFixedEntity(r1.leftEntity)) {
            noSynonymGroup.addRelRef(r1);
            visited[index1] = 1;
        } else {
            findConnectedRelationshipsAndAdd(index1, &adjacencyList, relationships);
        }
    }

    if (!noSynonymGroup.relRefGroup.empty()) {
        groups.push_back(noSynonymGroup);
    }

    while (notVisitedYet(visited, relationships) != -1) {
        RelationshipRefGroup tempGroup = traverseGraph(&visited, relationships, &adjacencyList);
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

void PreOptimiser::findConnectedRelationshipsAndAdd(int index1,
                                                    std::unordered_map<int, std::deque<int>>* adjacencyList,
                                                    const std::vector<RelationshipRef>& relationships) {
    (*adjacencyList)[index1] = {};
    RelationshipRef r1 = relationships[index1];
    RelationshipRef r2;

    for (int index2 = 0; index2 < relationships.size(); index2++) {
        addRelationshipIfConnected(index1, index2, relationships, adjacencyList);
    }
}

void PreOptimiser::addRelationshipIfConnected(int index1, int index2,
                                              const std::vector<RelationshipRef>& relationships,
                                              std::unordered_map<int, std::deque<int>>* adjacencyList) {
    if (index1 != index2) {
        RelationshipRef r1 = relationships[index1];
        RelationshipRef r2 = relationships[index2];

        int numberOfEntitiesRelated = checkRelationshipsConnected(r1, r2);

        if (numberOfEntitiesRelated >= 2) {
            (*adjacencyList)[index1].push_front(index2);
        } else if (numberOfEntitiesRelated == 1) {
            (*adjacencyList)[index1].push_back(index2);
        }
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

    if (numEntityConnected > 0 &&
            (isFixedEntity(r2.leftEntity) || isFixedEntity(r2.rightEntity))
            && r2.rType != RelationshipType::PATTERN) {
        return numEntityConnected + 1;
    } else {
        return numEntityConnected;
    }
}

int PreOptimiser::countNumberOfNonFixedEntity(const RelationshipRef& relationship) {
    int i = 0;
    if (!isFixedEntity(relationship.leftEntity)) {
        i = i + 1;
    }
    if (!isFixedEntity(relationship.rightEntity)) {
        i = i + 1;
    }
    if (relationship.AssignmentEntity.eType != EntityType::NULL_ENTITY
            && !isFixedEntity(relationship.AssignmentEntity)) {
        i = i + 1;
    }
    return i;
}

int PreOptimiser::notVisitedYet(std::vector<int> visited, const std::vector<RelationshipRef>& relationships) {
    int temp = -1;
    for (int i = 0; i < visited.size(); i++) {
        if (visited[i] == 0 && countNumberOfNonFixedEntity(relationships[i]) == 1) {
            return i;
        } else if (visited[i] == 0 && temp == -1) {
            temp = i;
        }
    }
    return temp;
}

void PreOptimiser::traverseNeighbours(std::unordered_map<int, std::deque<int>>* adjacencyList,
                                      int currentNode,
                                      std::vector<int>* visited,
                                      std::queue<int>* myQueue) {
    for (int neighbour : (*adjacencyList)[currentNode]) {
        if ((*visited)[neighbour] == 0) {
            (*myQueue).push(neighbour);
        }
    }
}

RelationshipRefGroup PreOptimiser::traverseGraph(std::vector<int>* visited,
                                                 const std::vector<RelationshipRef>& relationships,
                                                 std::unordered_map<int, std::deque<int>>* adjacencyList) {
    RelationshipRefGroup tempGroup;
    std::queue<int> myQueue;
    myQueue.push(notVisitedYet(*visited, relationships));

    while (!myQueue.empty()) {
        int currentNode = myQueue.front();
        myQueue.pop();
        if ((*visited)[currentNode] == 0) {
            (*visited)[currentNode] = 1;
            tempGroup.addRelRef(relationships[currentNode]);
            traverseNeighbours(adjacencyList, currentNode, visited, &myQueue);
        }
    }
    return tempGroup;
}

std::string PreOptimiser::listToString(const std::vector<int>& ls) {
    std::string s = "[";
    for (int i : ls) {
        s = s + std::to_string(i) + " ";
    }
    return s + "]";
}

std::string PreOptimiser::dequeToString(const std::deque<int>& ls) {
    std::string s = "[";
    for (int i : ls) {
        s.append(std::to_string(i)).append(" ");
    }
    return s + "]";
}

void PreOptimiser::printAdjacencyList(const std::unordered_map<int, std::deque<int>>& adjacencyList) {
    for (auto const& pair : adjacencyList) {
        std::cout << "{" << pair.first << ": " << dequeToString(pair.second) << "}\n";
    }
}
