#include <list>
#include "ResultFormatter.h"

ResultFormatter::ResultFormatter() = default;

std::list<std::string> ResultFormatter::formatResult(const std::set<ProgramElement>& finalResult) {
  std::list<std::string> result;
  for(const ProgramElement& e : finalResult) {
    if(e.integer_value == ProgramElement::null_integer_value) {
      result.push_back(e.string_value);
    } else {
      result.push_back(std::to_string(e.integer_value));
    }
  }
  return result;
}
