#include "QueryRegex.h"

/**
 * @param s a string
 * @return returns true if string s is a IDENT. else return false
 */
bool isIdent(const std::string& s){
  std::regex identRegex("[A-Za-z][A-Za-z0-9]*");
  return std::regex_match(s, identRegex);
}

/**
 * @param s a string
 * @return returns true if string s is an INTEGER. else return false
 */
bool isInteger(const std::string& s){
  std::regex integerRegex("[0-9]+");
  return std::regex_match(s, integerRegex);
}

/**
 * @param s a string
 * @return returns true if string s is an WILDCARD. else return false
 */
bool isWildCard(const std::string& s) {
  return s == "_";
}

/**
 * @param s a string
 * @return return true if string s is '"' IDENT '"'. ekse return false
 */
bool isQuotationIdent(const std::string& s){
  std::regex quotationIdentRegex("\"[A-Za-z][A-Za-z0-9]*\"");
  return std::regex_match(s, quotationIdentRegex);
}

/**
 * @param s a string
 * @return returns true if string s is '_' '"' expr '"' '_'. Else return false
 */
bool isStringWithinWildCard(const std::string& s){
  return s.substr(0,2) == "_\"" && s.substr(s.length()-2, 2) == "\"_";
}

/**
 * @param s a string
 * @return return true if string s is a stmtRef. Else return false
 */
bool isStmtRef(const std::string& s){
  return isIdent(s) || isWildCard(s) || isInteger(s) ;
}

/**
 * @param s a string
 * @return return true if string s is a entRef. Else return false
 */
bool isEntRef(const std::string& s){
  return isIdent(s) || isWildCard(s) || isQuotationIdent(s);
}

/**
 * @param s a string
 * @return remove whitespace from the front and the back of a string
 */
std::string stripString(std::string s){
  const char* whitespace = " \t\n\r\f\v";
  s.erase(0, s.find_first_not_of(whitespace));
  s.erase(s.find_last_not_of(whitespace) + 1);
  return s;
}

/**
 * @param s a string to split
 * @param delimiter delimiter string to split the string by
 * @return a list of substring, splitted by the delimiter
 */
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

/**
 * Split entity declarations from select clause
 * @param s query string
 * @return List of declarations and select clauses, seperated
 */
std::vector<std::string> splitDeclarationAndSelect(const std::string& s){
  return splitString(s, ";");
}

/**
 * Extract Select Clauses from query string
 * @param s query string
 * @return List of Select Clauses
 */
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

/**
 * Extract Entity Declarations from query string
 * @param s query string
 * @return List of Entity Declaration Statements
 */
std::vector<std::string> extractDeclaration(const std::string& s){
  std::vector<std::string> returnList;
  std::vector<std::string> stmtList = splitDeclarationAndSelect(s);
  for (const std::string& stmt: stmtList){
    if (stmt.find("Select") != 0){
      // select does not exist in the statement
      returnList.push_back(stmt);
    }
  }
  return returnList;
}

/**
 * check if Entity Declaration Statements are valid
 * @param sArr List where the first element is a designEntity, and the rest are synonyms
 * @return returns true if the Entity Declaration Statement is valid. else, return false
 */
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

/**
 * split string by multiple space delimiters
 * @param s some string
 * @return list of words with spaces removed
 */
std::vector<std::string> splitStringBySpaces(const std::string& s){
  std::vector<std::string> toReturn;
  std::regex re("[ \t\n\r\f\v]");
  std::sregex_token_iterator first{s.begin(), s.end(), re, -1}, last;
  std::vector<std::string> tokens{first, last};
  std::string temp;
  for (auto word: tokens) {
    temp = stripString(word);
    if (!temp.empty()) {
      toReturn.push_back(temp);
    }
  }
  return toReturn;
}

/**
 * Seperate Design Entity and Synonyms and put them in a list
 * @param s Entity declarations statement
 * @return List where the first element is a Design Entity and the rest are Synonyms
 */
