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

bool Result::getValid() const {
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

bool Result::operator==(const Result& r1) const {
    return type == r1.type &&
            valid == r1.valid &&
            oneSynEntity == r1.oneSynEntity &&
            oneSynSet == r1.oneSynSet &&
            twoSynEntities == r1.twoSynEntities &&
            twoSynSet == r1.twoSynSet;
}
