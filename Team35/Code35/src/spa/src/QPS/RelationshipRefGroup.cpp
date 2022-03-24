#include "RelationshipRefGroup.h"

RelationshipRefGroup::RelationshipRefGroup() = default;
void RelationshipRefGroup::addRelRef(const RelationshipRef& r) {
    relRefGroup.push_back(r);
}
