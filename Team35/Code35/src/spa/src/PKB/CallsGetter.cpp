#include "CallsGetter.h"

CallsGetter::CallsGetter(DB* db) : db(db) {}

bool CallsGetter::isRelationship(const ProgramElement& leftSide, const ProgramElement& rightSide) {
    if (!(leftSide.elementType == ElementType::PROCEDURE && rightSide.elementType == ElementType::PROCEDURE))
        throw std::invalid_argument("Wrong element type for isCalls");
    auto calls = db->callsTable.find(leftSide.procName);
    return (calls != db->callsTable.end() && calls->second.find(rightSide.procName) != calls->second.end());
}

std::set<ProgramElement*> CallsGetter::getLeftSide(const ProgramElement& rightSide, const ElementType& typeToGet) {
    std::set<ProgramElement*> result;
    if (!(typeToGet == ElementType::PROCEDURE && rightSide.elementType == ElementType::PROCEDURE))
        throw std::invalid_argument("Wrong element type for getLeftSide on Calls");
    auto caller = db->callsTableR.find(rightSide.procName);
    if (caller == db->callsTableR.end()) return {};
    for (const std::string& callerProc : caller->second)
        result.insert(&db->elementProcTable.at(callerProc));
    return result;
}

std::set<ProgramElement*> CallsGetter::getRightSide(const ProgramElement& leftSide, const ElementType& typeToGet) {
    std::set<ProgramElement*> result;
    if (!(leftSide.elementType == ElementType::PROCEDURE && typeToGet == ElementType::PROCEDURE))
        throw std::invalid_argument("Wrong element type for getRightSide on Calls");
    auto calls = db->callsTable.find(leftSide.procName);
    if (calls == db->callsTable.end()) return {};
    for (const std::string& calledProc : calls->second)
        result.insert(&db->elementProcTable.at(calledProc));
    return result;
}
