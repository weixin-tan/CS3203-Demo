#ifndef SPA_SRC_SPA_SRC_QUERYPROCESSOR_H_
#define SPA_SRC_SPA_SRC_QUERYPROCESSOR_H_

#include <string>
#include <vector>
#include <unordered_map>
#include "Clause.h"
#include "QueryRegex.h"

class QueryProcessor {
 public:
  QueryProcessor();
  std::vector<Clause> parsePQL(const std::string& parsePQL);
 private:
  static std::unordered_map<std::string, Entity> createDeclarationObjects(std::vector<std::string> designEntityArr, std::unordered_map<std::string, Entity> entityMap);
  static RelationshipRef createRelationshipObject(std::vector<std::string> relRefList, const std::unordered_map<std::string, Entity>& entityMap);
  static Entity findRelationshipEntity(const std::string& s, std::unordered_map<std::string, Entity> entityMap);
};

#endif //SPA_SRC_SPA_SRC_QUERYPROCESSOR_H_
