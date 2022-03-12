#include "Result.h"

#include <utility>

// To initialise empty result class
Result::Result() {
  type = ResultType::NullClause;
  valid = false;
  oneSynEntity = Entity();
  oneSynSet = {};
  twoSynEntities = std::pair<Entity, Entity>(Entity(), Entity());
  twoSynSet = {};
}

void Result::setResultType(ResultType r) {
  Result::type = r;
}

void Result::setValid(bool b) {
  Result::valid = b;
}

void Result::setOneSynEntity(Entity e) {
  Result::oneSynEntity = e;
}

void Result::setOneSynSet(std::set<ProgramElement> s) {
  Result::oneSynSet = s;
}

void Result::setTwoSynEntities(std::pair<Entity, Entity> p) {
  Result::twoSynEntities = p;
}

void Result::setTwoSynSet(std::set<std::pair<ProgramElement, ProgramElement>> t) {
  Result::twoSynSet = t;
}

ResultType Result::getResultType() {
  return Result::type;
}

bool Result::getValid() {
  return Result::valid;
}

Entity Result::getOneSynEntity() {
  return Result::oneSynEntity;
}

std::set<ProgramElement> Result::getOneSynSet() {
  return Result::oneSynSet;
}

std::pair<Entity, Entity> Result::getTwoSynEntities() {
  return Result::twoSynEntities;
}

std::set<std::pair<ProgramElement, ProgramElement>> Result::getTwoSynSet() {
  return Result::twoSynSet;
}
