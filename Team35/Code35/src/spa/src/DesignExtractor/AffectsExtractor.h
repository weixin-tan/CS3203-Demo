#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_AFFECTSEXTRACTOR_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_AFFECTSEXTRACTOR_H_

#include <set>
#include <string>
#include <stack>
#include "../PKB/DB.h"

class AffectsExtractor {
private:
    DB* db;

private:
    static void visitNext(std::set<int>& visited, int nextStmtNo, std::stack<int>& stk);
    void dfsAffects(int src, const std::string& var);
    void dfsAffectsR(int src, const std::string& var, std::set<int>& combinedAffectsR);

public:
    explicit AffectsExtractor(DB* db);
    void extractAffects(int src);
    void extractAffectsR(int src);
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_AFFECTSEXTRACTOR_H_
