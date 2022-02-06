#ifndef SPA_RESULT_H
#define SPA_RESULT_H


#include <vector>
#include "Entity.h"

class Result {
 public:
  Result();
  explicit Result(std::vector<Entity> entities);

  std::vector<Entity> resultEntities = {};

  void appendResultEntity(const Entity& e);
  std::vector<Entity> getResultEntities();
};


#endif //SPA_RESULT_H
