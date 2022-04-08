#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_FOLLOWSEXTRACTOR_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_FOLLOWSEXTRACTOR_H_

#include "PrecomputableRelationshipExtractor.h"
#include "../PKB/DB.h"

class FollowsExtractor : public PrecomputableRelationshipExtractor {
private:
    DB* db;
    void computeReverse();

public:
    explicit FollowsExtractor(DB* db);
    void precomputeRelationship() override;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_FOLLOWSEXTRACTOR_H_
