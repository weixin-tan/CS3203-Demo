#include "DesignExtractor.h"

DesignExtractor::DesignExtractor(DB* db) : db(db) {}

std::map<std::string, std::set<std::string>> DesignExtractor::extractCalls() const {
    std::map<std::string, std::set<std::string>> callsTable;
    for (const auto&[stmtNo, procedureCalled] : db->stmtToProcedureCalled) {
        const auto& parsedStatement = db->stmtTable.at(stmtNo);
        callsTable[parsedStatement.procedure_name].insert(parsedStatement.procedure_called);
    }
    return callsTable;
}

void DesignExtractor::dfsCallsT(const std::string& callingProc, std::set<std::string>& visited) const {
    for (const auto& calledProc : db->callsTable[callingProc]) {
        if (visited.count(calledProc) == 0) {  // not visited
            visited.insert(calledProc);
            dfsCallsT(calledProc, visited);
        }
    }
}

std::map<std::string, std::set<std::string>> DesignExtractor::extractCallsT() const {
    std::map<std::string, std::set<std::string>> callsTTable;
    for (const auto&[callingProc, _] : db->callsTable) {
        std::set<std::string> visited;
        dfsCallsT(callingProc, visited);
        callsTTable[callingProc] = visited;
    }
    return callsTTable;
}
