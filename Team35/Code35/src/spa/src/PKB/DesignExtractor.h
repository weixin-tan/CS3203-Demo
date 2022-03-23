#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_DESIGNEXTRACTOR_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_DESIGNEXTRACTOR_H_

#include "DB.h"

class DesignExtractor {
private:
    DB* db;
    void dfsCallsT(const std::string& originProc, std::set<std::string>& visited);
    void dfsFollowsT(const int& originStmt, std::set<int>& visited);
    void dfsParentT(const int& originStmt, std::set<int>& visited);
    void dfsNext(const int& originStmt, std::set<int>& visited, const std::map<int, std::set<int>>& stmtListHead, std::map<int, std::set<int>>& nextTable);

public:
    explicit DesignExtractor(DB* db);
    void extractFollows(std::map<int, std::set<int>>& followsTable);
    void extractFollowsT(std::map<int, std::set<int>>& followsTTable);
    void extractParent(std::map<int, std::set<int>>& parentTable);
    void extractParentT(std::map<int, std::set<int>>& parentTTable);
    void extractCalls(std::map<std::string, std::set<std::string>>& callsTable);
    void extractCallsT(std::map<std::string, std::set<std::string>>& callsTTable);
    void extractModifiesP(std::map<std::string, std::set<std::string>>& modifiesPTable);
    void extractModifiesS(std::map<int, std::set<std::string>>& modifiesSTable);
    void extractUsesP(std::map<std::string, std::set<std::string>>& usesPTable);
    void extractUsesS(std::map<int, std::set<std::string>>& usesSTable);
    void extractNext(std::map<int, std::set<int>>& nextTable);

    template<typename U, typename V>
    static void computeReverse(std::map<U, std::set<V>>& normalMap, std::map<V, std::set<U>>& reverseMap, const std::set<V>& fullRange);
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_DESIGNEXTRACTOR_H_
