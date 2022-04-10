#include "UsesPExtractor.h"

UsesPExtractor::UsesPExtractor(DB* db) : db(db) {}

void UsesPExtractor::computeReverse() {
    for (const auto&[leftSide, rightSides] : db->usesPTable)
        for (const auto& rightSide : rightSides)
            db->usesPTableR[rightSide].insert(leftSide);
    for (const auto& r : db->variables)
        if (db->usesPTableR.find(r) == db->usesPTableR.end()) db->usesPTableR.insert({r, {}});
}

void UsesPExtractor::precomputeRelationship() {
    std::map<std::string, std::set<std::string>>& usesPTable = db->usesPTable;
    for (const auto&[_, parsedStatement] : db->stmtTable)
        usesPTable[parsedStatement.procedureName].insert(parsedStatement.varUsed.begin(),
                                                         parsedStatement.varUsed.end());

    for (const auto&[callingProc, calledTProcs] : db->callsTTable) {
        for (const auto& calledTProc : calledTProcs) {
            auto usedVars = usesPTable.find(calledTProc);
            if (usedVars == usesPTable.end()) continue;
            usesPTable[callingProc].insert(usedVars->second.begin(), usedVars->second.end());
        }
    }
    for (const auto& proc : db->procedures)
        if (usesPTable.find(proc) == usesPTable.end()) usesPTable.insert({proc, {}});
    computeReverse();
}
