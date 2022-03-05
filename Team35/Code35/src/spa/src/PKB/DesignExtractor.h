#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_DESIGNEXTRACTOR_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_DESIGNEXTRACTOR_H_

#include "DB.h"

class DesignExtractor {
private:
    DB* db;
    void dfsCallsT(const std::string& callingProc, std::set<std::string>& visited) const;

public:
    explicit DesignExtractor(DB* db);
    std::map<std::string, std::set<std::string>> extractCalls() const;
    std::map<std::string, std::set<std::string>> extractCallsT() const;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_DESIGNEXTRACTOR_H_
