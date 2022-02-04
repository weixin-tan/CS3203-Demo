//
// Created by viv on 4/2/2022.
//

#ifndef SPA_RESULT_H
#define SPA_RESULT_H


#include <vector>
#include "Entity.h"

class Result {
 public:
  Result();

  std::vector<Entity> resultEntities;

  void appendResultEntity(Entity e);
  std::string toString();

};


#endif //SPA_RESULT_H
