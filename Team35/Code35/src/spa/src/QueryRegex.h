#ifndef SPA_SRC_SPA_SRC_QUERYREGEX_H_
#define SPA_SRC_SPA_SRC_QUERYREGEX_H_

#include <stdio.h>
#include <regex>

bool isIdent(std::string s);
bool isInteger(std::string s);
bool isWildCard(std::string s);
bool isQuotationIdent(std::string s);
bool isStmtRef(std::string s);
bool isEntRef(std::string s);
std::vector<std::string> splitString(std::string s, std::string delimiter);

std::vector<std::string> splitDeclarationAndSelect(std::string s);
std::vector<std::string> extractSelect(std::string s);
std::vector<std::string> extractDeclaration(std::string s);

std::vector<std::string> extractDesignEntityandSynonym(std::string s);
bool checkDesignEntity(std::string s);

bool checkSelect(std::string s);
std::vector<std::string> splitSuchThatAndPattern(std::string s);

bool checkSuchThat(std::string s);
bool checkRelRef(std::string s);
std::vector<std::string> extractRelRef(std::string s);

bool checkPattern(std::string s);
std::vector<std::string> extractPattern(std::string s);

bool checkExpression(std::string s);

#endif //SPA_SRC_SPA_SRC_QUERYREGEX_H_
