#include "NextTGetter.h"

#include <queue>

NextTGetter::NextTGetter(DB* db) : db(db) {}

void NextTGetter::dfsNextT(int src, std::set<int>& computedNextTSrc, const std::map<int, std::set<int>>& nextGraph, std::map<int, std::set<int>>& nextTGraph) {
    if (computedNextTSrc.count(src)) return;
    computedNextTSrc.insert(src);
    std::queue<int> q;
    std::set<int> visited;
    q.push(src);
    while (!q.empty()) {
        int curStmtNo = q.front();
        q.pop();
        auto nextStmtNos = nextGraph.at(curStmtNo);
        for (int nextStmtNo : nextStmtNos) {
            if (visited.count(nextStmtNo) == 0) {
                visited.insert(nextStmtNo);
                q.push(nextStmtNo);
            }
        }
    }
    nextTGraph.insert({src, visited});
}

void NextTGetter::computeAndCacheNextT(int src) {
    dfsNextT(src, db->computedNextTSrc, db->nextTable, db->nextTTable);
}

void NextTGetter::computeAndCacheNextTR(int src) {
    dfsNextT(src, db->computedNextTRSrc, db->nextTableR, db->nextTTableR);
}

bool NextTGetter::isRelationship(const ProgramElement& leftSide, const ProgramElement& rightSide) {
    if (!(isStatementType(leftSide.elementType) && isStatementType(rightSide.elementType)))
        throw std::invalid_argument("Wrong element type for isNextT");
    computeAndCacheNextT(leftSide.stmtNo);
    auto nextT = db->nextTTable.find(leftSide.stmtNo);
    return (nextT != db->nextTTable.end() && nextT->second.find(rightSide.stmtNo) != nextT->second.end());
}

std::set<ProgramElement> NextTGetter::getLeftSide(const ProgramElement& rightSide, const ElementType& typeToGet) {
    std::set<ProgramElement> result;
    if(!(isStatementType(rightSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for getLeftSide on NEXT_T");
    computeAndCacheNextTR(rightSide.stmtNo);
    auto previousT = db->nextTTableR.at(rightSide.stmtNo);
    for (const int& previousTStmtNo : previousT)
        RelationshipGetter::insertStmtElement(result, db->elementStmtTable.at(previousTStmtNo), typeToGet);
    return result;
}

std::set<ProgramElement> NextTGetter::getRightSide(const ProgramElement& leftSide, const ElementType& typeToGet) {
    std::set<ProgramElement> result;
    if(!(isStatementType(leftSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for getLeftSide on NEXT_T");
    computeAndCacheNextT(leftSide.stmtNo);
    auto nextT = db->nextTTable.at(leftSide.stmtNo);
    for (const int& nextTStmtNo : nextT)
        RelationshipGetter::insertStmtElement(result, db->elementStmtTable.at(nextTStmtNo), typeToGet);
    return result;
}
