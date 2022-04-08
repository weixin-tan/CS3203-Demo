#include "UsesSExtractor.h"

UsesSExtractor::UsesSExtractor(DB* db) : db(db) {}

void UsesSExtractor::computeReverse() {
    for (const auto& [leftSide, rightSides] : db->usesSTable)
        for (const auto& rightSide : rightSides)
            db->usesSTableR[rightSide].insert(leftSide);
    for (const auto& r : db->variables)
        if (db->usesSTableR.find(r) == db->usesSTableR.end()) db->usesSTableR.insert({r, {}});
}

void UsesSExtractor::precomputeRelationship() {
    std::map<int, std::set<std::string>>& usesSTable = db->usesSTable;
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
    computeReverse();
}
