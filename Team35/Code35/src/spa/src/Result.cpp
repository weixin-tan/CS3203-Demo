#include "Result.h"

Result::Result() = default;

Result::Result(std::vector<Entity> entities) {
  Result::resultEntities = entities;
}

void Result::appendResultEntity(const Entity& e) {
  resultEntities.push_back(e);
}

std::vector<Entity> Result::getResultEntities() {
    return resultEntities;
}
