#include "DesignExtractor.h"
#include <queue>
#include <stack>

DesignExtractor::DesignExtractor(DB* db) : db(db) {}

void DesignExtractor::dfsCallsT(const std::string& originProc, std::set<std::string>& visited) {
    std::queue<std::string> q;
    q.push(originProc);
    while (!q.empty()) {
        std::string proc = q.front();
        q.pop();
        auto calledProcs = db->callsTable.at(proc);
        if (calledProcs.empty()) continue;
        for (const auto& calledProc : calledProcs) {
            if (visited.count(calledProc) == 0) {
                visited.insert(calledProc);
                q.push(calledProc);
            }
        }
    }
}

void DesignExtractor::dfsFollowsT(const int& originStmt, std::set<int>& visited) {
    std::queue<int> q;
    q.push(originStmt);
    while (!q.empty()) {
        int stmtNo = q.front();
        q.pop();
        auto follows = db->followsTable.at(stmtNo);
        if (follows.empty()) continue;
        for (const auto& followsStmtNo : follows) {
            if (visited.count(followsStmtNo) == 0) {
                visited.insert(followsStmtNo);
                q.push(followsStmtNo);
            }
        }
    }
}

void DesignExtractor::dfsParentT(const int& originStmt, std::set<int>& visited) {
    std::queue<int> q;
    q.push(originStmt);
    while (!q.empty()) {
        int stmtNo = q.front();
        q.pop();
        auto children = db->parentTable.at(stmtNo);
        if (children.empty()) continue;
        for (const auto& childStmtNo : children) {
            if (visited.count(childStmtNo) == 0) {
                visited.insert(childStmtNo);
                q.push(childStmtNo);
            }
        }
    }
}

void DesignExtractor::dfsNext(const int& originStmt, std::set<int>& visited, const std::map<int, std::set<int>>& stmtListHead, std::map<int, std::set<int>>& nextTable) {
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
            for (const int& listHeadStmtNo : stmtListHead.at(stmtNo))
                nextTable[stmtNo].insert(listHeadStmtNo);
        if (stmtType == ElementType::WHILE)
            for (const int& listHeadStmtNo : stmtListHead.at(stmtNo))
                q.push({listHeadStmtNo, stmtNo});
        if (stmtType == ElementType::IF) {
            auto followsStmtNo = db->followsTable.at(stmtNo);
            int newReturnStmtNo = ((followsStmtNo.empty()) ? returnStmtNo : *followsStmtNo.begin());
            for (const int& listHeadStmtNo : stmtListHead.at(stmtNo))
                q.push({listHeadStmtNo, newReturnStmtNo});
        }

        auto followsStmtNo = db->followsTable.at(stmtNo);
        if (stmtType != ElementType::IF) {
            if (!followsStmtNo.empty()) {
                nextTable[stmtNo].insert(*followsStmtNo.begin());
            } else if (returnStmtNo != nullReturnStmt)
                nextTable[stmtNo].insert(returnStmtNo);
        }
        if (!followsStmtNo.empty())
            q.push({*followsStmtNo.begin(), returnStmtNo});
    }
}

void DesignExtractor::dfsNextT(int src) {
    if (db->computedNextTSrc.count(src)) return;
    db->computedNextTSrc.insert(src);
    std::queue<int> q;
    std::set<int> visited;
    q.push(src);
    while (!q.empty()) {
        int curStmtNo = q.front();
        q.pop();
        auto nextStmtNos = db->nextTable.at(curStmtNo);
        for (int nextStmtNo : nextStmtNos) {
            if (visited.count(nextStmtNo) == 0) {
                visited.insert(nextStmtNo);
                q.push(nextStmtNo);
            }
        }
    }
    db->nextTTable.insert({src, visited});
}

void DesignExtractor::dfsNextTR(int src) {
    if (db->computedNextTRSrc.count(src)) return;
    db->computedNextTRSrc.insert(src);
    std::queue<int> q;
    std::set<int> visited;
    q.push(src);
    while (!q.empty()) {
        int curStmtNo = q.front();
        q.pop();
        auto nextRStmtNos = db->nextTableR.at(curStmtNo);
        for (int nextRStmtNo : nextRStmtNos) {
            if (visited.count(nextRStmtNo) == 0) {
                visited.insert(nextRStmtNo);
                q.push(nextRStmtNo);
            }
        }
    }
    db->nextTTableR.insert({src, visited});
}

