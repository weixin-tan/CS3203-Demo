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
