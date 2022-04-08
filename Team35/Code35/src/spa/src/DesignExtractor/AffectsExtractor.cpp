#include "AffectsExtractor.h"

AffectsExtractor::AffectsExtractor(DB* db) : db(db) {}

void AffectsExtractor::visitNext(std::set<int>& visited, int nextStmtNo, std::stack<int>& stk) {
    if (visited.count(nextStmtNo) == 0) {
        visited.insert(nextStmtNo);
        stk.push(nextStmtNo);
    }
}

void AffectsExtractor::dfsAffects(int src, const std::string& var) {
    std::stack<int> stk;
    std::set<int> visited;
    stk.push(src);
    bool isStart = true;
    while (!stk.empty()) {
        int curStmtNo = stk.top();
        stk.pop();

        ElementType stmtType = db->elementStmtTable.at(curStmtNo).elementType;
        if (!isStart && ((stmtType != ElementType::WHILE) && (stmtType != ElementType::IF))
                && db->modifiesSTable.at(curStmtNo).count(var) == 1) continue;  // modifies, don't dfs further
        isStart = false;
        auto nextStmtNos = db->nextTable.find(curStmtNo);
        if (nextStmtNos == db->nextTable.end()) continue;
        for (int nextStmtNo : nextStmtNos->second)
            visitNext(visited, nextStmtNo, stk);
    }
    std::set<int> actualAffects;
    for (int stmtNo : visited)
        if (db->elementStmtTable.at(stmtNo).elementType == ElementType::ASSIGNMENT
                && db->usesSTable.at(stmtNo).count(var) == 1)
            actualAffects.insert(stmtNo);
    db->affectsTable.insert({src, actualAffects});
}

void AffectsExtractor::dfsAffectsR(int src, const std::string& var, std::set<int>& combinedAffectsR) {
    std::stack<int> stk;
    std::set<int> visited;
    stk.push(src);
    bool isStart = true;
    while (!stk.empty()) {
        int curStmtNo = stk.top();
        stk.pop();

        ElementType stmtType = db->elementStmtTable.at(curStmtNo).elementType;
        if (!isStart && ((stmtType != ElementType::WHILE) && (stmtType != ElementType::IF))
                && db->modifiesSTable.at(curStmtNo).count(var) == 1) continue;  // modifies, don't dfs further
        isStart = false;
        auto nextStmtNos = db->nextTableR.find(curStmtNo);
        if (nextStmtNos == db->nextTableR.end()) continue;
        for (int nextStmtNo : nextStmtNos->second)
            visitNext(visited, nextStmtNo, stk);
    }
    for (int stmtNo : visited)
        if (db->elementStmtTable.at(stmtNo).elementType == ElementType::ASSIGNMENT
                && db->modifiesSTable.at(stmtNo).count(var) == 1)
            combinedAffectsR.insert(stmtNo);
}

void AffectsExtractor::extractAffects(int src) {
    if (db->computedAffectsSrc.count(src)) return;
    db->computedAffectsSrc.insert(src);
    std::string var = *db->modifiesSTable.at(src).begin();
    dfsAffects(src, var);
}

void AffectsExtractor::extractAffectsR(int src) {
    if (db->computedAffectsRSrc.count(src)) return;
    db->computedAffectsRSrc.insert(src);
    db->affectsTableR.insert({src, {}});
    for (const std::string& var : db->usesSTable.at(src))
        dfsAffectsR(src, var, db->affectsTableR.at(src));
}
