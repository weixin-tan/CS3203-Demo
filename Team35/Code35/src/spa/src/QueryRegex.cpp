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
  std::regex wildCardRegex("_");
  return std::regex_match(s, wildCardRegex);
}

bool isQuotationIdent(const std::string& s){
  std::regex quotationIdentRegex("\"[A-Za-z][A-Za-z0-9]*\"");
  return std::regex_match(s, quotationIdentRegex);
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
    if (stmt.find("Select") != std::string::npos){
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
    returnBool = true;
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

std::vector<std::string> extractDesignEntityAndSynonym(const std::string& s){
  std::vector<std::string> returnList;
  std::vector<std::string> temp = splitString(s,",");
  std::vector<std::string> temp2 = splitString(temp.front(), " ");

  for(int i=0; i<temp2.size(); i++){
    returnList.push_back(temp2[i]);
  }
  for(int i=1; i<temp.size(); i++){
    returnList.push_back(temp[i]);
  }
  return returnList;
}

bool checkSelect(const std::string& s){
  return s.find("Select") == 0 ;
}

bool checkSuchThat(const std::string& s){
  std::vector<std::string> relRefList {"Follows", "Follows*", "Parent", "Parent*", "Uses", "Modifies"};

  for(const auto& i: relRefList){
    if (s.find(i) == 0){
      return true;
    }
  }
  return false;
}

std::vector<std::string> extractVariablesClauses(const std::string& s){
  std::vector<std::string> returnList;
  std::vector<std::string> temp = splitString(s,"such that");
  for(const auto& i:temp){
    std::vector<std::string> temp2 = splitString(i, "pattern");
    for (const auto& j:temp2){
      returnList.push_back(j);
    }
  }
  return returnList;
}

std::vector<std::string> extractVariablesToSelect(const std::string& s){
  std::vector<std::string> returnList;
  std::vector<std::string> everythingList = extractVariablesClauses(s);

  for(const auto& stmt:everythingList){
    if (checkSelect(stmt)){
      returnList.push_back(stripString(stmt.substr(6)));
    }
  }
  return returnList;
}
std::vector<std::string> extractSuchThatClauses(const std::string& s){
  std::vector<std::string> returnList;
  std::vector<std::string> everythingList = extractVariablesClauses(s);

  for(const auto& stmt:everythingList){
    if (checkSuchThat(stmt)){
      std::vector<std::string> wordsList = splitString(stmt, " ");
      std::string relStmt;
      for (int i=0; i<wordsList.size(); i=i+3){
        relStmt = wordsList[i] + " " + wordsList[i+1] + " " + wordsList[i+2];
        returnList.push_back(relStmt);
      }
    }
  }
  return returnList;
}
std::vector<std::string> extractPatternClauses(const std::string& s){
  std::vector<std::string> returnList;
  std::vector<std::string> everythingList = extractVariablesClauses(s);

  for(const auto& stmt:everythingList){
    if ( (!checkSuchThat(stmt) && !checkSelect(stmt))){
      returnList.push_back(stmt);
    }
  }
  return returnList;
}

std::vector<std::string> extractItemsInBrackets(const std::string& s){
  std::vector<std::string> returnList;

  long openBracketPosition = s.find("(");
  long commaPosition = s.find(",");
  long closeBracketPosition = s.find(")");

  if (openBracketPosition < commaPosition && commaPosition < closeBracketPosition){
    returnList.push_back(stripString(s.substr(0, openBracketPosition)));
    returnList.push_back(stripString(s.substr(openBracketPosition+1, commaPosition - openBracketPosition - 1)));
    returnList.push_back(stripString(s.substr(commaPosition+1, closeBracketPosition - commaPosition - 1)));

  }
  return returnList;
}

bool checkRelRefArr(std::vector<std::string> sArr){
  if (sArr.size() != 3){
    return false;
  }else{
    if (sArr[0] == "Follows" || sArr[0] ==  "Follows*" || sArr[0] ==  "Parent" || sArr[0] == "Parent*"){
      return isStmtRef(sArr[1]) && isStmtRef(sArr[2]);
    }else{
      return (isEntRef(sArr[1]) && isEntRef(sArr[2])) || (isStmtRef(sArr[1]) && isEntRef(sArr[2]));
    }
  }
}