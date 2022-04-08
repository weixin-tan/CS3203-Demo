#include "ParentTExtractor.h"
#include <queue>

ParentTExtractor::ParentTExtractor(DB* db) : db(db) {}

void ParentTExtractor::computeReverse() {
    for (const auto& [leftSide, rightSides] : db->parentTTable)
        for (const auto& rightSide : rightSides)
            db->parentTTableR[rightSide].insert(leftSide);
    for (const auto& r : db->stmtNos)
        if (db->parentTTableR.find(r) == db->parentTTableR.end()) db->parentTTableR.insert({r, {}});
}

void visitParentNeighbour(const int& stmtNo, std::set<int>& visited, std::queue<int>& q) {
    if (visited.count(stmtNo) == 0) {
        visited.insert(stmtNo);
        q.push(stmtNo);
    }
}

void ParentTExtractor::dfsParentT(const int& originStmt, std::set<int>& visited) {
    std::queue<int> q;
    q.push(originStmt);
    while (!q.empty()) {
        int stmtNo = q.front();
        q.pop();
        auto children = db->parentTable.at(stmtNo);
        if (children.empty()) continue;
        for (const auto& childStmtNo : children)
            visitParentNeighbour(childStmtNo, visited, q);
    }
}

void ParentTExtractor::precomputeRelationship() {
    std::map<int, std::set<int>>& parentTTable = db->parentTTable;
    for (const auto&[originStmt, _] : db->parentTable)
        dfsParentT(originStmt, parentTTable[originStmt]);
    for (const auto&[stmtNo, _] : db->stmtTable)
        if (parentTTable.find(stmtNo) == parentTTable.end()) parentTTable.insert({stmtNo, {}});
    computeReverse();
}
