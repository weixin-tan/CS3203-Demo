#include "ParentExtractor.h"

ParentExtractor::ParentExtractor(DB* db) : db(db) {}

void ParentExtractor::computeReverse() {
    for (const auto&[leftSide, rightSides] : db->parentTable)
        for (const auto& rightSide : rightSides)
            db->parentTableR[rightSide].insert(leftSide);
    for (const auto& r : db->stmtNos)
        if (db->parentTableR.find(r) == db->parentTableR.end()) db->parentTableR.insert({r, {}});
}

void ParentExtractor::precomputeRelationship() {
    std::map<int, std::set<int>>& parentTable = db->parentTable;
    for (const auto&[stmtNo, parsedStatement] : db->stmtTable) {
        int parentStmtNo = ((parsedStatement.ifStmtNo != ParsedStatement::DEFAULT_NULL_STMT_NO)
                            ? parsedStatement.ifStmtNo
                            : parsedStatement.whileStmtNo);
        if (parentStmtNo != ParsedStatement::DEFAULT_NULL_STMT_NO)
            parentTable[parentStmtNo].insert(parsedStatement.stmtNo);
    }
    for (const auto&[stmtNo, _] : db->stmtTable)
        if (parentTable.find(stmtNo) == parentTable.end()) parentTable.insert({stmtNo, {}});
    computeReverse();
}

