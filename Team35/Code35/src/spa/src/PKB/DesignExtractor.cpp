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
