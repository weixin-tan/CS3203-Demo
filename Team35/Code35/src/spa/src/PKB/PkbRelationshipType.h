#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_RELATIONSHIPTYPE_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_RELATIONSHIPTYPE_H_

// TODO: rename to just RelationshipType once deconflict with QPS RelationshipType
enum class PkbRelationshipType {
    MODIFIES,
    USES,
    PARENT,
    PARENT_T,
    FOLLOWS,
    FOLLOWS_T,
    CALLS,
    CALLS_T,
    NEXT,
    NEXT_T,
    AFFECTS,
    AFFECTS_T
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_RELATIONSHIPTYPE_H_
