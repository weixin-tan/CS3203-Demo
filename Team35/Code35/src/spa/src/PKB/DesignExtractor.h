#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_DESIGNEXTRACTOR_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_DESIGNEXTRACTOR_H_

#include "DB.h"

class DesignExtractor {
private:
    DB* db;
    void dfsCallsT(const std::string& originProc, std::set<std::string>& visited);
    void dfsFollowsT(const int& originStmt, std::set<int>& visited);
    void dfsParentT(const int& originStmt, std::set<int>& visited);

public:
    explicit DesignExtractor(DB* db);
    void extractFollows(std::map<int, std::set<int>>& followsTable);
    void extractFollowsT(std::map<int, std::set<int>>& followsTTable);
    void extractParent(std::map<int, std::set<int>>& parentTable);
    void extractParentT(std::map<int, std::set<int>>& parentTTable);
    void extractCalls(std::map<std::string, std::set<std::string>>& callsTable);
    void extractCallsT(std::map<std::string, std::set<std::string>>& callsTTable);
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_DESIGNEXTRACTOR_H_
