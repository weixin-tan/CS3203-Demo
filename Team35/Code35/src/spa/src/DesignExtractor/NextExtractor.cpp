#include "NextExtractor.h"
#include <queue>

NextExtractor::NextExtractor(DB* db) : db(db) {}

void NextExtractor::computeReverse() {
    for (const auto& [leftSide, rightSides] : db->nextTable)
        for (const auto& rightSide : rightSides)
            db->nextTableR[rightSide].insert(leftSide);
    for (const auto& r : db->stmtNos)
        if (db->nextTableR.find(r) == db->nextTableR.end()) db->nextTableR.insert({r, {}});
}

void NextExtractor::visitContainer(const int& stmtNo, const int& returnStmtNo, std::queue<std::pair<int, int>>& q) {
    for (const int& listHeadStmtNo : stmtListHead.at(stmtNo))
        q.push({listHeadStmtNo, returnStmtNo});
}

void NextExtractor::insertNextContainer(const int& stmtNo) {
    for (const int& listHeadStmtNo : stmtListHead.at(stmtNo))
        db->nextTable[stmtNo].insert(listHeadStmtNo);
}

void NextExtractor::dfsNext(const int& originStmt, std::set<int>& visited) {
    std::map<int, std::set<int>>& nextTable = db->nextTable;
    const int nullReturnStmt = -1;
    std::queue<std::pair<int, int>> q;
    q.push({originStmt, nullReturnStmt});
    while (!q.empty()) {
        const auto[stmtNo, returnStmtNo] = q.front();
        visited.insert(stmtNo);
        q.pop();
        // Next is childStmtList
        ElementType stmtType = db->elementStmtTable.at(stmtNo).elementType;
        if (stmtType == ElementType::WHILE || stmtType == ElementType::IF)
            insertNextContainer(stmtNo);
        if (stmtType == ElementType::WHILE)
            visitContainer(stmtNo, stmtNo, q);
        if (stmtType == ElementType::IF) {
            auto followsStmtNo = db->followsTable.at(stmtNo);
            int newReturnStmtNo = ((followsStmtNo.empty()) ? returnStmtNo : *followsStmtNo.begin());
            visitContainer(stmtNo, newReturnStmtNo, q);
        }

        auto followsStmtNo = db->followsTable.at(stmtNo);
        if (!followsStmtNo.empty())
            q.push({*followsStmtNo.begin(), returnStmtNo});
        if (followsStmtNo.empty() && returnStmtNo == nullReturnStmt)
            continue;
        if (stmtType != ElementType::IF)
            nextTable[stmtNo].insert((!followsStmtNo.empty()) ? *followsStmtNo.begin() : returnStmtNo);
    }
}

void NextExtractor::computeStmtListHead() {
    for (const auto& [stmtNo, parsedStatement]: db->stmtTable) {
        int parentStmtNo = ((parsedStatement.ifStmtNo != ParsedStatement::DEFAULT_NULL_STMT_NO)
                            ? parsedStatement.ifStmtNo
                            : parsedStatement.whileStmtNo);
        if (parentStmtNo != ParsedStatement::DEFAULT_NULL_STMT_NO
                && parsedStatement.preceding == ParsedStatement::DEFAULT_NULL_STMT_NO) {  // first in a statement list
            stmtListHead[parentStmtNo].insert(stmtNo);
        }
    }
    for (const auto& [stmtNo, parsedStatement]: db->stmtTable) {
        int parentStmtNo = ((parsedStatement.ifStmtNo != ParsedStatement::DEFAULT_NULL_STMT_NO)
                            ? parsedStatement.ifStmtNo
                            : parsedStatement.whileStmtNo);
        if (parentStmtNo != ParsedStatement::DEFAULT_NULL_STMT_NO
                && parsedStatement.preceding == ParsedStatement::DEFAULT_NULL_STMT_NO) {  // first in a statement list
            stmtListHead[parentStmtNo].insert(stmtNo);
        }
    }
}

void NextExtractor::precomputeRelationship() {
    std::map<int, std::set<int>>& nextTable = db->nextTable;
    computeStmtListHead();
    std::set<int> visited;
    for (const auto& [stmtNo, _]: db->stmtTable) {
        if (visited.find(stmtNo) == visited.end())
            dfsNext(stmtNo, visited);
    }
    for (const auto&[stmtNo, _] : db->stmtTable)
        if (nextTable.find(stmtNo) == nextTable.end()) nextTable.insert({stmtNo, {}});
    computeReverse();
}

