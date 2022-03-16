#ifndef SPA_RESULTPROCESSOR_H
#define SPA_RESULTPROCESSOR_H


#include "Result.h"
#include "Group.h"
#include <vector>

class ResultProcessor {
 public:
  ResultProcessor();
  std::vector<ProgramElement> processResults(std::vector<Group> groups);
  int getIndexEntity(std::vector<Entity> v, Entity K);
};


#endif //SPA_RESULTPROCESSOR_H
