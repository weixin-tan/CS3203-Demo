#include "Result.h"

#include <utility>

Result::Result() = default;

void Result::setNoClauseElements(std::set<ProgramElement> elements) {
  Result::noClauseElements = std::move(elements);
}

void Result::setSuchThatElements(std::set<std::pair<ProgramElement, ProgramElement>> elements) {
    Result::suchThatElements = std::move(elements);
}

void Result::setPatternElements(std::set<ProgramElement> elements) {
  Result::patternElements = std::move(elements);
}

void Result::setResultEntity(Entity entityToGet) {
  Result::resultEntity = std::move(entityToGet);
}

void Result::setLeftSuchThatEntity(Entity left) {
  Result::leftSuchThatEntity = std::move(left);
}

void Result::setRightSuchThatEntity(Entity right) {
  Result::rightSuchThatEntity = std::move(right);
}

void Result::setAssignEntity(Entity assign) {
  Result::assignEntity = std::move(assign);
}

std::set<ProgramElement> Result::getNoClauseElements() const {
    return Result::noClauseElements;
}

std::set<ProgramElement> Result::getPatternElements() const {
  return Result::patternElements;
}

std::set<std::pair < ProgramElement, ProgramElement>> Result::getSuchThatElements() const {
  return Result::suchThatElements;
}

Entity Result::getResultEntity() const {
    return Result::resultEntity;
}

Entity Result::getLeftSuchThatEntity() const {
    return Result::leftSuchThatEntity;
}

Entity Result::getRightSuchThatEntity() const {
    return Result::rightSuchThatEntity;
}

Entity Result::getAssignEntity() const {
    return Result::assignEntity;
}

bool Result::allSuchThatAndPatternEntitiesNull() const {
    if (Result::leftSuchThatEntity.eType == EntityType::Null && Result::rightSuchThatEntity.eType == EntityType::Null
    && Result::assignEntity.eType == EntityType::Null) {
      return true;
    } else {
      return false;
    }
}
