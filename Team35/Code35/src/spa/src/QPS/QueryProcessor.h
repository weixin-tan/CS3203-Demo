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
    std::vector<Clause> parsePQL(const std::string& queryString);
private:
    static void createDeclarationObjects(std::vector<std::string>* designEntityArr,
                                         std::unordered_map<std::string, Entity>* entityMap);
    static RelationshipRef createSuchThatRelationship(std::vector<std::string>* relRefList,
                                                      std::unordered_map<std::string, Entity>* entityMap);
    static Entity getSuchThatEntity(const std::string& s, std::unordered_map<std::string, Entity>* entityMap);
    static RelationshipRef createPatternRelationship(std::vector<std::string>* patternList,
                                                     std::unordered_map<std::string, Entity>* entityMap);
    static Entity createExpressionEntity(const std::string& s);
    static Entity createEntityWithAttribute(const std::string& s,
                                            std::unordered_map<std::string, Entity>* entityMap);
    static RelationshipRef createWithRelationship(std::vector<std::string>* clauseList,
                                                  std::unordered_map<std::string, Entity>* entityMap);
    static void addIfNotDuplicate(Clause* newClause, const RelationshipRef& newRef);

    static void handleVariablesToFind(std::vector<std::string>* variablesToSelect,
                                      Clause* newClause,
                                      bool* isValid,
                                      std::unordered_map<std::string,Entity>* entityMap);

    static void handleSuchThat(std::vector<std::string>* SuchThatClauses,
                               Clause* newClause,
                               bool* isValid,
                               bool* isSemanticallyValid,
                               std::unordered_map<std::string, Entity>* entityMap);

    static void handlePattern(std::vector<std::string>* PatternClauses,
                              Clause* newClause,
                              bool* isValid,
                              bool* isSemanticallyValid,
                              std::unordered_map<std::string, Entity>* entityMap);

    static void handleWith(std::vector<std::string>* WithClauses,
                           Clause* newClause,
                           bool* isValid,
                           bool* isSemanticallyValid,
                           std::unordered_map<std::string, Entity>* entityMap);
};
#endif //SPA_SRC_SPA_SRC_QUERYPROCESSOR_H_
