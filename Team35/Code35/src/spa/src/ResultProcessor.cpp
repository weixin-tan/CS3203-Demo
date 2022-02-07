#include "ResultProcessor.h"

ResultProcessor::ResultProcessor() = default;

Result ResultProcessor::processResults(std::vector<Result> results) {
  Result r = results.front();
  return r;
}
