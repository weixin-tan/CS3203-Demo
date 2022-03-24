#include "PreOptimiser.h"

GroupedClause PreOptimiser::optimise(const std::vector<Clause>& clauses) {
    std::vector<RelationshipRefGroup> relRefGroups;
    RelationshipRefGroup group1;
    // just 1 group for now as it just skeleton code

    GroupedClause toreturn = GroupedClause();
    Clause c = clauses[0];
    toreturn.setEntityToFindList(c.entityToFindList);

    for (const auto& relRef: c.refList){
        group1.addRelRef(relRef);
    }

    relRefGroups.push_back(group1);
    toreturn.setRelationshipRefGroup(relRefGroups);
    return toreturn;
}
PreOptimiser::PreOptimiser() = default;
