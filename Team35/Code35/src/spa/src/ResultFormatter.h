//
// Created by viv on 4/2/2022.
//

#ifndef SPA_RESULTFORMATTER_H
#define SPA_RESULTFORMATTER_H


#include <string>
#include "Result.h"

class ResultFormatter {
 public:
  ResultFormatter();
  std::string formatResult(Result finalResult);
};


#endif //SPA_RESULTFORMATTER_H
