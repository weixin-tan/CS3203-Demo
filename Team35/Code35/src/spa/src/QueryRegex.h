#ifndef SPA_SRC_SPA_SRC_QUERYREGEX_H_
#define SPA_SRC_SPA_SRC_QUERYREGEX_H_

#include <stdio.h>
#include <iostream>
#include <regex>

bool isIdent(std::string s);
bool isInteger(std::string s);
bool isWildCard(std::string s);
bool isQuotationIdent(std::string s);
bool isStmtRef(std::string s);
bool isEntRef(std::string s);
std::string stripString(std::string s);
std::vector<std::string> splitString(std::string s, std::string delimiter);

std::vector<std::string> splitDeclarationAndSelect(std::string s);
std::vector<std::string> extractSelect(std::string s);
std::vector<std::string> extractDeclaration(std::string s);

std::vector<std::string> extractDesignEntityandSynonym(std::string s);
bool checkDesignEntitySynonyms(std::vector<std::string> sArr);

bool checkSelect(std::string s);
bool checkSuchThat(std::string s);
std::vector<std::string> extractVariablesClauses(std::string s);
std::vector<std::string> extractVariablesToSelect(std::string s);
std::vector<std::string> extractSuchThatClauses(std::string s);
std::vector<std::string> extractPatternCaluses(std::string s);

std::vector<std::string> extractItemsInBrackets(std::string s);
bool checkRelRefArr(std::vector<std::string> s);

//bool checkPattern(std::string s);
//bool checkExpression(std::string s);

#endif //SPA_SRC_SPA_SRC_QUERYREGEX_H_
