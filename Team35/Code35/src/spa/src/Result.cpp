#include "Result.h"

#include <utility>

Result::Result() = default;

void Result::setNoClauseElements(std::set<ProgramElement> elements) {
  Result::noClauseElements = elements;
}

void Result::setSuchThatElements(std::set<std::pair<ProgramElement, ProgramElement>> elements) {
    Result::suchThatElements = elements;
}

void Result::setPatternElements(std::set<ProgramElement> elements) {
  Result::patternElements = elements;
}

void Result::setResultEntity(Entity entityToGet) {
  Result::resultEntity = entityToGet;
}

void Result::setLeftSuchThatEntity(Entity left) {
  Result::leftSuchThatEntity = left;
}

void Result::setRightSuchThatEntity(Entity right) {
  Result::rightSuchThatEntity = right;
}

void Result::setAssignEntity(Entity assign) {
  Result::assignEntity = assign;
}

void Result::setAssignEntRef(Entity entRef) {
  Result::assignEntRef = entRef;
}

void Result::setEntRefElements(std::set<std::pair<ProgramElement, ProgramElement>> elements) {
  Result::entRefElements = elements;
}

std::set<ProgramElement> Result::getNoClauseElements() {
    return Result::noClauseElements;
}

std::set<ProgramElement> Result::getPatternElements() {
  return Result::patternElements;
}

std::set<std::pair < ProgramElement, ProgramElement>> Result::getSuchThatElements() {
  return Result::suchThatElements;
}

Entity Result::getResultEntity() {
    return Result::resultEntity;
}

Entity Result::getLeftSuchThatEntity() {
    return Result::leftSuchThatEntity;
}

Entity Result::getRightSuchThatEntity() {
    return Result::rightSuchThatEntity;
}

Entity Result::getAssignEntity() {
    return Result::assignEntity;
}

Entity Result::getAssignEntRef() {
  return Result::assignEntRef;
}

std::set<std::pair<ProgramElement, ProgramElement>> Result::getEntRefElements() {
    return Result::entRefElements;
}

bool Result::allSuchThatAndPatternEntitiesNull() {
    if (Result::leftSuchThatEntity.eType == EntityType::Null && Result::rightSuchThatEntity.eType == EntityType::Null
    && Result::assignEntity.eType == EntityType::Null) {
      return true;
    } else {
      return false;
    }
}
