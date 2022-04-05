#include "ResultGroup.h"

#include <utility>

ResultGroup::ResultGroup() {
    ResultGroup::isBoolReturn = false;
    ResultGroup::isValid = false;
    ResultGroup::entitiesToReturn = {};
    ResultGroup::resultLists = {};
}

void ResultGroup::setBoolReturn(bool b) {
    ResultGroup::isBoolReturn = b;
}

void ResultGroup::setValid(bool b) {
    ResultGroup::isValid = b;
}


void ResultGroup::setEntitiesToReturn(std::vector<Entity> entities) {
    ResultGroup::entitiesToReturn = std::move(entities);
}

void ResultGroup::addResultList(const std::vector<Result>& r) {
    ResultGroup::resultLists.push_back(r);
}

bool ResultGroup::getIsBoolReturn() const {
    return ResultGroup::isBoolReturn;
}

bool ResultGroup::getIsValid() const {
    return ResultGroup::isValid;
}

std::vector<Entity> ResultGroup::getEntitiesToReturn() const {
    return ResultGroup::entitiesToReturn;
}

std::vector<std::vector<Result>> ResultGroup::getResultLists() const {
    return ResultGroup::resultLists;
}







