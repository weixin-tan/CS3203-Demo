#ifndef SPA_RESULTPROCESSOR_H
#define SPA_RESULTPROCESSOR_H


#include "Result.h"
#include <vector>

class ResultProcessor {
 public:
  ResultProcessor();
  std::set<ProgramElement> processResults(std::vector<Result> results);
  int getIndexEntity(std::vector<Entity> v, Entity K);
};


#endif //SPA_RESULTPROCESSOR_H
