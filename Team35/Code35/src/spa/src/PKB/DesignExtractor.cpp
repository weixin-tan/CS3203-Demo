    #include "DesignExtractor.h"
#include <queue>

DesignExtractor::DesignExtractor(DB* db) : db(db) {}

void DesignExtractor::dfsCallsT(const std::string& originProc, std::set<std::string>& visited) {
    std::queue<std::string> q;
    q.push(originProc);
    while (!q.empty()) {
        std::string proc = q.front();
        q.pop();
        auto calls = db->callsTable.find(proc);
        if (calls == db->callsTable.end()) continue;
        for (const auto& calledProc : calls->second) {
            if (visited.find(calledProc) == visited.end()) {
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
        auto follows = db->followsTable.find(stmtNo);
        if (follows == db->followsTable.end()) continue;
        for (const auto& followsStmtNo : follows->second) {
            if (visited.find(followsStmtNo) == visited.end()) {
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
        auto children = db->parentTable.find(stmtNo);
        if (children == db->parentTable.end()) continue;
        for (const auto& childStmtNo : children->second) {
            if (visited.find(childStmtNo) == visited.end()) {
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
        if (stmtType == ElementType::kWhile || stmtType == ElementType::kIf)
            for (const int& listHeadStmtNo : stmtListHead.at(stmtNo))
                nextTable[stmtNo].insert(listHeadStmtNo);
        if (stmtType == ElementType::kWhile)
            for (const int& listHeadStmtNo : stmtListHead.at(stmtNo))
                q.push({listHeadStmtNo, stmtNo});
        if (stmtType == ElementType::kIf) {
            auto followsStmtNo = db->followsTable.find(stmtNo);
            int newReturnStmtNo = ((followsStmtNo != db->followsTable.end()) ? *followsStmtNo->second.begin() : returnStmtNo);
            for (const int& listHeadStmtNo : stmtListHead.at(stmtNo))
                q.push({listHeadStmtNo, newReturnStmtNo});
        }

        auto followsStmtNo = db->followsTable.find(stmtNo);
        if (stmtType != ElementType::kIf) {
            if (followsStmtNo != db->followsTable.end()) {
                nextTable[stmtNo].insert(*followsStmtNo->second.begin());
            } else if (returnStmtNo != nullReturnStmt)
                nextTable[stmtNo].insert(returnStmtNo);
        }
        if (followsStmtNo != db->followsTable.end())
            q.push({*followsStmtNo->second.begin(), returnStmtNo});
    }

}

void DesignExtractor::extractFollows(std::map<int, std::set<int>>& followsTable) {
    for (const auto&[stmtNo, parsedStatement] : db->stmtTable) {
        if (parsedStatement.preceding != ParsedStatement::default_null_stmt_no)
            followsTable[parsedStatement.preceding].insert(parsedStatement.stmt_no);
    }
}

void DesignExtractor::extractFollowsT(std::map<int, std::set<int>>& followsTTable) {
    for (const auto&[originStmt, _] : db->followsTable)
        dfsFollowsT(originStmt, followsTTable[originStmt]);
}

void DesignExtractor::extractParent(std::map<int, std::set<int>>& parentTable) {
    for (const auto&[stmtNo, parsedStatement] : db->stmtTable) {
        int parentStmtNo = ((parsedStatement.if_stmt_no != ParsedStatement::default_null_stmt_no)
                ? parsedStatement.if_stmt_no
                : parsedStatement.while_stmt_no);
        if (parentStmtNo != ParsedStatement::default_null_stmt_no)
            parentTable[parentStmtNo].insert(parsedStatement.stmt_no);
    }
}

void DesignExtractor::extractParentT(std::map<int, std::set<int>>& parentTTable) {
    for (const auto&[originStmt, _] : db->parentTable)
        dfsParentT(originStmt, parentTTable[originStmt]);
}

void DesignExtractor::extractCalls(std::map<std::string, std::set<std::string>>& callsTable) {
    for (const auto&[stmtNo, parsedStatement] : db->stmtTable) {
        if (parsedStatement.statement_type == StatementType::kcall_stmt)
            callsTable[parsedStatement.procedure_name].insert(parsedStatement.procedure_called);
    }
}

void DesignExtractor::extractCallsT(std::map<std::string, std::set<std::string>>& callsTTable) {
    for (const auto&[originProc, _] : db->callsTable)
        dfsCallsT(originProc, callsTTable[originProc]);
}

void DesignExtractor::extractModifiesP(std::map<std::string, std::set<std::string>>& modifiesPTable) {
    // insert local
    for (const auto&[_, parsedStatement] : db->stmtTable)
        modifiesPTable[parsedStatement.procedure_name].insert(parsedStatement.var_modified.begin(), parsedStatement.var_modified.end());

    for (const auto&[callingProc, calledTProcs] : db->callsTTable) {
        for (const auto& calledTProc : calledTProcs) {
            auto modifiedVars = modifiesPTable.find(calledTProc);
            if (modifiedVars == modifiesPTable.end()) continue;
            modifiesPTable[callingProc].insert(modifiedVars->second.begin(), modifiedVars->second.end());
        }
    }
}

void DesignExtractor::extractModifiesS(std::map<int, std::set<std::string>>& modifiesSTable) {
    // insert local
    for (const auto&[_, parsedStatement] : db->stmtTable) {
        modifiesSTable[parsedStatement.stmt_no].insert(parsedStatement.var_modified.begin(),
                                                       parsedStatement.var_modified.end());
        if (parsedStatement.statement_type == StatementType::kcall_stmt) {
            auto modifiedVars = db->modifiesPTable.find(parsedStatement.procedure_called);
            if (modifiedVars == db->modifiesPTable.end()) continue;
            modifiesSTable[parsedStatement.stmt_no].insert(modifiedVars->second.begin(), modifiedVars->second.end());
        }
    }

    for (const auto&[parentStmt, parentTStmts] : db->parentTTable) {
        for (const auto& parentTStmt : parentTStmts) {
            auto modifiedVars = modifiesSTable.find(parentTStmt);
            if (modifiedVars == modifiesSTable.end()) continue;
            modifiesSTable[parentStmt].insert(modifiedVars->second.begin(), modifiedVars->second.end());
        }
    }
}

void DesignExtractor::extractUsesP(std::map<std::string, std::set<std::string>>& usesPTable) {
    // insert local
    for (const auto&[_, parsedStatement] : db->stmtTable)
        usesPTable[parsedStatement.procedure_name].insert(parsedStatement.var_used.begin(), parsedStatement.var_used.end());

    for (const auto&[callingProc, calledTProcs] : db->callsTTable) {
        for (const auto& calledTProc : calledTProcs) {
            auto usedVars = usesPTable.find(calledTProc);
            if (usedVars == usesPTable.end()) continue;
            usesPTable[callingProc].insert(usedVars->second.begin(), usedVars->second.end());
        }
    }
}

void DesignExtractor::extractUsesS(std::map<int, std::set<std::string>>& usesSTable) {
    // insert local
    for (const auto&[_, parsedStatement] : db->stmtTable) {
        usesSTable[parsedStatement.stmt_no].insert(parsedStatement.var_used.begin(),
                                                       parsedStatement.var_used.end());
        if (parsedStatement.statement_type == StatementType::kcall_stmt) {
            auto usedVars = db->usesPTable.find(parsedStatement.procedure_called);
            if (usedVars == db->usesPTable.end()) continue;
            usesSTable[parsedStatement.stmt_no].insert(usedVars->second.begin(), usedVars->second.end());
        }
    }

    for (const auto&[parentStmt, parentTStmts] : db->parentTTable) {
        for (const auto& parentTStmt : parentTStmts) {
            auto usedVars = usesSTable.find(parentTStmt);
            if (usedVars == usesSTable.end()) continue;
            usesSTable[parentStmt].insert(usedVars->second.begin(), usedVars->second.end());
        }
    }
}

void DesignExtractor::extractNext(std::map<int, std::set<int>>& nextTable) {
    std::map<int, std::set<int>> stmtListHead;
    for (const auto& [stmtNo, parsedStatement]: db->stmtTable) {
        int parentStmtNo = ((parsedStatement.if_stmt_no != ParsedStatement::default_null_stmt_no)
                            ? parsedStatement.if_stmt_no
                            : parsedStatement.while_stmt_no);
        if (parentStmtNo != ParsedStatement::default_null_stmt_no
            && parsedStatement.preceding == ParsedStatement::default_null_stmt_no) {  // first in a statement list
            stmtListHead[parentStmtNo].insert(stmtNo);
        }
    }
    for (const auto& [stmtNo, parsedStatement]: db->stmtTable) {
        int parentStmtNo = ((parsedStatement.if_stmt_no != ParsedStatement::default_null_stmt_no)
                            ? parsedStatement.if_stmt_no
                            : parsedStatement.while_stmt_no);
        if (parentStmtNo != ParsedStatement::default_null_stmt_no
                && parsedStatement.preceding == ParsedStatement::default_null_stmt_no) {  // first in a statement list
            stmtListHead[parentStmtNo].insert(stmtNo);
        }
    }
    std::set<int> visited;
    for (const auto& [stmtNo, _]: db->stmtTable) {
        if (visited.find(stmtNo) == visited.end())
            dfsNext(stmtNo, visited, stmtListHead, nextTable);
    }
}

template<typename U, typename V>
void DesignExtractor::computeReverse(std::map<U, std::set<V>>& normalMap, std::map<V, std::set<U>>& reverseMap) {
    for (const auto& [leftSide, rightSides] : normalMap)
        for (const auto& rightSide : rightSides)
            reverseMap[rightSide].insert(leftSide);
}

template void DesignExtractor::computeReverse<int, int>(std::map<int, std::set<int>>& normalMap, std::map<int, std::set<int>>& reverseMap);
template void DesignExtractor::computeReverse<int, std::string>(std::map<int, std::set<std::string>>& normalMap, std::map<std::string, std::set<int>>& reverseMap);
template void DesignExtractor::computeReverse<std::string, std::string>(std::map<std::string, std::set<std::string>>& normalMap, std::map<std::string, std::set<std::string>>& reverseMap);
template void DesignExtractor::computeReverse<std::string, int>(std::map<std::string, std::set<int>>& normalMap, std::map<int, std::set<std::string>>& reverseMap);
