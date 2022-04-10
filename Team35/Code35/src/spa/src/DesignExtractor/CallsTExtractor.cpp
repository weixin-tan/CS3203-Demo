#include "CallsTExtractor.h"
#include <queue>

CallsTExtractor::CallsTExtractor(DB* db) : db(db) {}

void CallsTExtractor::computeReverse() {
    for (const auto&[leftSide, rightSides] : db->callsTTable)
        for (const auto& rightSide : rightSides)
            db->callsTTableR[rightSide].insert(leftSide);
    for (const auto& r : db->procedures)
        if (db->callsTTableR.find(r) == db->callsTTableR.end()) db->callsTTableR.insert({r, {}});
}

void visitNeighbour(const std::string& calledProc, std::set<std::string>& visited, std::queue<std::string>& q) {
    if (visited.count(calledProc) == 0) {
        visited.insert(calledProc);
        q.push(calledProc);
    }
}

void CallsTExtractor::dfsCallsT(const std::string& originProc, std::set<std::string>& visited) {
    std::queue<std::string> q;
    q.push(originProc);
    while (!q.empty()) {
        std::string proc = q.front();
        q.pop();
        auto calledProcs = db->callsTable.at(proc);
        if (calledProcs.empty()) continue;
        for (const auto& calledProc : calledProcs)
            visitNeighbour(calledProc, visited, q);
    }
}

void CallsTExtractor::precomputeRelationship() {
    std::map<std::string, std::set<std::string>>& callsTTable = db->callsTTable;
    for (const auto&[originProc, _] : db->callsTable)
        dfsCallsT(originProc, callsTTable[originProc]);
    for (const auto& proc : db->procedures)
        if (callsTTable.find(proc) == callsTTable.end()) callsTTable.insert({proc, {}});
    computeReverse();
}
