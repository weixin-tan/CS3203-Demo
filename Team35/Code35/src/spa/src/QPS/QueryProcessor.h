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
    static void createDeclarationObjects(std::vector<std::string> designEntityArr,
                                         std::unordered_map<std::string, Entity>* entityMap);
    static RelationshipRef createRelationshipObject(std::vector<std::string> relRefList,
                                                    std::unordered_map<std::string, Entity>* entityMap);
    static Entity findRelationshipEntity(const std::string& s, std::unordered_map<std::string, Entity>* entityMap);
    static RelationshipRef createPatternObject(std::vector<std::string> patternList,
                                               std::unordered_map<std::string, Entity>* entityMap);
    static Entity createExpressionEntity(const std::string& s);
    static Entity findRelationshipEntityWithAttribute(const std::string& s,
                                                      std::unordered_map<std::string, Entity>* entityMap);
    static RelationshipRef createWithObject(std::vector<std::string> clauseList,
                                            std::unordered_map<std::string, Entity>* entityMap);
    static void addIfNotDuplicate(Clause *newClause, const RelationshipRef& newRef);
};
#endif //SPA_SRC_SPA_SRC_QUERYPROCESSOR_H_