std::vector<std::string> extractDesignEntityAndSynonyms(const std::string& s){
  std::vector<std::string> returnList;
  std::vector<std::string> laterSynonymsList = splitString(s, ",");
  std::vector<std::string> frontDesignEntityAndSynonym = splitStringBySpaces(laterSynonymsList.front());
  std::string tempString;
  for(auto & i : frontDesignEntityAndSynonym){
    tempString = stripString(i);
    if (!tempString.empty()){
      returnList.push_back(i);
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

/**
 * @param s some string
 * @return returns true if the word "Select" is at the start of a string
 */
bool isSelect(const std::string& s){
  return s.find("Select") == 0 ;
}

/**
 * @param s some string
 * @return returns true if the word "pattern" is at the start of a string
 */
bool isPattern(const std::string& s){
  return s.find("pattern") == 0;
}

/**
 * @param s some string
 * @return returns true if "such" "that" exists in a string
 */
bool existSuchThat(const std::string &s) {
  std::vector<long> mylist = findSuchThatClause(s);
  long suchPosition = mylist[0];
  long thatPosition = mylist[1];
  if ((suchPosition == std::string::npos || thatPosition == std::string::npos)){
    return false;
  }else{
    return thatPosition - suchPosition - 4 > 0;
  }
}

/**
 * find first index of pattern clause
 * @param s some string
 * @return returns first index of pattern clause
 */
long findPatternClause(const std::string& s){
  long selectLength = 6;
  long patternLength = 7;
  long temp;
  std::string placeholder;
  long patternPosition = s.find("pattern");

  if (stripString(s.substr(selectLength, patternPosition-selectLength)).empty()){
    //find a new pattern
    placeholder = s.substr (patternPosition+patternLength, s.size() - patternPosition);
    temp = placeholder.find("pattern");
    if (temp != std::string::npos){
      patternPosition = temp + patternPosition + patternLength;
    }
  }
  return patternPosition;
}

/**
 * find first index of such that clause
 * @param s some string
 * @return returns first index of such that clause
 */
std::vector<long> findSuchThatClause(const std::string& s){
  std::vector<long> returnList;
  long suchLength = 4;
  long temp;
  long suchPosition = s.find("such");
  long thatPosition = s.find("that");
  if ((suchPosition == std::string::npos || thatPosition == std::string::npos)){
    returnList.push_back(std::string::npos);
    returnList.push_back(std::string::npos);
  }else if (stripString(s.substr(suchPosition+4, thatPosition - suchPosition - 4)).empty() &&  suchPosition < thatPosition){
    if (thatPosition - suchPosition == 4){
      temp = thatPosition;
      returnList = findSuchThatClause(s.substr(temp+suchLength, s.size() - temp));
      returnList[0] = returnList[0] + temp + suchLength;
      returnList[1] = returnList[1] + temp + suchLength;
    }else{
      returnList.push_back(suchPosition);
      returnList.push_back(thatPosition);
    }
  }else{
    if (suchPosition < thatPosition){
      temp = suchPosition;
    }else{
      temp = thatPosition;
    }
    returnList = findSuchThatClause(s.substr(temp+suchLength, s.size() - temp));
    returnList[0] = returnList[0] + temp + suchLength;
    returnList[1] = returnList[1] + temp + suchLength;
  }
  return returnList;
}

/**
 * Split the selected variable and clauses into a list
 * @param s some string to split
 * @return a list containing the variable to select and clauses
 */
std::vector<std::string> splitVariablesAndClauses(const std::string& s){
  std::vector<std::string> returnList;
  std::string varString;
  std::string clauseString;
  std::string placeholder;
  long suchThatPosition = findSuchThatClause(s)[0];
  long patternPosition = findPatternClause(s);

  if ((suchThatPosition == std::string::npos) && (patternPosition == std::string::npos)){
    returnList.push_back(s);
  }else if (suchThatPosition == std::string::npos){
    // pattern
    varString = s.substr(0, patternPosition);
    clauseString = s.substr(patternPosition, s.length() - patternPosition);
  }else if (patternPosition == std::string::npos){
    varString = s.substr(0, suchThatPosition);
    clauseString = s.substr(suchThatPosition, s.length() - suchThatPosition);
  }else if (patternPosition < suchThatPosition){
    varString = s.substr(0, patternPosition);
    clauseString = s.substr(patternPosition, s.length() - patternPosition);
  }else{
    varString = s.substr(0, suchThatPosition);
    clauseString = s.substr(suchThatPosition, s.length() - suchThatPosition);
  }
  returnList.push_back(varString);
  returnList.push_back(clauseString);
  return returnList;
}

/**
 * split clauses into pattern and such that clauses
 * @param s some string to be split
 * @return a list of clauses, either pattern or such that
 */
std::vector<std::string> splitPatternAndSuchThatClauses(std::string s){
  std::vector<std::string> returnList;
  std::vector<long> mylist = findSuchThatClause(s);
  long suchPosition = mylist[0];
  long thatPosition = mylist[1];
  if (suchPosition != std::string::npos){
    // remove such that
    s = s.substr(0, suchPosition) + s.substr(thatPosition+4, s.length() - thatPosition-4);
  }
  std::vector<std::string> wordsList = splitString(s, ")");
  for (int i=0; i<wordsList.size(); i=i+1){
    if (!wordsList[i].empty()){
      returnList.push_back(wordsList[i] + ")");
    }
  }
  return returnList;
}

/**
 * Extract the variable to select
 * @param s some string
 * @return list of the variable to select
 */
std::vector<std::string> extractVariablesToSelect(const std::string& s){
  std::vector<std::string> returnList;
  std::string variableString = splitVariablesAndClauses(s)[0];
  if (isSelect(variableString)){
    returnList.push_back(stripString(variableString.substr(6,variableString.length()-6)));
  }
  return returnList;
}

/**
 * Extract such that clauses
 * @param s some string
 * @return a list of such that clauses
 */
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

/**
 * Extract pattern clauses
 * @param s
 * @return
 */
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

/**
 * Extract the statements in front and within brackets
 * @param s some string
 * @return list of statements
 */
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

/**
 * Check if list of rel ref relationship and entities is valid
 * @param sArr a list where the first element is a relRef and the rest are entity names
 * @return returns true if rel ref list is valid
 */
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

/**
 * check if a pattern list is valid of not
 * @param patternList a list where the first element is a assignment synonyms and the rest are synonyms
 * @param entityMap entity map to reference to
 * @return
 */
bool checkPatternList(std::vector<std::string> patternList, std::unordered_map<std::string, Entity>* entityMap){
  if (patternList.size() == 3 && isEntRef(patternList[1]) && entityMapContains(patternList[0], entityMap)){
    return true;
  }
  return false;
}

/**
 * Remove pattern word from front of the string if it exists
 * @param s some string
 * @return string without pattern in front
 */
std::string removePattern(const std::string& s){
  if (s.find("pattern") == 0){
    return stripString(s.substr(7,s.length()));
  }
  return s;
}

/**
 * @param s some string
 * @param entityMap entity Map to reference to
 * @return returns true if some string s is one of the keys of the entity map
 */
bool entityMapContains(const std::string& s, std::unordered_map<std::string, Entity>* entityMap){
  return (*entityMap).find(s) != (*entityMap).end();
}