void DesignExtractor::dfsAffects(int src, const std::string& var) {
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
        for (int nextStmtNo : nextStmtNos->second) {
            if (visited.count(nextStmtNo) == 0) {
                visited.insert(nextStmtNo);
                stk.push(nextStmtNo);
            }
        }
    }
    std::set<int> actualAffects;
    for (int stmtNo : visited)
        if (db->elementStmtTable.at(stmtNo).elementType == ElementType::ASSIGNMENT
                && db->usesSTable.at(stmtNo).count(var) == 1)
            actualAffects.insert(stmtNo);
    db->affectsTable.insert({src, actualAffects});
}

void DesignExtractor::dfsAffectsR(int src, const std::string& var, std::set<int>& combinedAffectsR) {
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
        for (int nextStmtNo : nextStmtNos->second) {
            if (visited.count(nextStmtNo) == 0) {
                visited.insert(nextStmtNo);
                stk.push(nextStmtNo);
            }
        }
    }
    for (int stmtNo : visited)
        if (db->elementStmtTable.at(stmtNo).elementType == ElementType::ASSIGNMENT
                && db->modifiesSTable.at(stmtNo).count(var) == 1)
            combinedAffectsR.insert(stmtNo);
}

void DesignExtractor::dfsAffectsT(int src) {
    std::stack<int> stk;
    std::set<int> visited;
    stk.push(src);
    while (!stk.empty()) {
        int curStmtNo = stk.top();
        stk.pop();

        extractAffects(curStmtNo);
        for (int affectsStmtNo : db->affectsTable.at(curStmtNo)) {
            if (visited.count(affectsStmtNo) == 1) continue;
            visited.insert(affectsStmtNo);
            stk.push(affectsStmtNo);
        }
    }
    db->affectsTTable.insert({src, visited});
}

void DesignExtractor::dfsAffectsTR(int src) {
    std::stack<int> stk;
    std::set<int> visited;
    stk.push(src);
    while (!stk.empty()) {
        int curStmtNo = stk.top();
        stk.pop();

        extractAffectsR(curStmtNo);
        for (int affectsStmtNo : db->affectsTableR.at(curStmtNo)) {
            if (visited.count(affectsStmtNo) == 1) continue;
            visited.insert(affectsStmtNo);
            stk.push(affectsStmtNo);
        }
    }
    db->affectsTTableR.insert({src, visited});
}

void DesignExtractor::extractFollows(std::map<int, std::set<int>>& followsTable) {
    for (const auto&[stmtNo, parsedStatement] : db->stmtTable) {
        if (parsedStatement.preceding != ParsedStatement::DEFAULT_NULL_STMT_NO)
            followsTable[parsedStatement.preceding].insert(parsedStatement.stmtNo);
    }
    for (const auto&[stmtNo, _] : db->stmtTable)
        if (followsTable.find(stmtNo) == followsTable.end()) followsTable.insert({stmtNo, {}});
}

void DesignExtractor::extractFollowsT(std::map<int, std::set<int>>& followsTTable) {
    for (const auto&[originStmt, _] : db->followsTable)
        dfsFollowsT(originStmt, followsTTable[originStmt]);
    for (const auto&[stmtNo, _] : db->stmtTable)
        if (followsTTable.find(stmtNo) == followsTTable.end()) followsTTable.insert({stmtNo, {}});
}

void DesignExtractor::extractParent(std::map<int, std::set<int>>& parentTable) {
    for (const auto&[stmtNo, parsedStatement] : db->stmtTable) {
        int parentStmtNo = ((parsedStatement.ifStmtNo != ParsedStatement::DEFAULT_NULL_STMT_NO)
                            ? parsedStatement.ifStmtNo
                            : parsedStatement.whileStmtNo);
        if (parentStmtNo != ParsedStatement::DEFAULT_NULL_STMT_NO)
            parentTable[parentStmtNo].insert(parsedStatement.stmtNo);
    }
    for (const auto&[stmtNo, _] : db->stmtTable)
        if (parentTable.find(stmtNo) == parentTable.end()) parentTable.insert({stmtNo, {}});
}

void DesignExtractor::extractParentT(std::map<int, std::set<int>>& parentTTable) {
    for (const auto&[originStmt, _] : db->parentTable)
        dfsParentT(originStmt, parentTTable[originStmt]);
    for (const auto&[stmtNo, _] : db->stmtTable)
        if (parentTTable.find(stmtNo) == parentTTable.end()) parentTTable.insert({stmtNo, {}});
}

void DesignExtractor::extractCalls(std::map<std::string, std::set<std::string>>& callsTable) {
    for (const auto&[stmtNo, parsedStatement] : db->stmtTable) {
        if (parsedStatement.statementType == StatementType::CALL_STMT)
            callsTable[parsedStatement.procedureName].insert(parsedStatement.procedureCalled);
    }
    for (const auto& proc : db->procedures)
        if (callsTable.find(proc) == callsTable.end()) callsTable.insert({proc, {}});
}

