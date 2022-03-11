#include "CallsTGetter.h"

CallsTGetter::CallsTGetter(DB* db) : db(db) {}

bool CallsTGetter::isCallsT(const ProgramElement& leftSide, const ProgramElement& rightSide) const {
    if (!(leftSide.elementType == ElementType::kProcedure && rightSide.elementType == ElementType::kProcedure))
        throw std::invalid_argument("Wrong element type for isCallsT");
    auto callsT = db->callsTTable.find(leftSide.procName);
    return (callsT != db->callsTTable.end() && callsT->second.find(rightSide.procName) != callsT->second.end());
}

std::set<ProgramElement> CallsTGetter::getLeftCallsT(const ProgramElement& rightSide,
                                                   const ElementType& typeToGet) const {
    std::set<ProgramElement> result;
    if (!(typeToGet == ElementType::kProcedure && rightSide.elementType == ElementType::kProcedure))
        throw std::invalid_argument("Wrong element type for getLeftSide on CallsT");
    auto callerT = db->callsTTableR.find(rightSide.procName);
    if (callerT == db->callsTTableR.end()) return {};
    for (const std::string& callerTProc : callerT->second)
        result.insert(ProgramElement::createProcedure(callerTProc));
    return result;
}

std::set<ProgramElement> CallsTGetter::getRightCallsT(const ProgramElement& leftSide,
                                                    const ElementType& typeToGet) const {
    std::set<ProgramElement> result;
    if (!(leftSide.elementType == ElementType::kProcedure && typeToGet == ElementType::kProcedure))
        throw std::invalid_argument("Wrong element type for getRightSide on CallsT");
    auto callsT = db->callsTTable.find(leftSide.procName);
    if (callsT == db->callsTTable.end()) return {};
    for (const std::string& calledProc : callsT->second)
        result.insert(ProgramElement::createProcedure(calledProc));
    return result;
}
