#include "CallsGetter.h"

CallsGetter::CallsGetter(DB* db) : db(db) {}

bool CallsGetter::isCalls(const ProgramElement& leftSide, const ProgramElement& rightSide) const {
    if (!(leftSide.elementType == ElementType::kProcedure && rightSide.elementType == ElementType::kProcedure))
        throw std::invalid_argument("Wrong element type for isCalls");
    auto calls = db->callsTable.find(leftSide.procName);
    return (calls != db->callsTable.end() && calls->second.find(rightSide.procName) != calls->second.end());
}

std::set<ProgramElement> CallsGetter::getLeftCalls(const ProgramElement& rightSide,
                                                       const ElementType& typeToGet) const {
    std::set<ProgramElement> result;
    if (!(typeToGet == ElementType::kProcedure && rightSide.elementType == ElementType::kProcedure))
        throw std::invalid_argument("Wrong element type for getLeftSide on Calls");
    auto caller = db->callsTableR.find(rightSide.procName);
    if (caller == db->callsTableR.end()) return {};
    for (const std::string& callerProc : caller->second)
        result.insert(ProgramElement::createProcedure(callerProc));
    return result;
}

std::set<ProgramElement> CallsGetter::getRightCalls(const ProgramElement& leftSide,
                                                        const ElementType& typeToGet) const {
    std::set<ProgramElement> result;
    if (!(leftSide.elementType == ElementType::kProcedure && typeToGet == ElementType::kProcedure))
        throw std::invalid_argument("Wrong element type for getRightSide on Calls");
    auto calls = db->callsTable.find(leftSide.procName);
    if (calls == db->callsTable.end()) return {};
    for (const std::string& calledProc : calls->second)
        result.insert(ProgramElement::createProcedure(calledProc));
    return result;
}
