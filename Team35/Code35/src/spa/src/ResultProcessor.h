#ifndef SPA_RESULTPROCESSOR_H
#define SPA_RESULTPROCESSOR_H


#include "Result.h"

class ResultProcessor {
 public:
  ResultProcessor();
  Result processResults(std::vector<Result> results);

};


#endif //SPA_RESULTPROCESSOR_H
