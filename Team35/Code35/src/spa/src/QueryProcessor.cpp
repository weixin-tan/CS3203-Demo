#include "QueryProcessor.h"

QueryProcessor::QueryProcessor(){ }

std::vector<Clause> QueryProcessor::parsePQL(std::string parsePQL) {
  bool isValid = true;
  std::vector<Clause> clauseList;
  std::vector<std::string> declarationStmtList = extractDeclaration(parsePQL);
  std::vector<std::string> selectStmtList = extractSelect(parsePQL);

  for (auto declarationStmt: declarationStmtList){
    std::vector<std::string> designEntityArr = extractDesignEntityandSynonym(declarationStmt);

    if (!checkDesignEntitySynonyms(designEntityArr)){
      isValid = false;
    }else{
      // create declaration objects and insert them into an unordered map
    }
  }

  if (isValid){
    for (auto selectStmt: selectStmtList){
      std::vector<std::string> variablesToSelect = extractVariablesToSelect(selectStmt);
      std::vector<std::string> SuchThatClauses = extractSuchThatClauses(selectStmt);
      std::vector<std::string> PatternClauses = extractPatternCaluses(selectStmt);

      //build clause object
      for (auto s: variablesToSelect){
        isValid = isValid && isIdent(s);

        if (isValid){
          //insert into variables to find into clause object
        }
      }

      for (auto s: SuchThatClauses){
        std::vector<std::string> relRefList = extractItemsInBrackets(s);
        isValid = isValid && checkRelRefArr(relRefList);
        if (isValid){
          //create relationship object to insert into clause object
        }
      }
      /*
      FOR NEXT WEEK LOL
      for (auto s: PatternClauses){
        //validate pattern
        //create relationship object
      }
      */
    }
  }

  return clauseList;
}
