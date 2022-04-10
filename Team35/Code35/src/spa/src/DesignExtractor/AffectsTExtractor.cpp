#include "AffectsTExtractor.h"

#include <set>
#include <stack>

AffectsTExtractor::AffectsTExtractor(DB* db) : db(db), affectsExtractor(db) {}

void AffectsTExtractor::dfsAffectsT(int src) {
    std::stack<int> stk;
    std::set<int> visited;
    stk.push(src);
    while (!stk.empty()) {
        int curStmtNo = stk.top();
        stk.pop();

        affectsExtractor.extractAffects(curStmtNo);
        for (int affectsStmtNo : db->affectsTable.at(curStmtNo)) {
            if (visited.count(affectsStmtNo) == 1) continue;
            visited.insert(affectsStmtNo);
            stk.push(affectsStmtNo);
        }
    }
    db->affectsTTable.insert({src, visited});
}

void AffectsTExtractor::dfsAffectsTR(int src) {
    std::stack<int> stk;
    std::set<int> visited;
    stk.push(src);
    while (!stk.empty()) {
        int curStmtNo = stk.top();
        stk.pop();

        affectsExtractor.extractAffectsR(curStmtNo);
        for (int affectsStmtNo : db->affectsTableR.at(curStmtNo)) {
            if (visited.count(affectsStmtNo) == 1) continue;
            visited.insert(affectsStmtNo);
            stk.push(affectsStmtNo);
        }
    }
    db->affectsTTableR.insert({src, visited});
}

void AffectsTExtractor::extractAffectsT(int src) {
    if (db->computedAffectsTSrc.count(src)) return;
    db->computedAffectsTSrc.insert(src);
    dfsAffectsT(src);
}

void AffectsTExtractor::extractAffectsTR(int src) {
    if (db->computedAffectsTRSrc.count(src)) return;
    db->computedAffectsTRSrc.insert(src);
    dfsAffectsTR(src);
}

