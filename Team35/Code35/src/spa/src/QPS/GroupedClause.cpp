#include "GroupedClause.h"

GroupedClause::GroupedClause() = default;
void GroupedClause::setEntityToFindList(const std::vector<Entity>& e) {
    entityToFindList = e;
}
void GroupedClause::setRelationshipRefGroup(const std::vector<RelationshipRefGroup>& r) {
    relRefGroups = r;
}

