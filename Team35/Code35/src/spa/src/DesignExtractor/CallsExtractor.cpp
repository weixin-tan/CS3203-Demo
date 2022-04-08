#include "CallsExtractor.h"

CallsExtractor::CallsExtractor(DB* db) : db(db) {}

void CallsExtractor::computeReverse() {
    for (const auto& [leftSide, rightSides] : db->callsTable)
        for (const auto& rightSide : rightSides)
            db->callsTableR[rightSide].insert(leftSide);
    for (const auto& r : db->procedures)
        if (db->callsTableR.find(r) == db->callsTableR.end()) db->callsTableR.insert({r, {}});
}

void CallsExtractor::precomputeRelationship() {
    std::map<std::string, std::set<std::string>>& callsTable = db->callsTable;
    for (const auto&[stmtNo, parsedStatement] : db->stmtTable) {
        if (parsedStatement.statementType == StatementType::CALL_STMT)
            callsTable[parsedStatement.procedureName].insert(parsedStatement.procedureCalled);
    }
    for (const auto& proc : db->procedures)
        if (callsTable.find(proc) == callsTable.end()) callsTable.insert({proc, {}});
    computeReverse();
}