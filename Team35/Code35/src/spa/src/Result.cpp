#include "Result.h"

#include <utility>

Result::Result() = default;

Result::Result(std::set <ProgramElement> programElements) {
    Result::programElements = std::move(programElements);
}

void Result::setRelationshipRef(const RelationshipRef& r) {
    Result::relRef.rType = r.rType;
    Result::relRef.leftEntity = r.leftEntity;
    Result::relRef.rightEntity = r.rightEntity;
}

void Result::setEntityToFind(const Entity& e) {
    Result::entityToFind.eType = e.eType;
    Result::entityToFind.name = e.name;
}

std::set<ProgramElement> Result::getProgramElements() {
    return programElements;
}

