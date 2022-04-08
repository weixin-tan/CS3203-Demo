#include "NextTExtractor.h"
#include <queue>

NextTExtractor::NextTExtractor(DB* db) : db(db) {}

void NextTExtractor::visitNext(std::set<int>& visited, int nextStmtNo, std::queue<int>& q) {
    if (visited.count(nextStmtNo) == 0) {
        visited.insert(nextStmtNo);
        q.push(nextStmtNo);
    }
}

void NextTExtractor::dfsNextT(int src) {
    if (db->computedNextTSrc.count(src)) return;
    db->computedNextTSrc.insert(src);
    std::queue<int> q;
    std::set<int> visited;
    q.push(src);
    while (!q.empty()) {
        int curStmtNo = q.front();
        q.pop();
        auto nextStmtNos = db->nextTable.at(curStmtNo);
        for (int nextStmtNo : nextStmtNos)
            visitNext(visited, nextStmtNo, q);
    }
    db->nextTTable.insert({src, visited});
}

void NextTExtractor::dfsNextTR(int src) {
    if (db->computedNextTRSrc.count(src)) return;
    db->computedNextTRSrc.insert(src);
    std::queue<int> q;
    std::set<int> visited;
    q.push(src);
    while (!q.empty()) {
        int curStmtNo = q.front();
        q.pop();
        auto nextRStmtNos = db->nextTableR.at(curStmtNo);
        for (int nextRStmtNo : nextRStmtNos)
            visitNext(visited, nextRStmtNo, q);
    }
    db->nextTTableR.insert({src, visited});
}

void NextTExtractor::extractNextT(int src) {
    dfsNextT(src);
}

void NextTExtractor::extractNextTR(int src) {
    dfsNextTR(src);
}

