#include <iostream>
#include "ResultProcessor.h"

ResultProcessor::ResultProcessor() = default;

Result ResultProcessor::processResults(std::vector<Result> results) {
  std::vector<Entity> resultEntities;
  if (results.size() == 2) {
    std::vector<Entity> r1 = results[0].getResultEntities();
      std::vector<Entity> r2 = results[1].getResultEntities();
    for (Entity e : r1) {
      if (std::find(r2.begin(), r2.end(), e) !=r2.end()) {
        resultEntities.push_back(e);
      }
    }
    return Result(resultEntities);
  } else {
    return results.front();
  }
}
