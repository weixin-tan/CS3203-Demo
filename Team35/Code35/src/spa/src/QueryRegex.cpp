#include "QueryRegex.h"

bool isIdent(const std::string& s){
  std::regex identRegex("[A-Za-z][A-Za-z0-9]*");
  return std::regex_match(s, identRegex);
}

bool isInteger(const std::string& s){
  std::regex integerRegex("[0-9]+");
  return std::regex_match(s, integerRegex);
}

bool isWildCard(const std::string& s) {
  return s == "_";
}

bool isQuotationIdent(const std::string& s){
  std::regex quotationIdentRegex("\"[A-Za-z][A-Za-z0-9]*\"");
  return std::regex_match(s, quotationIdentRegex);
}

bool isStringWithinWildCard(const std::string& s){
  return s.substr(0,2) == "_\"" && s.substr(s.length()-2, 2) == "\"_";
}

bool isStmtRef(const std::string& s){
  return isIdent(s) || isWildCard(s) || isInteger(s) ;
}

bool isEntRef(const std::string& s){
  return isIdent(s) || isWildCard(s) || isQuotationIdent(s);
}

std::string stripString(std::string s){
  const char* whitespace = " \t\n\r\f\v";
  s.erase(0, s.find_first_not_of(whitespace));
  s.erase(s.find_last_not_of(whitespace) + 1);
  return s;
}

std::vector<std::string> splitString(const std::string& s, const std::string& delimiter){
  long startPosition = 0;
  long endPosition;
  long delimiterLength = delimiter.length();
  std::string placeholder;
  std::vector<std::string> strList;
  endPosition = s.find(delimiter, startPosition);
  while (endPosition != std::string::npos) {
    placeholder = s.substr (startPosition, endPosition - startPosition);
    startPosition = endPosition + delimiterLength;
    strList.push_back(stripString(placeholder));

    endPosition = s.find(delimiter, startPosition);
  }
  strList.push_back(stripString(s.substr(startPosition)));
  return strList;
}

std::vector<std::string> splitDeclarationAndSelect(const std::string& s){
  return splitString(s, ";");
}

std::vector<std::string> extractSelect(const std::string& s){
  std::vector<std::string> returnList;
  std::vector<std::string> stmtList = splitDeclarationAndSelect(s);
  for (const std::string& stmt: stmtList){
    if (stmt.find("Select") == 0){
      // select exists as the first word in the statement
      returnList.push_back(stmt);
    }
  }
  return returnList;
}

std::vector<std::string> extractDeclaration(const std::string& s){
  std::vector<std::string> returnList;
  std::vector<std::string> stmtList = splitDeclarationAndSelect(s);
  for (const std::string& stmt: stmtList){
    if (stmt.find("Select") == std::string::npos){
      // select does not exist in the statement
      returnList.push_back(stmt);
    }
  }
  return returnList;
}

bool checkDesignEntitySynonyms(std::vector<std::string> sArr) {
  std::vector<std::string> designEntity {"stmt", "read", "print", "call", "while", "if", "assign", "variable", "constant", "procedure"};
  bool returnBool;
  if (sArr.size() <= 1){
    returnBool = false;
  }else{
    std::string designStr = sArr[0];
    if (std::find(std::begin(designEntity), std::end(designEntity), designStr) != std::end(designEntity)){
      //designStr contains one of the design entity keywords
      returnBool = true;
    }else{
      returnBool = false;
    }
    for (int i=1; i<sArr.size(); i++){
      returnBool = returnBool && isIdent(sArr[i]);
    }
  }
  return returnBool;
}

std::vector<std::string> extractDesignEntityAndSynonyms(const std::string& s){
  std::vector<std::string> returnList;
  std::vector<std::string> laterSynonymsList = splitString(s, ",");
  std::vector<std::string> frontDesignEntityAndSynonym = splitString(laterSynonymsList.front(), " ");
  std::string tempString;
  for(int i=0; i<frontDesignEntityAndSynonym.size(); i++){
    tempString = stripString(frontDesignEntityAndSynonym[i]);
    if (!tempString.empty()){
      returnList.push_back(frontDesignEntityAndSynonym[i]);
    }
  }

  if (returnList.size() != 2){
    std::vector<std::string> emptyList;
    return emptyList;
  }

  for(int i=1; i<laterSynonymsList.size(); i++){
    tempString = stripString(laterSynonymsList[i]);
    if (!tempString.empty()){
      returnList.push_back(stripString(laterSynonymsList[i]));
    }
  }
  return returnList;
}

bool isSelect(const std::string& s){
  return s.find("Select") == 0 ;
}

bool isPattern(const std::string& s){
  return s.find("pattern") == 0;
}

