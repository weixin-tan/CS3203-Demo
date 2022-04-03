#ifndef SPA_SRC_SPA_SRC_QPS_RELATIONSHIPREFGROUP_H_
#define SPA_SRC_SPA_SRC_QPS_RELATIONSHIPREFGROUP_H_

#include <vector>
#include "RelationshipRef.h"

class RelationshipRefGroup {
public:
    std::vector<RelationshipRef> relRefGroup;
    RelationshipRefGroup();

    void addRelRef(const RelationshipRef& r);
    void setRelRefGroup(std::vector<RelationshipRef> g);
};

#endif //SPA_SRC_SPA_SRC_QPS_RELATIONSHIPREFGROUP_H_
