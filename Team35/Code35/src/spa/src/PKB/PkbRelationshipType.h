#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_RELATIONSHIPTYPE_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_RELATIONSHIPTYPE_H_

// TODO: rename to just RelationshipType once deconflict with QPS RelationshipType
enum class PkbRelationshipType {
    kModifies,
    kUses,
    kParent,
    kParentT,
    kFollows,
    kFollowsT,
    kCalls,
    kCallsT,
    kNext,
    kNextT,
    kAffects,
    kAffectsT
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_RELATIONSHIPTYPE_H_
