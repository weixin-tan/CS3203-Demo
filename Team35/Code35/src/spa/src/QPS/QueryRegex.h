#ifndef SPA_SRC_SPA_SRC_QUERYREGEX_H_
#define SPA_SRC_SPA_SRC_QUERYREGEX_H_

#include <iostream>
#include <regex>
#include <unordered_map>
#include "Entity.h"
#include "RelationshipRef.h"

bool firstWordChecker(const std::string& s, const std::string& targetWord);
bool isSpaces(const std::string& s);
bool doesStringExist(const std::string& s, const std::string& substring);
bool entityMapContains(const std::string& s, std::unordered_map<std::string, Entity>* entityMap);

bool isIdent(const std::string& s);
bool isInteger(const std::string& s);
bool isWildCard(const std::string& s);
bool isQuotationIdent(const std::string& s);
bool isStringWithinQuotations(const std::string& s);
bool isStringWithinWildCard(const std::string& s);
bool isStmtRef(const std::string& s);
bool isEntRef(const std::string& s);

bool isSelect(const std::string& s);
bool isPattern(const std::string& s);
bool isWith(const std::string& s);
bool existSuchThat(const std::string& s);

bool checkDesignEntitySynonymsList(std::vector<std::string> sArr);
bool checkRelRefList(std::vector<std::string> s);
bool checkPatternList(std::vector<std::string> patternList, std::unordered_map<std::string, Entity>* entityMap);

std::string extractFirstChar(const std::string& s);
std::string extractLastChar(const std::string& s);
std::string extractStringWithoutFirstAndLastChar(const std::string& s);
std::string extractStringFromQuotation(const std::string& s);
std::string extractStringFromWildCard(const std::string& s);
std::string stripString(std::string s);

std::string removeSelect(const std::string& s);
std::string removeSuchThat(const std::string& s);
std::string removePattern(const std::string& s);
std::string removeWith(const std::string& s);

long findClauseStartPosition(const std::string& s, const std::string& commandWord, const std::string& original);
long findPatternClause(const std::string& s);
long findWithClause(const std::string& s);
long findPatternClauseInSubstring(const std::string& s, std::string original);
std::vector<long> findSuchThatClause(const std::string& s);
long smallestNumber(const long positionList[3]);

std::vector<std::string> splitString(const std::string& s, const std::string& delimiter);
std::vector<std::string> splitStringBySpaces(const std::string& s);

bool existVBrackets(const std::string& s);
std::string removeVBrackets(const std::string& s);
bool checkAndCommandWord(const std::vector<std::string>& clausesList,
                         bool (* existCommandWordFunction)(const std::string& s));
void removeAndCommandWord(std::vector<std::string>* clausesList,
                          std::string (* removeCommandWordFunction)(const std::string& s));
bool checkAndSuchThat(const std::vector<std::string>& clausesList);
void removeAndSuchThat(std::vector<std::string>* clausesList);
bool checkAndPattern(const std::vector<std::string>& clausesList);
void removeAndPattern(std::vector<std::string>* clausesList);
bool checkAndWith(const std::vector<std::string>& clausesList);
void removeAndWith(std::vector<std::string>* clausesList);

std::vector<std::string> splitDeclarationAndSelect(const std::string& s);
std::vector<std::vector<std::string>> extractSelectAndDeclarationStatements(const std::string& s);
std::vector<std::string> extractDesignEntityAndSynonyms(const std::string& s);

std::vector<std::string> splitVariablesAndClauses(const std::string& s);
std::vector<std::string> extractWithClauses(const std::string& s);
std::vector<std::string> splitSuchThatPatternWithClauses(const std::string& s);
std::vector<std::string> extractVariablesToSelect(const std::string& s);
std::vector<std::vector<std::string>> extractClauses(const std::string& s);

std::vector<std::string> extractItemsInBrackets(const std::string& s);
std::vector<std::string> extractPatternBrackets(const std::string& s);

bool checkRelationshipRef(const RelationshipRef& r);
bool checkEntityIsStmtRef(const Entity& e);
bool checkFollowsOrParentsOrNextOrAffects(const RelationshipRef& r);
bool checkUsesLeftSide(const Entity& e);
bool checkUses(const RelationshipRef& r);
bool checkModifiesLeftSide(const Entity& e);
bool checkModifies(const RelationshipRef& r);
bool checkCallsEntity(const Entity& e);
bool checkCalls(const RelationshipRef& r);
bool checkPattern(const RelationshipRef& r);
bool checkWith(const RelationshipRef& r);
bool checkWithEntity(const Entity& e);
bool checkVariableToSelect(const Entity& e);

#endif //SPA_SRC_SPA_SRC_QUERYREGEX_H_