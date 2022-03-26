#include "QueryRegex.h"

bool firstWordChecker(const std::string& s, const std::string& targetWord) {
    int startPosition = 0;
    return s.find(targetWord) == startPosition;
}

bool isSpaces(const std::string& s) {
    return stripString(s).empty();
}

bool doesStringExist(const std::string& s, const std::string& substring) {
    return s.find(substring) != std::string::npos;
}

/**
 * @param s some string
 * @param entityMap entity Map to reference to
 * @return returns true if some string s is one of the keys of the entity map
 */
bool entityMapContains(const std::string& s, std::unordered_map<std::string, Entity>* entityMap) {
    return (*entityMap).find(s) != (*entityMap).end();
}

/**
 * @param s a string
 * @return returns true if string s is a IDENT. else return false
 */
bool isIdent(const std::string& s) {
    std::regex identRegex("[A-Za-z][A-Za-z0-9]*");
    return std::regex_match(s, identRegex);
}

/**
 * @param s a string
 * @return returns true if string s is an INTEGER. else return false
 */
bool isInteger(const std::string& s) {
    std::regex integerRegex("[1-9][0-9]*");
    return std::regex_match(s, integerRegex) || s == "0";
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
 * @return return true if string s is '"' IDENT '"'. else return false
 */
bool isQuotationIdent(const std::string& s) {
    std::string temp;
    if (firstWordChecker(extractFirstChar(s), "\"") && firstWordChecker(extractLastChar(s), "\"")) {
        temp = extractStringWithoutFirstAndLastChar(s);
        return isIdent(temp);
    }
    return false;
}

bool isStringWithinQuotations(const std::string& s) {
    std::string temp;
    return firstWordChecker(extractFirstChar(s), "\"") && firstWordChecker(extractLastChar(s), "\"");
}

/**
 * @param s a string
 * @return returns true if string s is '_' '"' expr '"' '_'. Else return false
 */
bool isStringWithinWildCard(const std::string& s) {
    std::string temp;
    if (firstWordChecker(extractFirstChar(s), "_") && firstWordChecker(extractLastChar(s), "_")) {
        temp = extractStringWithoutFirstAndLastChar(s);
        return firstWordChecker(extractFirstChar(temp), "\"") && firstWordChecker(extractLastChar(temp), "\"");
    }
    return false;
}

/**
 * @param s a string
 * @return return true if string s is a stmtRef. Else return false
 */
bool isStmtRef(const std::string& s) {
    return isIdent(s) || isWildCard(s) || isInteger(s);
}

/**
 * @param s a string
 * @return return true if string s is a entRef. Else return false
 */
bool isEntRef(const std::string& s) {
    return isIdent(s) || isWildCard(s) || isQuotationIdent(s);
}

/**
 * @param s some string
 * @return returns true if the word "Select" is at the start of a string
 */
bool isSelect(const std::string& s) {
    return firstWordChecker(s, "Select");
}

/**
 * @param s some string
 * @return returns true if the word "pattern" is at the start of a string
 */
bool isPattern(const std::string& s) {
    return firstWordChecker(s, "pattern");
}

bool isWith(const std::string& s) {
    return firstWordChecker(s, "with");
}

/**
 * @param s some string
 * @return returns true if "such" "that" exists in a string
 */
bool existSuchThat(const std::string& s) {
    std::vector<long> myList = findSuchThatClause(s);
    long suchPosition = myList[0];
    long thatPosition = myList[1];
    int suchLength = 4;
    if (suchPosition == std::string::npos || thatPosition == std::string::npos) {
        return false;
    } else {
        return thatPosition - suchPosition - suchLength > 0;
    }
}

/**
 * check if Entity Declaration Statements are valid
 * @param sArr List where the first element is a designEntity, and the rest are synonyms
 * @return returns true if the Entity Declaration STATEMENT is valid. else, return false
 */
bool checkDesignEntitySynonymsList(std::vector<std::string> sArr) {
    bool returnBool = true;
    std::vector<std::string>
            designEntity{"stmt", "read", "print", "call", "while", "if", "assign", "variable", "constant", "procedure"};
    if (sArr.size() <= 1) {
        return false;
    } else {
        std::string designStr = sArr[0];
        if (std::find(std::begin(designEntity), std::end(designEntity), designStr) != std::end(designEntity)) {
            //designStr contains one of the design entity keywords
            for (int i = 1; i < sArr.size(); i++) {
                returnBool = returnBool && isIdent(sArr[i]);
            }
            return returnBool;
        } else {
            return false;
        }
    }
}

/**
 * Check if list of rel ref relationship and entities is valid
 * @param sArr a list where the first element is a relRef and the rest are entity names
 * @return returns true if rel ref list is valid
 */
bool checkRelRefList(std::vector<std::string> sArr) {
    if (sArr.size() != 3) {
        return false;
    } else {
        if (sArr[0] == "Follows" || sArr[0] == "Follows*" || sArr[0] == "Parent" || sArr[0] == "Parent*"
                || sArr[0] == "Next" || sArr[0] == "Next*" || sArr[0] == "Affects" || sArr[0] == "Affects*") {
            return isStmtRef(sArr[1]) && isStmtRef(sArr[2]);
        } else if (sArr[0] == "Uses" || sArr[0] == "Modifies") {
            return (isEntRef(sArr[1]) && isEntRef(sArr[2])) || (isStmtRef(sArr[1]) && isEntRef(sArr[2]));
        } else if (sArr[0] == "Calls" || sArr[0] == "Calls*") {
            return (isEntRef(sArr[1]) && isEntRef(sArr[2]));
        } else {
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
bool checkPatternList(std::vector<std::string> patternList, std::unordered_map<std::string, Entity>* entityMap) {
    if (patternList.size() == 3 && isEntRef(patternList[1]) && entityMapContains(patternList[0], entityMap)) {
        Entity e = (*entityMap)[patternList[0]];
        return e.eType == EntityType::ASSIGNMENT || (e.eType == EntityType::WHILE && isWildCard(patternList[2]));
    } else if (patternList.size() == 4 && isEntRef(patternList[1]) && entityMapContains(patternList[0], entityMap)) {
        Entity e = (*entityMap)[patternList[0]];
        return e.eType == EntityType::IF && isWildCard(patternList[2]) && isWildCard(patternList[3]);
    }
    return false;
}

std::string extractFirstChar(const std::string& s) {
    return s.substr(0, 1);
}

std::string extractLastChar(const std::string& s) {
    return s.substr(s.length() - 1, 1);
}

std::string extractStringWithoutFirstAndLastChar(const std::string& s) {
    return stripString(s.substr(1, s.length() - 2));
}

std::string extractStringFromQuotation(const std::string& s) {
    std::string temp;
    temp = extractStringWithoutFirstAndLastChar(s);
    return temp;
}

std::string extractStringFromWildCard(const std::string& s) {
    std::string temp = extractStringWithoutFirstAndLastChar(s);
    temp = extractStringWithoutFirstAndLastChar(temp);
    return temp;
}

/**
 * @param s a string
 * @return remove whitespace from the front and the back of a string
 */
std::string stripString(std::string s) {
    const char* whitespace = " \t\n\r\f\v";
    s.erase(0, s.find_first_not_of(whitespace));
    s.erase(s.find_last_not_of(whitespace) + 1);
    return s;
}

std::string removeSelect(const std::string& s) {
    int selectLength = 6;
    return stripString(s.substr(selectLength, s.length() - selectLength));
}

std::string removeSuchThat(const std::string& s) {
    std::vector<long> myList = findSuchThatClause(s);
    long suchPosition = myList[0];
    long thatPosition = myList[1];
    long suchLength = 4;
    if (suchPosition != std::string::npos) {
        // remove such that
        return stripString(s.substr(0, suchPosition)
                                   + s.substr(thatPosition + suchLength, s.length() - thatPosition - suchLength));
    } else {
        return s;
    }
}

/**
 * Remove pattern word from front of the string if it exists
 * @param s some string
 * @return string without pattern in front
 */
std::string removePattern(const std::string& s) {
    int patternLength = 7;
    if (isPattern(s)) {
        return stripString(s.substr(patternLength, s.length() - patternLength));
    } else {
        return s;
    }
}

std::string removeWith(const std::string& s) {
    int withLength = 4;
    if (isWith(s)) {
        return stripString(s.substr(withLength, s.length() - withLength));
    } else {
        return s;
    }
}

long findClauseStartPosition(const std::string& s, const std::string& commandWord, const std::string& original) {
    long selectLength = 6;
    long commandWordLength = commandWord.length();
    long temp;
    std::string placeholder;
    long patternPosition = s.find(commandWord);

    if (patternPosition != std::string::npos) {
        if (doesStringExist(s.substr(patternPosition, s.length() - patternPosition), ">")
                || isSpaces(original.substr(selectLength, patternPosition - selectLength))) {
            temp = findClauseStartPosition(s.substr(patternPosition + commandWordLength, s.size() - patternPosition),
                                           commandWord,
                                           original);
            if (temp != std::string::npos) {
                patternPosition = temp + patternPosition + commandWordLength;
            } else {
                patternPosition = std::string::npos;
            }
        }
        return patternPosition;
    } else {
        return std::string::npos;
    }
}

/**
 * find first index of pattern clause
 * @param s some string
 * @return returns first index of pattern clause
 */
long findPatternClause(const std::string& s) {
    return findClauseStartPosition(s, "pattern", s);
}

long findWithClause(const std::string& s) {
    return findClauseStartPosition(s, "with", s);
}

long findPatternClauseInSubstring(const std::string& s, std::string original) {
    long commaPosition = original.rfind(',');
    if (commaPosition != std::string::npos) {
        original = original.substr(0, commaPosition);
    }
    long patternPosition = s.rfind("pattern");
    long bracketPosition = original.rfind("(");
    int patternLength = 7;

    if (patternPosition != std::string::npos) {
        if (bracketPosition > patternPosition && !isSpaces(original.substr(patternPosition + patternLength,
                                                                           bracketPosition - patternPosition
                                                                                   - patternLength))) {
            return patternPosition;
        } else {
            return findPatternClauseInSubstring(s.substr(0, patternPosition), original);
        }
    } else {
        return std::string::npos;
    }
}

/**
 * find first index of such that clause
 * @param s some string
 * @return returns first index of such that clause
 */
std::vector<long> findSuchThatClause(const std::string& s) {
    std::vector<long> returnList;
    long suchLength = 4;
    long temp;
    long suchPosition = s.find("such");
    long thatPosition = s.find("that");
    if ((suchPosition == std::string::npos || thatPosition == std::string::npos)) {
        returnList.push_back(std::string::npos);
        returnList.push_back(std::string::npos);
    } else if (stripString(s.substr(suchPosition + suchLength, thatPosition - suchPosition - suchLength)).empty()
            && suchPosition < thatPosition) {
        if (thatPosition - suchPosition == suchLength) {
            temp = thatPosition;
            returnList = findSuchThatClause(s.substr(temp + suchLength, s.size() - temp));
            returnList[0] = returnList[0] + temp + suchLength;
            returnList[1] = returnList[1] + temp + suchLength;
        } else {
            returnList.push_back(suchPosition);
            returnList.push_back(thatPosition);
        }
    } else {
        if (suchPosition < thatPosition) {
            temp = suchPosition;
        } else {
            temp = thatPosition;
        }
        returnList = findSuchThatClause(s.substr(temp + suchLength, s.size() - temp));
        returnList[0] = returnList[0] + temp + suchLength;
        returnList[1] = returnList[1] + temp + suchLength;
    }
    return returnList;
}

long smallestNumber(const long positionList[3]) {
    long toReturn = INT32_MAX;
    long temp;
    int positionListSize = 3;
    for (int i = 0; i < positionListSize; i++) {
        temp = positionList[i];
        if (std::string::npos != temp && temp < toReturn && temp >= 0) {
            toReturn = temp;
        }
    }
    return toReturn;
}

/**
 * @param s a string to split
 * @param delimiter delimiter string to split the string by
 * @return a list of substring, split by the delimiter
 */
std::vector<std::string> splitString(const std::string& s, const std::string& delimiter) {
    long startPosition = 0;
    long endPosition;
    long delimiterLength = delimiter.length();
    std::string placeholder;
    std::vector<std::string> strList;
    endPosition = s.find(delimiter, startPosition);
    while (endPosition != std::string::npos) {
        placeholder = s.substr(startPosition, endPosition - startPosition);
        startPosition = endPosition + delimiterLength;
        strList.push_back(stripString(placeholder));
        endPosition = s.find(delimiter, startPosition);
    }
    placeholder = stripString(s.substr(startPosition));
    if (!isSpaces(placeholder)) {
        strList.push_back(placeholder);
    }
    return strList;
}

/**
 * split string by multiple space delimiters
 * @param s some string
 * @return list of words with spaces removed
 */
std::vector<std::string> splitStringBySpaces(const std::string& s) {
    std::vector<std::string> toReturn;
    std::regex re("[ \t\n\r\f\v]");
    std::sregex_token_iterator first{s.begin(), s.end(), re, -1}, last;
    std::vector<std::string> tokens{first, last};
    std::string temp;
    for (const auto& word : tokens) {
        temp = stripString(word);
        if (!temp.empty()) {
            toReturn.push_back(temp);
        }
    }
    return toReturn;
}

bool existVBrackets(const std::string& s) {
    return extractFirstChar(s) == "<" && extractLastChar(s) == ">";
}

std::string removeVBrackets(const std::string& s) {
    return extractStringWithoutFirstAndLastChar(s);
}

bool checkAndCommandWord(const std::vector<std::string>& clausesList,
                         bool (* existCommandWordFunction)(const std::string& s)) {
    bool returnBool = true;
    for (auto& i : clausesList) {
        returnBool = returnBool && firstWordChecker(i, "and") || existCommandWordFunction(i);
    }
    return returnBool;
}

void removeAndCommandWord(std::vector<std::string>* clausesList,
                          std::string (* removeCommandWordFunction)(const std::string& s)) {
    int andLength = 3;
    for (auto& i : *clausesList) {
        if (firstWordChecker(i, "and")) {
            i = stripString(i.substr(andLength, i.length() - andLength));
        } else {
            i = removeCommandWordFunction(i);
        }
    }
}

bool checkAndSuchThat(const std::vector<std::string>& clausesList) {
    return checkAndCommandWord(clausesList, existSuchThat);
}

void removeAndSuchThat(std::vector<std::string>* clausesList) {
    removeAndCommandWord(clausesList, removeSuchThat);
}

bool checkAndPattern(const std::vector<std::string>& clausesList) {
    return checkAndCommandWord(clausesList, isPattern);
}

void removeAndPattern(std::vector<std::string>* clausesList) {
    removeAndCommandWord(clausesList, removePattern);
}

bool checkAndWith(const std::vector<std::string>& clausesList) {
    return checkAndCommandWord(clausesList, isWith);
}

void removeAndWith(std::vector<std::string>* clausesList) {
    removeAndCommandWord(clausesList, removeWith);
}

/**
 * Split entity declarations from select clause
 * @param s query string
 * @return List of declarations and select clauses, seperated
 */
std::vector<std::string> splitDeclarationAndSelect(const std::string& s) {
    return splitString(s, ";");
}

std::vector<std::vector<std::string>> extractSelectAndDeclarationStatements(const std::string& s) {
    std::vector<std::vector<std::string>> returnList;
    std::vector<std::string> selectList;
    std::vector<std::string> declarationList;
    std::vector<std::string> stmtList = splitDeclarationAndSelect(s);
    for (const std::string& stmt : stmtList) {
        if (isSelect(stmt)) {
            // select exists as the first word in the statement
            selectList.push_back(stmt);
        } else {
            declarationList.push_back(stmt);
        }
    }
    returnList.push_back(selectList);
    returnList.push_back(declarationList);
    return returnList;
}

/**
 * Seperates Design Entity and Synonyms and put them in a list
 * @param s Entity declarations statement
 * @return List where the first element is a Design Entity and the rest are Synonyms
 */
std::vector<std::string> extractDesignEntityAndSynonyms(const std::string& s) {
    std::vector<std::string> returnList;
    std::vector<std::string> laterSynonymsList = splitString(s, ",");
    std::vector<std::string> frontDesignEntityAndSynonym = splitStringBySpaces(laterSynonymsList.front());
    for (auto& i : frontDesignEntityAndSynonym) {
        if (!isSpaces(i)) {
            returnList.push_back(i);
        }
    }

    if (returnList.size() != 2) {
        std::vector<std::string> emptyList;
        return emptyList;
    }

    for (int i = 1; i < laterSynonymsList.size(); i++) {
        if (!isSpaces(laterSynonymsList[i])) {
            returnList.push_back(stripString(laterSynonymsList[i]));
        }
    }
    return returnList;
}

/**
 * Split the selected variable and clauses into a list
 * @param s some string to split
 * @return a list containing the variable to select and clauses
 */
std::vector<std::string> splitVariablesAndClauses(const std::string& s) {
    std::vector<std::string> returnList;
    std::string varString;
    std::string clauseString;
    std::string placeholder;
    long suchThatPosition = findSuchThatClause(s)[0];
    long patternPosition = findPatternClause(s);
    long withPosition = findWithClause(s);

    long positionList[3] = {suchThatPosition, patternPosition, withPosition};
    long selected = smallestNumber(positionList);

    if ((suchThatPosition == std::string::npos) && (patternPosition == std::string::npos)
            && (withPosition == std::string::npos)) {
        returnList.push_back(s);
        returnList.emplace_back("");
        return returnList;
    } else if (selected == suchThatPosition) {
        varString = s.substr(0, suchThatPosition);
        clauseString = s.substr(suchThatPosition, s.length() - suchThatPosition);
    } else if (selected == patternPosition) {
        varString = s.substr(0, patternPosition);
        clauseString = s.substr(patternPosition, s.length() - patternPosition);
    } else {
        varString = s.substr(0, withPosition);
        clauseString = s.substr(withPosition, s.length() - withPosition);
    }
    returnList.push_back(varString);
    returnList.push_back(clauseString);
    return returnList;
}

std::vector<std::string> extractWithClauses(const std::string& s) {
    std::vector<std::string> toReturn;
    std::string temp = std::regex_replace(s, std::regex("="), " ");
    std::vector<std::string> tempList = splitStringBySpaces(temp);
    std::vector<std::string> tokenList;

    std::string tempString;
    bool afterDot = false;
    int i = 0;
    int j = 0;

    while (i < tempList.size()) {
        if (tempList[i] == ".") {
            afterDot = true;
        } else if (afterDot) {
            tokenList[j - 1] = tokenList[j - 1] + "." + tempList[i];
            afterDot = false;
        } else {
            tokenList.push_back(tempList[i]);
            j = j + 1;
        }
        i = i + 1;
    }

    while (tokenList.size() % 3 != 0) {
        tokenList.emplace_back("");
    }

    for (int q = 0; q < tokenList.size(); q = q + 3) {
        temp = tokenList[q] + " " + tokenList[q + 1] + " " + tokenList[q + 2];
        toReturn.push_back(temp);
    }
    return toReturn;
}

/**
 * split clauses into pattern and such that clauses
 * @param s some string to be split
 * @return a list of clauses, either pattern or such that
 */
std::vector<std::string> splitSuchThatPatternWithClauses(const std::string& s) {
    std::vector<std::string> returnList;
    std::vector<std::string> temp;
    std::vector<std::string> tempList = splitString(s, ")"); //tempList = splitString(s, ")");
    std::vector<std::string> wordsList;

    int count = 0;
    for (const auto& stmt : tempList) {
        if (!stmt.empty()) {
            if (firstWordChecker(stmt, "and") || isWith(stmt) ||
                    firstWordChecker(stmt, "such") || isPattern(stmt)) {
                wordsList.push_back(stmt);
                count = count + 1;
            } else {
                wordsList[count - 1] = wordsList[count - 1] + ")" + stmt;
            }
        } else {
            wordsList[count - 1] = wordsList[count - 1] + ")";
        }
    }

    for (const auto& stmt : wordsList) {
        if (!stmt.empty()) {
            if (!isWith(stmt)) {
                returnList.push_back(stmt + ")");
            } else if (isWith(stmt) && existSuchThat(stmt)) {
                long x = findSuchThatClause(stmt)[0];
                std::string first = stmt.substr(0, x);
                std::string second = stmt.substr(x, stmt.length() - x);
                temp = extractWithClauses(first);
                returnList.insert(returnList.end(), temp.begin(), temp.end());
                returnList.push_back(second + ")");
            } else if (isWith(stmt) && findPatternClauseInSubstring(stmt, stmt) != std::string::npos) {
                long x = findPatternClauseInSubstring(stmt, stmt);
                std::string first = stmt.substr(0, x);
                std::string second = stmt.substr(x, stmt.length() - x);
                temp = extractWithClauses(first);
                returnList.insert(returnList.end(), temp.begin(), temp.end());
                returnList.push_back(second + ")");
            } else {
                temp = extractWithClauses(stmt);
                returnList.insert(returnList.end(), temp.begin(), temp.end());
            }
        }
    }
    return returnList;
}

/**
 * Extract the variable to select
 * @param s some string
 * @return list of the variable to select
 */
std::vector<std::string> extractVariablesToSelect(const std::string& s) {
    std::vector<std::string> returnList;
    std::string variableString = splitVariablesAndClauses(s)[0];

    if (isSelect(variableString)) {
        variableString = removeSelect(variableString);
        if (existVBrackets(variableString)) {
            variableString = removeVBrackets(variableString);
            for (const auto& word : splitString(variableString, ",")) {
                returnList.push_back(word);
            }
        } else {
            returnList.push_back(variableString);
        }
    }
    return returnList;
}

std::vector<std::vector<std::string>> extractClauses(const std::string& s) {
    enum class lastClauseType {
        None,
        SuchThat,
        Pattern,
        With
    };

    std::vector<std::vector<std::string>> returnList;
    std::vector<std::string> suchThatList;
    std::vector<std::string> patternList;
    std::vector<std::string> withList;
    std::vector<std::string> everythingList = splitVariablesAndClauses(s);

    if (everythingList.size() == 2) {
        std::string clausesString = everythingList[1];
        std::vector<std::string> clausesList = splitSuchThatPatternWithClauses(clausesString);
        lastClauseType lastType = lastClauseType::None;

        for (const auto& stmt : clausesList) {
            if (isWith(stmt)) {
                withList.push_back(stmt);
                lastType = lastClauseType::With;
            } else if (isPattern(stmt)) {
                patternList.push_back(stmt);
                lastType = lastClauseType::Pattern;
            } else if (existSuchThat(stmt)) {
                suchThatList.push_back(stmt);
                lastType = lastClauseType::SuchThat;
            } else {
                if (lastType == lastClauseType::Pattern) {
                    patternList.push_back(stmt);
                } else if (lastType == lastClauseType::SuchThat) {
                    suchThatList.push_back(stmt);
                } else if (lastType == lastClauseType::With) {
                    withList.push_back(stmt);
                } else {
                    // just push it to such that so that it will be invalid later
                    suchThatList.push_back(stmt);
                }
            }
        }
    }
    returnList.push_back(suchThatList);
    returnList.push_back(patternList);
    returnList.push_back(withList);
    return returnList;
}

/**
 * Extract the statements in front and within brackets
 * @param s some string
 * @return list of statements
 */
std::vector<std::string> extractItemsInBrackets(const std::string& s) {
    std::vector<std::string> returnList;

    long openBracketPosition = s.find('(');
    long commaPosition = s.find(',');
    long closeBracketPosition = s.find(')');

    if (openBracketPosition < commaPosition && commaPosition < closeBracketPosition) {
        returnList.push_back(stripString(s.substr(0, openBracketPosition)));
        returnList.push_back(stripString(s.substr(openBracketPosition + 1, commaPosition - openBracketPosition - 1)));
        returnList.push_back(stripString(s.substr(commaPosition + 1, closeBracketPosition - commaPosition - 1)));

        if (s.size() - 1 > closeBracketPosition && !isSpaces(s.substr(closeBracketPosition + 1))) {
            returnList.push_back(stripString(s.substr(closeBracketPosition + 1)));
        }

    }
    return returnList;
}

std::vector<std::string> extractPatternBrackets(const std::string& s) {
    std::vector<std::string> returnList;
    long openBracketPosition = s.find('(');
    long closeBracketPosition = s.rfind(')');

    if (openBracketPosition != std::string::npos && closeBracketPosition != std::string::npos) {
        returnList.push_back(stripString(s.substr(0, openBracketPosition)));
        std::string temp = extractStringWithoutFirstAndLastChar(s.substr(openBracketPosition, closeBracketPosition));
        std::vector<std::string> tempArr = splitString(temp, ",");
        returnList.insert(std::end(returnList), std::begin(tempArr), std::end(tempArr));
    }
    return returnList;
}

bool checkRelationshipRef(const RelationshipRef& r) {
    if (r.rType == RelationshipType::NULL_RELATIONSHIP) {
        return false;
    } else {
        bool returnBool;
        if (r.rType == RelationshipType::FOLLOWS || r.rType == RelationshipType::FOLLOWS_T ||
                r.rType == RelationshipType::PARENT || r.rType == RelationshipType::PARENT_T ||
                r.rType == RelationshipType::NEXT || r.rType == RelationshipType::NEXT_T) {
            returnBool = checkFollowsOrParentsOrNext(r);
        }else if(r.rType == RelationshipType::AFFECTS || r.rType == RelationshipType::AFFECTS_T){
            returnBool = checkAffects(r);
        } else if (r.rType == RelationshipType::USES) {
            returnBool = checkUses(r);
        } else if (r.rType == RelationshipType::MODIFIES) {
            returnBool = checkModifies(r);
        } else if (r.rType == RelationshipType::CALLS || r.rType == RelationshipType::CALLS_T) {
            returnBool = checkCalls(r);
        } else if (r.rType == RelationshipType::PATTERN) {
            returnBool = checkPattern(r);
        } else if (r.rType == RelationshipType::WITH) {
            returnBool = checkWith(r);
        } else {
            returnBool = false;
        }
        return returnBool;
    }
}

bool checkEntityIsStmtRef(const Entity& e) {
    return e.eType == EntityType::FIXED_INTEGER
            || e.eType == EntityType::STATEMENT
            || e.eType == EntityType::ASSIGNMENT
            || e.eType == EntityType::WHILE
            || e.eType == EntityType::IF
            || e.eType == EntityType::CALL
            || e.eType == EntityType::PRINT
            || e.eType == EntityType::READ
            || e.eType == EntityType::WILDCARD;
}

bool checkVariable(const Entity& e){
    return e.eType == EntityType::FIXED_STRING
            || e.eType == EntityType::VARIABLE
            || e.eType == EntityType::WILDCARD;
}

bool checkUsesLeftSide(const Entity& e) {
    return e.eType == EntityType::FIXED_INTEGER
            || e.eType == EntityType::FIXED_STRING
            || e.eType == EntityType::STATEMENT
            || e.eType == EntityType::ASSIGNMENT
            || e.eType == EntityType::PRINT
            || e.eType == EntityType::WHILE
            || e.eType == EntityType::IF
            || e.eType == EntityType::PROCEDURE
            || e.eType == EntityType::CALL
            || e.eType == EntityType::WILDCARD;
}

bool checkModifiesLeftSide(const Entity& e) {
    return e.eType == EntityType::FIXED_INTEGER
            || e.eType == EntityType::FIXED_STRING
            || e.eType == EntityType::STATEMENT
            || e.eType == EntityType::ASSIGNMENT
            || e.eType == EntityType::READ
            || e.eType == EntityType::WHILE
            || e.eType == EntityType::IF
            || e.eType == EntityType::PROCEDURE
            || e.eType == EntityType::CALL
            || e.eType == EntityType::WILDCARD;
}

bool checkCallsEntity(const Entity& e) {
    return e.eType == EntityType::FIXED_STRING
            || e.eType == EntityType::PROCEDURE
            || e.eType == EntityType::WILDCARD;
}

bool checkAssignments(const Entity& e){
    return e.eType == EntityType::FIXED_INTEGER
        || e.eType == EntityType::WILDCARD
        || e.eType == EntityType::ASSIGNMENT;
}

bool checkFollowsOrParentsOrNext(const RelationshipRef& r) {
    bool leftSideBool = checkEntityIsStmtRef(r.leftEntity);
    bool rightSideBool = checkEntityIsStmtRef(r.rightEntity);
    return leftSideBool && rightSideBool;
}

bool checkUses(const RelationshipRef& r) {
    bool leftSideBool = checkUsesLeftSide(r.leftEntity);
    bool rightSideBool = checkVariable(r.rightEntity) ;
    return leftSideBool && rightSideBool;
}

bool checkModifies(const RelationshipRef& r) {
    bool leftSideBool = checkModifiesLeftSide(r.leftEntity);
    bool rightSideBool = checkVariable(r.rightEntity);
    return leftSideBool && rightSideBool;
}

bool checkCalls(const RelationshipRef& r) {
    bool leftSideBool = checkCallsEntity(r.leftEntity);
    bool rightSideBool = checkCallsEntity(r.rightEntity);
    return leftSideBool && rightSideBool;
}

bool checkAffects(const RelationshipRef& r){
    bool leftSideBool = checkAssignments(r.leftEntity);
    bool rightSideBool = checkAssignments(r.rightEntity);
    return leftSideBool && rightSideBool;
}

bool checkPattern(const RelationshipRef& r) {
    if (r.AssignmentEntity.eType == EntityType::ASSIGNMENT) {
        bool leftSideBool = checkVariable(r.leftEntity);
        bool rightSideBool = r.rightEntity.eType == EntityType::WILDCARD ||
                r.rightEntity.eType == EntityType::FIXED_STRING_WITHIN_WILDCARD ||
                r.rightEntity.eType == EntityType::FIXED_STRING;
        return leftSideBool && rightSideBool;
    } else if (r.AssignmentEntity.eType == EntityType::WHILE || r.AssignmentEntity.eType == EntityType::IF) {
        bool leftSideBool = checkVariable(r.leftEntity);
        bool rightSideBool = r.rightEntity.eType == EntityType::WILDCARD;
        return leftSideBool && rightSideBool;
    } else {
        return false;
    }
}

bool checkWith(const RelationshipRef& r) {
    return checkWithEntity(r.leftEntity) && checkWithEntity(r.rightEntity);
}

bool checkWithEntity(const Entity& e) {
    if (e.aType == EntityAttributeType::PROCNAME){
        return e.eType == EntityType::PROCEDURE || e.eType == EntityType::CALL;
    }else if (e.aType == EntityAttributeType::VARNAME){
        return e.eType == EntityType::VARIABLE || e.eType == EntityType::READ || e.eType == EntityType::PRINT;
    }else if (e.aType == EntityAttributeType::VALUE){
        return e.eType == EntityType::CONSTANT;
    }else if (e.aType == EntityAttributeType::STMT) {
        return e.eType == EntityType::STATEMENT || e.eType == EntityType::READ || e.eType == EntityType::PRINT ||
                e.eType == EntityType::CALL || e.eType == EntityType::WHILE || e.eType == EntityType::IF ||
                e.eType == EntityType::ASSIGNMENT;
    }else{
        return e.eType == EntityType::FIXED_STRING || e.eType == EntityType::FIXED_INTEGER;
    }
}

bool checkVariableToSelect(const Entity& e) {
    if (e.eType == EntityType::WILDCARD
            || e.eType == EntityType::FIXED_INTEGER
            || e.eType == EntityType::FIXED_STRING
            || e.eType == EntityType::FIXED_STRING_WITHIN_WILDCARD
            || e.eType == EntityType::NULL_ENTITY) {
        return false;
    } else {
        if (e.eType == EntityType::BOOLEAN) {
            return e.aType == EntityAttributeType::NULL_ATTRIBUTE;
        }
        return true;
    }
}

bool checkAlreadyInClause(const std::vector<RelationshipRef>& relationshipList, RelationshipRef newRelationship){
    bool toReturn = false;
    for (const RelationshipRef& rel: relationshipList){
        toReturn = toReturn || newRelationship.equals(rel);
    }
    return toReturn;
}