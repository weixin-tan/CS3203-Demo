#include "AffectsGetter.h"

#include <stack>

AffectsGetter::AffectsGetter(DB* db) : db(db) {}

void AffectsGetter::dfsAffects(
        int src,
        const std::map<int, std::set<int>>& nextGraph,
        std::map<int, std::set<int>>& affectsGraph,
        const std::string& var) {
    std::stack<int> stk;
    std::set<int> visited;
    stk.push(src);
    bool isStart = true;
    while (!stk.empty()) {
        int curStmtNo = stk.top();
        stk.pop();

        ElementType stmtType = db->elementStmtTable.at(curStmtNo).elementType;
        if (!isStart && ((stmtType != ElementType::WHILE) && (stmtType != ElementType::IF)) && db->modifiesSTable.at(curStmtNo).count(var) == 1) continue;  // modifies, don't dfs further
        isStart = false;
        auto nextStmtNos = nextGraph.find(curStmtNo);
        if (nextStmtNos == nextGraph.end()) continue;
        for (int nextStmtNo : nextStmtNos->second) {
            if (visited.count(nextStmtNo) == 0) {
                visited.insert(nextStmtNo);
                stk.push(nextStmtNo);
            }
        }
    }
    std::set<int> actualAffects;
    for (int stmtNo : visited)
        if (db->elementStmtTable.at(stmtNo).elementType == ElementType::ASSIGNMENT
            && db->usesSTable.at(stmtNo).count(var) == 1)
            actualAffects.insert(stmtNo);
    affectsGraph.insert({src, actualAffects});
}

void AffectsGetter::computeAndCacheAffects(int src) {
    if (db->computedAffectsSrc.count(src)) return;
    db->computedAffectsSrc.insert(src);
    std::string var = *db->modifiesSTable.at(src).begin();
    dfsAffects(src, db->nextTable, db->affectsTable, var);
}

void AffectsGetter::computeAndCacheAffectsR(int src) {
    if (db->computedAffectsRSrc.count(src)) return;
    db->computedAffectsRSrc.insert(src);
    // TODO: change to computing from NextT, maybe have reference to NextT getter for this?
    for (const auto& [stmtNo, _] : db->elementStmtTable)
        if (isRelationship(db->elementStmtTable.at(stmtNo), db->elementStmtTable.at(src)))
            db->affectsTableR[src].insert(stmtNo);
    if (db->affectsTableR.count(src) == 0) db->affectsTableR.insert({src, {}});
}

bool AffectsGetter::isRelationship(const ProgramElement& leftSide, const ProgramElement& rightSide) {
    if (!(isStatementType(leftSide.elementType) && isStatementType(rightSide.elementType)))
        throw std::invalid_argument("Wrong element type for isAffects");
    if (db->elementStmtTable.at(leftSide.stmtNo).elementType != ElementType::ASSIGNMENT
            || db->elementStmtTable.at(rightSide.stmtNo).elementType != ElementType::ASSIGNMENT) return false;
    if (db->usesSTable.at(rightSide.stmtNo).count(*db->modifiesSTable.at(leftSide.stmtNo).begin()) == 0)
        return false;
    computeAndCacheAffects(leftSide.stmtNo);
    auto affects = db->affectsTable.at(leftSide.stmtNo);
    return affects.count(rightSide.stmtNo) != 0;
}

// TODO: maybe optimise
std::set<ProgramElement> AffectsGetter::getLeftSide(const ProgramElement& rightSide, const ElementType& typeToGet) {
    std::set<ProgramElement> result;
    if(!(isStatementType(rightSide.elementType) && typeToGet == ElementType::ASSIGNMENT))
        throw std::invalid_argument("Wrong element type for getLeftSide on Affects");
    if (db->elementStmtTable.at(rightSide.stmtNo).elementType != ElementType::ASSIGNMENT)
        return {};
    computeAndCacheAffectsR(rightSide.stmtNo);
    for (const int& affectsStmtNo : db->affectsTableR.at(rightSide.stmtNo))
        RelationshipGetter::insertStmtElement(result, db->elementStmtTable.at(affectsStmtNo), typeToGet);
    return result;
}

std::set<ProgramElement> AffectsGetter::getRightSide(const ProgramElement& leftSide, const ElementType& typeToGet) {
    std::set<ProgramElement> result;
    if (!(isStatementType(leftSide.elementType) && typeToGet == ElementType::ASSIGNMENT))
        throw std::invalid_argument("Wrong element type for isAffects");
    if (db->elementStmtTable.at(leftSide.stmtNo).elementType != ElementType::ASSIGNMENT)
        return {};
    const std::string var = *db->modifiesSTable.at(leftSide.stmtNo).begin();
    computeAndCacheAffects(leftSide.stmtNo);
    for (const int& affectsStmtNo : db->affectsTable.at(leftSide.stmtNo))
        RelationshipGetter::insertStmtElement(result, db->elementStmtTable.at(affectsStmtNo), typeToGet);
    return result;
}
