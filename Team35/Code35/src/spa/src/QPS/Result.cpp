#include <utility>

#include "Result.h"

// To initialise empty result class
Result::Result() {
    type = ResultType::NULL_CLAUSE;
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
    Result::oneSynEntity = std::move(e);
}

void Result::setOneSynSet(const std::set<ProgramElement*>& s) {
    Result::oneSynSet = s;
}

void Result::setTwoSynEntities(std::pair<Entity, Entity> p) {
    Result::twoSynEntities = std::move(p);
}

void Result::setTwoSynSet(const std::set<std::pair<ProgramElement*, ProgramElement*>>& t) {
    Result::twoSynSet = t;
}

ResultType Result::getResultType() {
    return Result::type;
}

bool Result::getValid() const {
    return Result::valid;
}

Entity Result::getOneSynEntity() const {
    return Result::oneSynEntity;
}

std::set<ProgramElement*> Result::getOneSynSet() const {
    return Result::oneSynSet;
}

std::pair<Entity, Entity> Result::getTwoSynEntities() const {
    return Result::twoSynEntities;
}

std::set<std::pair<ProgramElement*, ProgramElement*>> Result::getTwoSynSet() const {
    return Result::twoSynSet;
}

std::set<ProgramElement> convertToLogicalOneSyn(const std::set<ProgramElement*>& pointerSet) {
    std::set<ProgramElement> logicalSet;
    for (auto ptr : pointerSet) {
        logicalSet.insert(*ptr);
    }
    return logicalSet;
}

std::set<std::pair<ProgramElement,ProgramElement>>
convertToLogicalTwoSyn(const std::set<std::pair<ProgramElement*, ProgramElement*>>& pointerSet) {
    std::set<std::pair<ProgramElement, ProgramElement>> logicalSet;
    for (auto pair : pointerSet) {
        logicalSet.insert(std::pair<ProgramElement, ProgramElement>(*pair.first, *pair.second));
    }
    return logicalSet;
}

bool Result::operator==(const Result& r1) const {
    return type == r1.type &&
           valid == r1.valid &&
           oneSynEntity == r1.oneSynEntity &&
           convertToLogicalOneSyn(oneSynSet) == convertToLogicalOneSyn(r1.oneSynSet) &&
           twoSynEntities == r1.twoSynEntities &&
           convertToLogicalTwoSyn(twoSynSet) == convertToLogicalTwoSyn(r1.twoSynSet);
}
