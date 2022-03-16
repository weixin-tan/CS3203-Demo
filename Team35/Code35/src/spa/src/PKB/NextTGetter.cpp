#include "NextTGetter.h"

#include <queue>

NextTGetter::NextTGetter(DB* db) : db(db) {}

void NextTGetter::dfsNextT(int src, std::set<int>& computedNextTSrc, const std::map<int, std::set<int>>& nextGraph, std::map<int, std::set<int>>& nextTGraph) {
    if (computedNextTSrc.count(src)) return;
    computedNextTSrc.insert(src);
    std::queue<int> q;
    std::set<int> visited;
//    std::cerr << "Computing NextT for " << src << " for the first time!\n";
    q.push(src);
    while (!q.empty()) {
        int curStmtNo = q.front();
        q.pop();
        auto nextStmtNos = nextGraph.find(curStmtNo);
        if (nextStmtNos == nextGraph.end()) continue;
        for (int nextStmtNo : nextStmtNos->second) {
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

bool NextTGetter::isNextT(const ProgramElement& leftSide, const ProgramElement& rightSide) {
    if (!(isStatementType(leftSide.elementType) && isStatementType(rightSide.elementType)))
        throw std::invalid_argument("Wrong element type for isNextT");
    computeAndCacheNextT(leftSide.stmtNo);
    auto nextT = db->nextTTable.find(leftSide.stmtNo);
    return (nextT != db->nextTTable.end() && nextT->second.find(rightSide.stmtNo) != nextT->second.end());
}

std::set<ProgramElement> NextTGetter::getLeftNextT(const ProgramElement& rightSide, const ElementType& typeToGet) {
    std::set<ProgramElement> result;
    if(!(isStatementType(rightSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for getLeftSide on NextT");
    computeAndCacheNextTR(rightSide.stmtNo);
    auto previous = db->nextTTableR.find(rightSide.stmtNo);
    if (previous == db->nextTTableR.end()) return {};
    for (const int& previousStmtNo : previous->second)
        TemplateGetter::insertStmtElement(result, db->elementStmtTable.at(previousStmtNo), typeToGet);
    return result;
}

std::set<ProgramElement> NextTGetter::getRightNextT(const ProgramElement& leftSide, const ElementType& typeToGet){
    std::set<ProgramElement> result;
    if(!(isStatementType(leftSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for getLeftSide on NextT");
    computeAndCacheNextT(leftSide.stmtNo);
    auto previous = db->nextTTable.find(leftSide.stmtNo);
    if (previous == db->nextTTable.end()) return {};
    for (const int& previousStmtNo : previous->second)
        TemplateGetter::insertStmtElement(result, db->elementStmtTable.at(previousStmtNo), typeToGet);
    return result;
}