std::vector<std::string> splitVariablesAndClauses(const std::string& s){
  std::vector<std::string> returnList;
  std::string varString;
  std::string clauseString;
  long suchThatPosition = s.find("such");
  long patternPosition = s.find("pattern");
  if ((suchThatPosition == std::string::npos) && (patternPosition == std::string::npos)){
    returnList.push_back(s);
  }else if (suchThatPosition == std::string::npos){
    // pattern
    varString = s.substr(0, patternPosition);
    clauseString = s.substr(patternPosition, s.length() - patternPosition);
    returnList.push_back(varString);
    returnList.push_back(clauseString);
  }else if (patternPosition == std::string::npos){
    varString = s.substr(0, suchThatPosition);
    clauseString = s.substr(suchThatPosition, s.length() - suchThatPosition);
    returnList.push_back(varString);
    returnList.push_back(clauseString);
  }else if (patternPosition < suchThatPosition){
    varString = s.substr(0, patternPosition);
    clauseString = s.substr(patternPosition, s.length() - patternPosition);
    returnList.push_back(varString);
    returnList.push_back(clauseString);
  }else{
    varString = s.substr(0, suchThatPosition);
    clauseString = s.substr(suchThatPosition, s.length() - suchThatPosition);
    returnList.push_back(varString);
    returnList.push_back(clauseString);
  }
  return returnList;
}

std::vector<std::string> splitPatternAndSuchThatClauses(std::string s){
  std::vector<std::string> returnList;
  long suchThatPosition = s.find("such");
  long patternPosition = s.find("that");
  if (suchThatPosition != std::string::npos){
    // remove such that
    s = s.substr(0, suchThatPosition) + s.substr(patternPosition+4, s.length() - patternPosition-4);
  }
  std::vector<std::string> wordsList = splitString(s, ")");
  for (int i=0; i<wordsList.size(); i=i+1){
    if (!wordsList[i].empty()){
      returnList.push_back(wordsList[i] + ")");
    }
  }
  return returnList;
}

std::vector<std::string> extractVariablesToSelect(const std::string& s){
  std::vector<std::string> returnList;
  std::string variableString = splitVariablesAndClauses(s)[0];
  if (isSelect(variableString)){
    returnList.push_back(stripString(variableString.substr(6,variableString.length()-6)));
  }
  return returnList;
}

std::vector<std::string> extractSuchThatClauses(const std::string& s){
  std::vector<std::string> returnList;
  std::vector<std::string> everythingList = splitVariablesAndClauses(s);
  if (everythingList.size() == 2) {
    std::string clausesString = everythingList[1];
    std::vector<std::string> clausesList = splitPatternAndSuchThatClauses(clausesString);

    for (const auto &stmt : clausesList) {
      if (!isPattern(stmt)) {
        returnList.push_back(stmt);
      }
    }
  }
  return returnList;
}

std::vector<std::string> extractPatternClauses(const std::string& s){
  std::vector<std::string> returnList;
  std::vector<std::string> everythingList = splitVariablesAndClauses(s);
  if (everythingList.size() == 2) {
    std::string clausesString = splitVariablesAndClauses(s)[1];
    std::vector<std::string> clausesList = splitPatternAndSuchThatClauses(clausesString);

    for (const auto &stmt : clausesList) {
      if (isPattern(stmt)) {
        returnList.push_back(stmt);
      }
    }
  }
  return returnList;
}

std::vector<std::string> extractItemsInBrackets(const std::string& s){
  std::vector<std::string> returnList;

  long openBracketPosition = s.find('(');
  long commaPosition = s.find(',');
  long closeBracketPosition = s.find(')');

  if (openBracketPosition < commaPosition && commaPosition < closeBracketPosition){
    returnList.push_back(stripString(s.substr(0, openBracketPosition)));
    returnList.push_back(stripString(s.substr(openBracketPosition+1, commaPosition - openBracketPosition - 1)));
    returnList.push_back(stripString(s.substr(commaPosition+1, closeBracketPosition - commaPosition - 1)));
  }
  return returnList;
}

bool checkRelRefList(std::vector<std::string> sArr){
  if (sArr.size() != 3){
    return false;
  }else{
    if (sArr[0] == "Follows" || sArr[0] ==  "Follows*" || sArr[0] ==  "Parent" || sArr[0] == "Parent*"){
      return isStmtRef(sArr[1]) && isStmtRef(sArr[2]);
    }else if (sArr[0] == "Uses" || sArr[0] == "Modifies"){
      return (isEntRef(sArr[1]) && isEntRef(sArr[2])) || (isStmtRef(sArr[1]) && isEntRef(sArr[2]));
    }else{
      return false;
    }
  }
}

bool checkPatternList(std::vector<std::string> patternList, std::unordered_map<std::string, Entity>* entityMap){
  if (patternList.size() == 3 && isEntRef(patternList[1]) && entityMapContains(patternList[0], entityMap)){
    return true;
  }
  return false;
}

std::string removePattern(const std::string& s){
  if (s.find("pattern") == 0){
    return stripString(s.substr(7,s.length()));
  }
  return s;
}

bool entityMapContains(const std::string& s, std::unordered_map<std::string, Entity>* entityMap){
  return (*entityMap).find(s) != (*entityMap).end();
}
