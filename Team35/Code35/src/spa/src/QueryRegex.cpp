#include "QueryRegex.h"

bool isIdent(std::string s){
  std::regex identRegex("[A-Za-z][A-Za-z0-9]*");
  return std::regex_match(s, identRegex);
}

bool isInteger(std::string s){
  std::regex integerRegex("[0-9]+");
  return std::regex_match(s, integerRegex);
}

bool isWildCard(std::string s) {
  std::regex wildCardRegex("_");
  return std::regex_match(s, wildCardRegex);
}

bool isQuotationIdent(std::string s){
  std::regex quotationIdentRegex("\"[A-Za-z][A-Za-z0-9]*\"");
  return std::regex_match(s, quotationIdentRegex);
}

bool isStmtRef(std::string s){
  return isIdent(s) || isWildCard(s) || isInteger(s) ;
}

bool isEntRef(std::string s){
  return isIdent(s) || isWildCard(s) || isQuotationIdent(s);
}

std::vector<std::string> splitString(std::string s, std::string delimiter){
  int startPosition = 0;
  int endPosition;
  int delimiterLength = delimiter.length();
  std::string placeholder;
  std::vector<std::string> strList;

  endPosition = s.find(delimiter, startPosition);
  while (endPosition != std::string::npos) {
    placeholder = s.substr (startPosition, endPosition - startPosition);
    startPosition = endPosition + delimiterLength;
    strList.push_back(placeholder);

    endPosition = s.find(delimiter, startPosition);
  };

  strList.push_back (s.substr(startPosition));
  return strList;
}

std::vector<std::string> splitDeclarationAndSelect(std::string s){
  return splitString(s, ";");
}

std::vector<std::string> extractSelect(std::string s){
  std::vector<std::string> returnList;
  std::vector<std::string> stmtList = splitDeclarationAndSelect(s);
  for (std::string stmt: stmtList){
    if (stmt.find("Select") != std::string::npos){
      returnList.push_back(stmt);
    }
  }
  return returnList;
}

std::vector<std::string> extractDeclaration(std::string s){
  std::vector<std::string> returnList;
  std::vector<std::string> stmtList = splitDeclarationAndSelect(s);
  for (std::string stmt: stmtList){
    if (stmt.find("Select") == std::string::npos){
      returnList.push_back(stmt);
    }
  }
  return returnList;
}

std::vector<std::string> extractDesignEntityandSynonym(std::string s);