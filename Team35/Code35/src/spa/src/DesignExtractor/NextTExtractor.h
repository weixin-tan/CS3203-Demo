#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_NEXTTEXTRACTOR_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_NEXTTEXTRACTOR_H_

#include "../PKB/DB.h"
#include <queue>

class NextTExtractor {
private:
    DB* db;

private:
    void dfsNextT(int src);
    void dfsNextTR(int src);
    static void visitNext(std::set<int>& visited, int nextStmtNo, std::queue<int>& q);

public:
    explicit NextTExtractor(DB* db);
    void extractNextT(int src);
    void extractNextTR(int src);
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_NEXTTEXTRACTOR_H_
