#include <list>
#include "ResultFormatter.h"

ResultFormatter::ResultFormatter() = default;

std::list<std::string> ResultFormatter::formatResult(Result finalResult) {
  std::vector<Entity> resultEntities = finalResult.getResultEntities();
  std::list<std::string> result;
  for(const Entity& e : resultEntities) {
    result.push_back(e.name);
  }
  return result;
}
