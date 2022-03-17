#include "CallsTGetter.h"

CallsTGetter::CallsTGetter(DB* db) : db(db) {}

bool CallsTGetter::isRelationship(const ProgramElement& leftSide, const ProgramElement& rightSide) {
    if (!(leftSide.elementType == ElementType::PROCEDURE && rightSide.elementType == ElementType::PROCEDURE))
        throw std::invalid_argument("Wrong element type for isCallsT");
    auto callsT = db->callsTTable.find(leftSide.procName);
    return (callsT != db->callsTTable.end() && callsT->second.find(rightSide.procName) != callsT->second.end());
}

std::set<ProgramElement> CallsTGetter::getLeftSide(const ProgramElement& rightSide, const ElementType& typeToGet) {
    std::set<ProgramElement> result;
    if (!(typeToGet == ElementType::PROCEDURE && rightSide.elementType == ElementType::PROCEDURE))
        throw std::invalid_argument("Wrong element type for getLeftSide on CallsT");
    auto callerT = db->callsTTableR.find(rightSide.procName);
    if (callerT == db->callsTTableR.end()) return {};
    for (const std::string& callerTProc : callerT->second)
        result.insert(ProgramElement::createProcedure(callerTProc));
    return result;
}

std::set<ProgramElement> CallsTGetter::getRightSide(const ProgramElement& leftSide, const ElementType& typeToGet) {
    std::set<ProgramElement> result;
    if (!(leftSide.elementType == ElementType::PROCEDURE && typeToGet == ElementType::PROCEDURE))
        throw std::invalid_argument("Wrong element type for getRightSide on CallsT");
    auto callsT = db->callsTTable.find(leftSide.procName);
    if (callsT == db->callsTTable.end()) return {};
    for (const std::string& calledProc : callsT->second)
        result.insert(ProgramElement::createProcedure(calledProc));
    return result;
}
