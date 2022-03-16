#ifndef SPA_RESULTFORMATTER_H
#define SPA_RESULTFORMATTER_H


#include <string>
#include <list>
#include "Result.h"

class ResultFormatter {
 public:
  ResultFormatter();
  std::list<std::string> formatResult(const std::vector<ProgramElement>& finalResult);
};


#endif //SPA_RESULTFORMATTER_H
