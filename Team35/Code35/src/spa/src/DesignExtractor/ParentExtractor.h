#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_PARENTEXTRACTOR_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_PARENTEXTRACTOR_H_

#include "PrecomputableRelationshipExtractor.h"
#include "../PKB/DB.h"

class ParentExtractor : public PrecomputableRelationshipExtractor {
private:
    DB* db;
    void computeReverse();

public:
    explicit ParentExtractor(DB* db);
    void precomputeRelationship() override;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_PARENTEXTRACTOR_H_
