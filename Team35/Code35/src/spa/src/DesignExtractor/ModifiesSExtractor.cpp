#include "ModifiesSExtractor.h"

ModifiesSExtractor::ModifiesSExtractor(DB* db) : db(db) {}

void ModifiesSExtractor::computeReverse() {
    for (const auto& [leftSide, rightSides] : db->modifiesSTable)
        for (const auto& rightSide : rightSides)
            db->modifiesSTableR[rightSide].insert(leftSide);
    for (const auto& r : db->variables)
        if (db->modifiesSTableR.find(r) == db->modifiesSTableR.end()) db->modifiesSTableR.insert({r, {}});
}

void ModifiesSExtractor::precomputeRelationship() {
    std::map<int, std::set<std::string>>& modifiesSTable = db->modifiesSTable;
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
    computeReverse();
}
