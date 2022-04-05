#include "RelationshipRefGroup.h"

#include <utility>

RelationshipRefGroup::RelationshipRefGroup() = default;
void RelationshipRefGroup::addRelRef(const RelationshipRef& r) {
    relRefGroup.push_back(r);
}
void RelationshipRefGroup::setRelRefGroup(std::vector<RelationshipRef> g) {
    relRefGroup = std::move(g);
}
