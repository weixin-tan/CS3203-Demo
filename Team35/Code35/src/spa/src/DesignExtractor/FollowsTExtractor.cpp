#include "FollowsTExtractor.h"
#include <queue>

FollowsTExtractor::FollowsTExtractor(DB* db) : db(db) {}

void FollowsTExtractor::computeReverse() {
    for (const auto&[leftSide, rightSides] : db->followsTTable)
        for (const auto& rightSide : rightSides)
            db->followsTTableR[rightSide].insert(leftSide);
    for (const auto& r : db->stmtNos)
        if (db->followsTTableR.find(r) == db->followsTTableR.end()) db->followsTTableR.insert({r, {}});
}

void visitFollowsNeighbour(const int& stmtNo, std::set<int>& visited, std::queue<int>& q) {
    if (visited.count(stmtNo) == 0) {
        visited.insert(stmtNo);
        q.push(stmtNo);
    }
}

void FollowsTExtractor::dfsFollowsT(const int& originStmt, std::set<int>& visited) {
    std::queue<int> q;
    q.push(originStmt);
    while (!q.empty()) {
        int stmtNo = q.front();
        q.pop();
        auto follows = db->followsTable.at(stmtNo);
        if (follows.empty()) continue;
        for (const auto& followsStmtNo : follows)
            visitFollowsNeighbour(followsStmtNo, visited, q);
    }
}

void FollowsTExtractor::precomputeRelationship() {
    std::map<int, std::set<int>>& followsTTable = db->followsTTable;
    for (const auto&[originStmt, _] : db->followsTable)
        dfsFollowsT(originStmt, followsTTable[originStmt]);
    for (const auto&[stmtNo, _] : db->stmtTable)
        if (followsTTable.find(stmtNo) == followsTTable.end()) followsTTable.insert({stmtNo, {}});
    computeReverse();
}
