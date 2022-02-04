//
// Created by Juliet Teoh on 4/2/22.
//

#include "QueryProcessor.h"

std::vector<Clause> QueryProcessor::parsePQL(std::string parsePQL) {

  std::vector<std::string> selectStmtList = extractSelect(parsePQL);
  std::vector<std::string> declarationStmtList = extractDeclaration(parsePQL);




  return std::vector<Clause>();
}