void DesignExtractor::extractCallsT(std::map<std::string, std::set<std::string>>& callsTTable) {
    for (const auto&[originProc, _] : db->callsTable)
        dfsCallsT(originProc, callsTTable[originProc]);
    for (const auto& proc : db->procedures)
        if (callsTTable.find(proc) == callsTTable.end()) callsTTable.insert({proc, {}});
}

void DesignExtractor::extractModifiesP(std::map<std::string, std::set<std::string>>& modifiesPTable) {
    // insert local
    for (const auto&[_, parsedStatement] : db->stmtTable)
        modifiesPTable[parsedStatement.procedureName].insert(parsedStatement.varModified.begin(), parsedStatement.varModified.end());

    for (const auto&[callingProc, calledTProcs] : db->callsTTable) {
        for (const auto& calledTProc : calledTProcs) {
            auto modifiedVars = modifiesPTable.find(calledTProc);
            if (modifiedVars == modifiesPTable.end()) continue;
            modifiesPTable[callingProc].insert(modifiedVars->second.begin(), modifiedVars->second.end());
        }
    }
    for (const auto& proc : db->procedures)
        if (modifiesPTable.find(proc) == modifiesPTable.end()) modifiesPTable.insert({proc, {}});
}

void DesignExtractor::extractModifiesS(std::map<int, std::set<std::string>>& modifiesSTable) {
    // insert local
    for (const auto&[_, parsedStatement] : db->stmtTable) {
        modifiesSTable[parsedStatement.stmtNo].insert(parsedStatement.varModified.begin(),
                                                      parsedStatement.varModified.end());
        if (parsedStatement.statementType == StatementType::CALL_STMT) {
            auto modifiedVars = db->modifiesPTable.find(parsedStatement.procedureCalled);
            if (modifiedVars == db->modifiesPTable.end()) continue;
            modifiesSTable[parsedStatement.stmtNo].insert(modifiedVars->second.begin(), modifiedVars->second.end());
        }
    }

    for (const auto&[parentStmt, parentTStmts] : db->parentTTable) {
        for (const auto& parentTStmt : parentTStmts) {
            auto modifiedVars = modifiesSTable.find(parentTStmt);
            if (modifiedVars == modifiesSTable.end()) continue;
            modifiesSTable[parentStmt].insert(modifiedVars->second.begin(), modifiedVars->second.end());
        }
    }
    for (const auto&[stmtNo, _] : db->stmtTable)
        if (modifiesSTable.find(stmtNo) == modifiesSTable.end()) modifiesSTable.insert({stmtNo, {}});
}

void DesignExtractor::extractUsesP(std::map<std::string, std::set<std::string>>& usesPTable) {
    // insert local
    for (const auto&[_, parsedStatement] : db->stmtTable)
        usesPTable[parsedStatement.procedureName].insert(parsedStatement.varUsed.begin(), parsedStatement.varUsed.end());

    for (const auto&[callingProc, calledTProcs] : db->callsTTable) {
        for (const auto& calledTProc : calledTProcs) {
            auto usedVars = usesPTable.find(calledTProc);
            if (usedVars == usesPTable.end()) continue;
            usesPTable[callingProc].insert(usedVars->second.begin(), usedVars->second.end());
        }
    }
    for (const auto& proc : db->procedures)
        if (usesPTable.find(proc) == usesPTable.end()) usesPTable.insert({proc, {}});
}

void DesignExtractor::extractUsesS(std::map<int, std::set<std::string>>& usesSTable) {
    // insert local
    for (const auto&[_, parsedStatement] : db->stmtTable) {
        usesSTable[parsedStatement.stmtNo].insert(parsedStatement.varUsed.begin(),
                                                  parsedStatement.varUsed.end());
        if (parsedStatement.statementType == StatementType::CALL_STMT) {
            auto usedVars = db->usesPTable.find(parsedStatement.procedureCalled);
            if (usedVars == db->usesPTable.end()) continue;
            usesSTable[parsedStatement.stmtNo].insert(usedVars->second.begin(), usedVars->second.end());
        }
    }

    for (const auto&[parentStmt, parentTStmts] : db->parentTTable) {
        for (const auto& parentTStmt : parentTStmts) {
            auto usedVars = usesSTable.find(parentTStmt);
            if (usedVars == usesSTable.end()) continue;
            usesSTable[parentStmt].insert(usedVars->second.begin(), usedVars->second.end());
        }
    }
    for (const auto&[stmtNo, _] : db->stmtTable)
        if (usesSTable.find(stmtNo) == usesSTable.end()) usesSTable.insert({stmtNo, {}});
}

