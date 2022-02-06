#ifndef SPA_SRC_SPA_SRC_QUERYPROCESSOR_H_
#define SPA_SRC_SPA_SRC_QUERYPROCESSOR_H_

#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Clause.h"
#include "QueryRegex.h"

class QueryProcessor {
 public:
  QueryProcessor();
  std::vector<Clause> parsePQL(std::string parsePQL);
};

#endif //SPA_SRC_SPA_SRC_QUERYPROCESSOR_H_
