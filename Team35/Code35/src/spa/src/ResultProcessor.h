#ifndef SPA_RESULTPROCESSOR_H
#define SPA_RESULTPROCESSOR_H

#include <vector>
#include <iostream>
#include "Result.h"

class ResultProcessor {
 public:
  ResultProcessor();
  std::set<ProgramElement> processResults(std::vector<Result> results);
};


#endif //SPA_RESULTPROCESSOR_H
