#ifndef SPA_SRC_SPA_SRC_QUERYREGEX_H_
#define SPA_SRC_SPA_SRC_QUERYREGEX_H_

#include <iostream>
#include <regex>
#include <unordered_map>
#include "Entity.h"

bool isIdent(const std::string& s);
bool isInteger(const std::string& s);
bool isWildCard(const std::string& s);
bool isQuotationIdent(const std::string& s);
bool isStmtRef(const std::string& s);
bool isEntRef(const std::string& s);
std::string stripString(std::string s);
std::vector<std::string> splitString(const std::string& s, const std::string& delimiter);

std::vector<std::string> splitDeclarationAndSelect(const std::string& s);
std::vector<std::string> extractSelect(const std::string& s);
std::vector<std::string> extractDeclaration(const std::string& s);

bool checkDesignEntitySynonyms(std::vector<std::string> sArr);
std::vector<std::string> extractDesignEntityAndSynonyms(const std::string& s);

bool isSelect(const std::string& s);
bool isPattern(const std::string& s);
std::vector<std::string> splitVariablesAndClauses(const std::string& s);
std::vector<std::string> extractVariablesToSelect(const std::string& s);
std::vector<std::string> extractSuchThatClauses(const std::string& s);
std::vector<std::string> extractPatternClauses(const std::string& s);

std::vector<std::string> extractItemsInBrackets(const std::string& s);
bool checkRelRefList(std::vector<std::string> s);

bool checkPatternList(std::vector<std::string> patternList, std::unordered_map<std::string, Entity> entityMap);
std::string removePattern(const std::string& s);

#endif //SPA_SRC_SPA_SRC_QUERYREGEX_H_
