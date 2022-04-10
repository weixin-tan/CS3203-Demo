#include "FollowsExtractor.h"

FollowsExtractor::FollowsExtractor(DB* db) : db(db) {}

void FollowsExtractor::computeReverse() {
    for (const auto&[leftSide, rightSides] : db->followsTable)
        for (const auto& rightSide : rightSides)
            db->followsTableR[rightSide].insert(leftSide);
    for (const auto& r : db->stmtNos)
        if (db->followsTableR.find(r) == db->followsTableR.end()) db->followsTableR.insert({r, {}});
}

void FollowsExtractor::precomputeRelationship() {
    std::map<int, std::set<int>>& followsTable = db->followsTable;
    for (const auto&[stmtNo, parsedStatement] : db->stmtTable) {
        if (parsedStatement.preceding != ParsedStatement::DEFAULT_NULL_STMT_NO)
            followsTable[parsedStatement.preceding].insert(parsedStatement.stmtNo);
    }
    for (const auto&[stmtNo, _] : db->stmtTable)
        if (followsTable.find(stmtNo) == followsTable.end()) followsTable.insert({stmtNo, {}});
    computeReverse();
}