void DesignExtractor::extractNext(std::map<int, std::set<int>>& nextTable) {
    std::map<int, std::set<int>> stmtListHead;
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
    std::set<int> visited;
    for (const auto& [stmtNo, _]: db->stmtTable) {
        if (visited.find(stmtNo) == visited.end())
            dfsNext(stmtNo, visited, stmtListHead, nextTable);
    }
    for (const auto&[stmtNo, _] : db->stmtTable)
        if (nextTable.find(stmtNo) == nextTable.end()) nextTable.insert({stmtNo, {}});
}

void DesignExtractor::extractNextT(int src) {
    dfsNextT(src);
}

void DesignExtractor::extractNextTR(int src) {
    dfsNextTR(src);
}

void DesignExtractor::extractAffects(int src) {
    if (db->computedAffectsSrc.count(src)) return;
    db->computedAffectsSrc.insert(src);
    std::string var = *db->modifiesSTable.at(src).begin();
    dfsAffects(src, var);
}

void DesignExtractor::extractAffectsR(int src) {
    if (db->computedAffectsRSrc.count(src)) return;
    db->computedAffectsRSrc.insert(src);
    db->affectsTableR.insert({src, {}});
    for (const std::string& var : db->usesSTable.at(src))
        dfsAffectsR(src, var, db->affectsTableR.at(src));
}

void DesignExtractor::extractAffectsT(int src) {
    if (db->computedAffectsTSrc.count(src)) return;
    db->computedAffectsTSrc.insert(src);
    dfsAffectsT(src);
}

void DesignExtractor::extractAffectsTR(int src) {
    if (db->computedAffectsTRSrc.count(src)) return;
    db->computedAffectsTRSrc.insert(src);
    dfsAffectsTR(src);
}

void DesignExtractor::precompute() {
    std::set<int> stmtNos;
    for (const auto& [stmtNo, _] : db->stmtTable) stmtNos.insert(stmtNo);

    extractCalls(db->callsTable);
    computeReverse(db->callsTable, db->callsTableR, db->procedures);
    extractCallsT(db->callsTTable);
    computeReverse(db->callsTTable, db->callsTTableR, db->procedures);
    extractFollows(db->followsTable);
    computeReverse(db->followsTable, db->followsTableR, stmtNos);
    extractFollowsT(db->followsTTable);
    computeReverse(db->followsTTable, db->followsTTableR, stmtNos);
    extractParent(db->parentTable);
    computeReverse(db->parentTable, db->parentTableR, stmtNos);
    extractParentT(db->parentTTable);
    computeReverse(db->parentTTable, db->parentTTableR, stmtNos);
    extractModifiesP(db->modifiesPTable);
    computeReverse(db->modifiesPTable, db->modifiesPTableR, db->variables);
    extractModifiesS(db->modifiesSTable);
    computeReverse(db->modifiesSTable, db->modifiesSTableR, db->variables);
    extractUsesP(db->usesPTable);
    computeReverse(db->usesPTable, db->usesPTableR, db->variables);
    extractUsesS(db->usesSTable);
    computeReverse(db->usesSTable, db->usesSTableR, db->variables);
    extractNext(db->nextTable);
    computeReverse(db->nextTable, db->nextTableR, stmtNos);
}

template<typename U, typename V>
void DesignExtractor::computeReverse(std::map<U, std::set<V>>& normalMap, std::map<V, std::set<U>>& reverseMap, const std::set<V>& fullRange) {
    for (const auto& [leftSide, rightSides] : normalMap)
        for (const auto& rightSide : rightSides)
            reverseMap[rightSide].insert(leftSide);
    for (const auto& r : fullRange)
        if (reverseMap.find(r) == reverseMap.end()) reverseMap.insert({r, {}});
}

template void DesignExtractor::computeReverse<int, int>(std::map<int, std::set<int>>& normalMap, std::map<int, std::set<int>>& reverseMap, const std::set<int>& fullRange);
template void DesignExtractor::computeReverse<int, std::string>(std::map<int, std::set<std::string>>& normalMap, std::map<std::string, std::set<int>>& reverseMap, const std::set<std::string>& fullRange);
template void DesignExtractor::computeReverse<std::string, std::string>(std::map<std::string, std::set<std::string>>& normalMap, std::map<std::string, std::set<std::string>>& reverseMap, const std::set<std::string>& fullRange);
template void DesignExtractor::computeReverse<std::string, int>(std::map<std::string, std::set<int>>& normalMap, std::map<int, std::set<std::string>>& reverseMap, const std::set<int>& fullRange);
