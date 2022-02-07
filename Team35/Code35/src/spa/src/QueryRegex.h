#ifndef SPA_SRC_SPA_SRC_QUERYREGEX_H_
#define SPA_SRC_SPA_SRC_QUERYREGEX_H_

#include <iostream>
#include <regex>

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

std::vector<std::string> extractDesignEntityAndSynonym(const std::string& s);
bool checkDesignEntitySynonyms(std::vector<std::string> sArr);

bool checkSelect(const std::string& s);
bool checkSuchThat(const std::string& s);
std::vector<std::string> extractVariablesClauses(const std::string& s);
std::vector<std::string> extractVariablesToSelect(const std::string& s);
std::vector<std::string> extractSuchThatClauses(const std::string& s);
std::vector<std::string> extractPatternClauses(const std::string& s);

std::vector<std::string> extractItemsInBrackets(const std::string& s);
bool checkRelRefArr(std::vector<std::string> s);

//bool checkPattern(std::string s);
//bool checkExpression(std::string s);

#endif //SPA_SRC_SPA_SRC_QUERYREGEX_H_
