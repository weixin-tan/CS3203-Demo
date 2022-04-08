#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_CALLSTEXTRACTOR_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_CALLSTEXTRACTOR_H_

#include "PrecomputableRelationshipExtractor.h"
#include "../PKB/DB.h"

class CallsTExtractor : public PrecomputableRelationshipExtractor {
private:
    DB* db;
    void dfsCallsT(const std::string& originProc, std::set<std::string>& visited);
    void computeReverse();

public:
    CallsTExtractor(DB* db);
    void precomputeRelationship() override;
};


#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_CALLSTEXTRACTOR_H_
