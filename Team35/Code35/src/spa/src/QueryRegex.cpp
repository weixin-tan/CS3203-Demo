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

std::string stripString(std::string s){
  const char* whitespace = " \t\n\r\f\v";
  s.erase(0, s.find_first_not_of(whitespace));
  s.erase(s.find_last_not_of(whitespace) + 1);

  return s;
}

std::vector<std::string> splitString(std::string s, std::string delimiter){
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
  };
  strList.push_back(stripString(s.substr(startPosition)));
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

bool checkDesignEntitySynonyms(std::vector<std::string> sArr){
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

std::vector<std::string> extractDesignEntityandSynonym(std::string s){
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

bool checkSelect(std::string s){
  return s.find("Select") == 0 ;
}

bool checkSuchThat(std::string s){
  std::vector<std::string> relRefList {"Follows", "Follows*", "Parent", "Parent*", "Uses", "Modifies"};

  for(auto i: relRefList){
    if (s.find(i) == 0){
      return true;
    }
  }
  return false;
}

std::vector<std::string> extractVariablesClauses(std::string s){
  std::vector<std::string> returnList;
  std::vector<std::string> temp = splitString(s,"such that");

  for(auto i:temp){
    std::vector<std::string> temp2 = splitString(i, "pattern");
    for (auto j:temp2){
      returnList.push_back(j);
    }
  }
  return returnList;
}

std::vector<std::string> extractVariablesToSelect(std::string s){
  std::vector<std::string> returnList;
  std::vector<std::string> everythingList = extractVariablesClauses(s);

  for(auto stmt:everythingList){
    if (checkSelect(stmt)){
      returnList.push_back(stripString(stmt.substr(6)));
    }
  }
  return returnList;
}
std::vector<std::string> extractSuchThatClauses(std::string s){
  std::vector<std::string> returnList;
  std::vector<std::string> everythingList = extractVariablesClauses(s);

  for(auto stmt:everythingList){
    if (checkSuchThat(stmt)){
      returnList.push_back(stmt);
    }
  }
  return returnList;
}
std::vector<std::string> extractPatternCaluses(std::string s){
  std::vector<std::string> returnList;
  std::vector<std::string> everythingList = extractVariablesClauses(s);

  for(auto stmt:everythingList){
    if ( (!checkSuchThat(stmt) && !checkSelect(stmt))){
      returnList.push_back(stmt);
    }
  }
  return returnList;
}

std::vector<std::string> extractItemsInBrackets(std::string s){
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
