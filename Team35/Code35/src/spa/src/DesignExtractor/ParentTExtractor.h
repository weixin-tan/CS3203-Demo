#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_PARENTTEXTRACTOR_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_PARENTTEXTRACTOR_H_

#include "PrecomputableRelationshipExtractor.h"
#include "../PKB/DB.h"

class ParentTExtractor : public PrecomputableRelationshipExtractor {
private:
    DB* db;
    void dfsParentT(const int& originStmt, std::set<int>& visited);
    void computeReverse();

public:
    explicit ParentTExtractor(DB* db);
    void precomputeRelationship() override;
};


#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_PARENTTEXTRACTOR_H_
