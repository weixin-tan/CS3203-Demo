#ifndef SPA_RESULTFORMATTER_H
#define SPA_RESULTFORMATTER_H


#include <string>
#include "Result.h"

class ResultFormatter {
 public:
  ResultFormatter();
  std::list<std::string> formatResult(Result finalResult);
};


#endif //SPA_RESULTFORMATTER_H
