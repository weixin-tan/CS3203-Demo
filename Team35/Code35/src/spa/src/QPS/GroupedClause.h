#ifndef SPA_SRC_SPA_SRC_QPS_GROUPEDCLAUSE_H_
#define SPA_SRC_SPA_SRC_QPS_GROUPEDCLAUSE_H_

#include <vector>
#include "Entity.h"
#include "RelationshipRefGroup.h"

class GroupedClause {
public:
    GroupedClause();

    std::vector<Entity> entityToFindList;
    std::vector<RelationshipRefGroup> relRefGroups;

    void setEntityToFindList(const std::vector<Entity>& e);
    void setRelationshipRefGroup(const std::vector<RelationshipRefGroup>& r);
};

#endif //SPA_SRC_SPA_SRC_QPS_GROUPEDCLAUSE_H_
