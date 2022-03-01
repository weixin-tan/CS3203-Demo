#ifndef SPA_SRC_SPA_SRC_QUERYREGEX_H_
#define SPA_SRC_SPA_SRC_QUERYREGEX_H_

#include <iostream>
#include <regex>
#include <unordered_map>
#include "Entity.h"
#include "RelationshipRef.h"

bool isIdent(const std::string& s);
bool isInteger(const std::string& s);
bool isWildCard(const std::string& s);

std::string extractFirstChar(const std::string& s);
std::string extractLastChar(const std::string& s);
std::string extractStringWithoutFirstAndLastChar(const std::string& s);
std::string extractStringFromQuotation(const std::string& s);
std::string extractStringFromWildCard(const std::string& s);

bool isQuotationIdent(const std::string& s);
bool isStringWithinWildCard(const std::string& s);
bool isStmtRef(const std::string& s);
bool isEntRef(const std::string& s);
bool entityMapContains(const std::string& s, std::unordered_map<std::string, Entity>* entityMap);

std::string stripString(std::string s);
std::vector<std::string> splitString(const std::string& s, const std::string& delimiter);
std::vector<std::string> splitStringBySpaces(const std::string& s);

bool isSelect(const std::string& s);
bool isPattern(const std::string& s);

std::vector<std::string> splitDeclarationAndSelect(const std::string& s);
std::vector<std::vector<std::string>> extractSelectAndDeclarationStatements(const std::string& s);
std::vector<std::string> extractDesignEntityAndSynonyms(const std::string& s);
bool checkDesignEntitySynonymsList(std::vector<std::string> sArr);

long findPatternClause(const std::string& s);
std::vector<long> findSuchThatClause(const std::string& s);
bool existSuchThat(const std::string& s);

bool existVBrackets(const std::string& s);
std::string removeVBrackets(const std::string& s);
bool checkAndSuchThat(std::vector<std::string> clausesList);
void removeAndSuchThat(std::vector<std::string>* clausesList);
bool checkAndPattern(std::vector<std::string> clausesList);
void removeAndPattern(std::vector<std::string>* clausesList);
std::string removeSelect(const std::string& s);
std::string removePattern(const std::string& s);
std::string removeSuchThat(std::string s);

std::vector<std::string> splitVariablesAndClauses(const std::string& s);
std::vector<std::string> splitPatternAndSuchThatClauses(std::string s);

std::vector<std::string> extractVariablesToSelect(const std::string& s);
std::vector<std::vector<std::string>> extractClauses(const std::string& s);

std::vector<std::string> extractItemsInBrackets(const std::string& s);

bool checkRelRefList(std::vector<std::string> s);
bool checkPatternList(std::vector<std::string> patternList, std::unordered_map<std::string, Entity>* entityMap);

bool checkRelationshipRef(const RelationshipRef& r);
bool checkFollowsOrParentsOrNextOrAffects(const RelationshipRef& r);
bool checkUses(const RelationshipRef& r);
bool checkModifies(const RelationshipRef& r);
bool checkCalls(const RelationshipRef& r);
bool checkPattern(const RelationshipRef& r);

#endif //SPA_SRC_SPA_SRC_QUERYREGEX_H_
