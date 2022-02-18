#include <iostream>
#include "ResultProcessor.h"

ResultProcessor::ResultProcessor() = default;

Result ResultProcessor::processResults(std::vector<Result> results) {
  std::set<ProgramElement> resultElements;
  if (results.size() == 2) {
    std::set<ProgramElement> set1 = results[0].getProgramElements();
    std::set<ProgramElement> set2 = results[1].getProgramElements();
    for (const auto& e : set1) {
      if (set2.count(e)) {
        resultElements.insert(e);
      }
    }
    return Result(resultElements);
  } else {
    return results.front();
  }
}
