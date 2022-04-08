#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_NEXTEXTRACTOR_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_NEXTEXTRACTOR_H_

#include "PrecomputableRelationshipExtractor.h"
#include "../PKB/DB.h"

#include <queue>

class NextExtractor : public PrecomputableRelationshipExtractor {
private:
    DB* db;
    std::map<int, std::set<int>> stmtListHead;

private:
    void computeStmtListHead();
    void insertNextContainer(const int& stmtNo);
    void visitContainer(const int& stmtNo, const int& returnStmtNo, std::queue<std::pair<int, int>>& q);
    void dfsNext(const int& originStmt, std::set<int>& visited);
    void computeReverse();

public:
    explicit NextExtractor(DB* db);
    void precomputeRelationship() override;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_NEXTEXTRACTOR_H_
