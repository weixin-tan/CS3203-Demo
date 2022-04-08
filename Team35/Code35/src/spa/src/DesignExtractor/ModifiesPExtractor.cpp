#include "ModifiesPExtractor.h"

ModifiesPExtractor::ModifiesPExtractor(DB* db) : db(db) {}

void ModifiesPExtractor::computeReverse() {
    for (const auto& [leftSide, rightSides] : db->modifiesPTable)
        for (const auto& rightSide : rightSides)
            db->modifiesPTableR[rightSide].insert(leftSide);
    for (const auto& r : db->variables)
        if (db->modifiesPTableR.find(r) == db->modifiesPTableR.end()) db->modifiesPTableR.insert({r, {}});
}

void ModifiesPExtractor::precomputeRelationship() {
    std::map<std::string, std::set<std::string>>& modifiesPTable = db->modifiesPTable;
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
    computeReverse();
}

