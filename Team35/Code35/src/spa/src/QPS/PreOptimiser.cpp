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

bool PreOptimiser::isFixedEntity(const Entity& e){
    return e.eType == EntityType::FIXED_STRING || e.eType == EntityType::FIXED_INTEGER || e.eType == EntityType::FIXED_STRING_WITHIN_WILDCARD;
}

bool PreOptimiser::isFixedEntityOrWildcard(const Entity& e){
    return isFixedEntity(e) || e.eType == EntityType::WILDCARD;
}

bool PreOptimiser::entityMatchRelRef(const Entity& e, const RelationshipRef& r){
    return e == r.leftEntity || e == r.rightEntity || e == r.AssignmentEntity;
}

bool PreOptimiser::checkRelationshipsConnected(const RelationshipRef& r1, const RelationshipRef& r2){
    if (!isFixedEntityOrWildcard(r1.leftEntity) && entityMatchRelRef(r1.leftEntity, r2)){
        return true;
    }
    if  (!isFixedEntity(r1.rightEntity) && entityMatchRelRef(r1.rightEntity, r2)){
        return true;
    }
    if(r1.AssignmentEntity.eType != EntityType::NULL_ENTITY
        && !isFixedEntityOrWildcard(r1.AssignmentEntity)
        && entityMatchRelRef(r1.AssignmentEntity, r2)){
        return true;
    }
    return false;
}

int PreOptimiser::notVisitedYet(std::vector<int> visited){
    for (int i = 0; i < visited.size(); i++){
        if (visited[i] == 0){
            return i;
        }
    }
    return -1;
}

void PreOptimiser::addRelationshipToAdjacencyList(int index1, std::unordered_map<int, std::vector<int>>* adjacencyList, const std::vector<RelationshipRef>& relationships){
    (*adjacencyList)[index1] = {};
    RelationshipRef r1 = relationships[index1];
    RelationshipRef r2;

    for (int index2=0; index2<relationships.size(); index2++){
        if (index1 != index2){
            r2 = relationships[index2];
            if (checkRelationshipsConnected(r1, r2)){
                (*adjacencyList)[index1].push_back(index2);
            }
        }
    }
}

RelationshipRefGroup PreOptimiser::traverseGraph(std::vector<int> *visited, const std::vector<RelationshipRef>& relationships, std::unordered_map<int, std::vector<int>> *adjacencyList){
    RelationshipRefGroup tempGroup;
    std::queue<int> myQueue;
    myQueue.push(notVisitedYet(*(visited)));

    while (!myQueue.empty()){
        int currentNode = myQueue.front();
        myQueue.pop();

        if ((*visited)[currentNode] == 0){
            (*visited)[currentNode] = 1;
            tempGroup.addRelRef(relationships[currentNode]);
            for (int neighbour: (*adjacencyList)[currentNode]){
                if ((*visited)[neighbour] == 0){
                    myQueue.push(neighbour);
                }
            }
        }
    }
    return tempGroup;
}

void PreOptimiser::sortGroup(RelationshipRefGroup *tempGroup){
    //to-do
}

std::vector<RelationshipRefGroup> PreOptimiser::groupRelationships(const std::vector<RelationshipRef>& relationships) {
    std::vector<RelationshipRefGroup> groups;
    RelationshipRefGroup noSynonymGroup;
    std::unordered_map<int, std::vector<int>> adjacencyList;

    std::vector<int> visited;
    for(int i=0; i<relationships.size(); i++){
        visited.push_back(0);
    }

    RelationshipRef r1;

    for (int index1=0; index1<relationships.size(); index1++){
        r1 = relationships[index1];
        if (isFixedEntity(r1.rightEntity) && isFixedEntity(r1.leftEntity)){
            noSynonymGroup.addRelRef(r1);
            visited[index1] = 1;
        }else {
            addRelationshipToAdjacencyList(index1, &adjacencyList, relationships);
        }
    }

    if (!noSynonymGroup.relRefGroup.empty()){
        groups.push_back(noSynonymGroup);
    }

    while (notVisitedYet(visited) != -1){
        RelationshipRefGroup tempGroup = traverseGraph( &visited, relationships, &adjacencyList);
        sortGroup(&tempGroup);
        groups.push_back(tempGroup);
    }
    return groups;
}

std::string PreOptimiser::listToString(const std::vector<int>& ls){
    std::string s = "[";
    for (int i: ls){
        s = s + std::to_string(i) + " ";
    }
    return s + "]";
}

void PreOptimiser::printAdjacencyList(const std::unordered_map<int, std::vector<int>>& adjacencyList){
    for (auto const &pair: adjacencyList) {
        std::cout << "{" << pair.first << ": " << listToString(pair.second) << "}\n";
    }
}
