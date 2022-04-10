#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_FOLLOWSTEXTRACTOR_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_FOLLOWSTEXTRACTOR_H_

#include "PrecomputableRelationshipExtractor.h"
#include "../PKB/DB.h"

class FollowsTExtractor : public PrecomputableRelationshipExtractor {
private:
    DB* db;
    void dfsFollowsT(const int& originStmt, std::set<int>& visited);
    void computeReverse();

public:
    explicit FollowsTExtractor(DB* db);
    void precomputeRelationship() override;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_FOLLOWSTEXTRACTOR_H_
