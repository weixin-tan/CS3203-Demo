#include "ResultFormatter.h"

ResultFormatter::ResultFormatter() = default;

std::string ResultFormatter::formatResult(Result finalResult) {
  std::vector<Entity> resultEntities = finalResult.getResultEntities();
  std::string result;
  for(const Entity& e : resultEntities) {
    result += e.name + ", ";
  }
  
  return result;
}
