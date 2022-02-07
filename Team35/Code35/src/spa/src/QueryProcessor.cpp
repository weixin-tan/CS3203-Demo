#include "QueryProcessor.h"

QueryProcessor::QueryProcessor()= default;

std::unordered_map<std::string,Entity> QueryProcessor::createDeclarationObjects(std::vector<std::string> designEntityArr, std::unordered_map<std::string, Entity> entityMap){
  std::vector<Entity> returnList;
  std::string designEntity = designEntityArr[0];
  EntityType eType;
  if (designEntity == "stmt"){
    eType = EntityType::Statement;
  }else if (designEntity == "read"){
    eType = EntityType::Read;
  }else if (designEntity == "print"){
    eType = EntityType::Print;
  }else if (designEntity == "call"){
    eType = EntityType::Call;
  }else if (designEntity == "while"){
    eType = EntityType::While;
  }else if (designEntity == "if"){
    eType = EntityType::If;
  }else if (designEntity == "assign"){
    eType = EntityType::Assignment;
  }else if (designEntity == "variable"){
    eType = EntityType::Variable;
  }else if (designEntity == "constant"){
    eType = EntityType::Constant;
  }else if (designEntity == "procedure"){
    eType = EntityType::Procedure;
  }else{
    eType = EntityType::Wildcard; // just putting some default type down
  }
  for(int i=1; i<designEntityArr.size(); i++){
    std::string entityName = designEntityArr[i];
    Entity newEntity = Entity(eType, entityName);
    entityMap[entityName] = newEntity;
  }
  return entityMap;
}

RelationshipRef QueryProcessor::createRelationshipObject(std::vector<std::string> relRefList, const std::unordered_map<std::string, Entity>& entityMap){
  std::string relStr = relRefList[0];
  RelationshipType rType;

  if (relStr == "Follows"){
    rType = RelationshipType::Follows;
  }else if (relStr == "Follows*"){
    rType = RelationshipType::FollowsT;
  }else if (relStr == "Parent"){
    rType = RelationshipType::Parent;
  }else if (relStr == "Parent*"){
    rType = RelationshipType::ParentT;
  }else if (relStr == "Uses"){
    rType = RelationshipType::Uses;
  }else if (relStr == "Modifies"){
    rType = RelationshipType::Modifies;
  }else{
    rType = RelationshipType::Uses; // just putting some default type down
  }
  Entity leftEntity = findRelationshipEntity(relRefList[1], entityMap);
  Entity rightEntity = findRelationshipEntity(relRefList[2], entityMap);

  return RelationshipRef(rType, leftEntity, rightEntity);
}

Entity QueryProcessor::findRelationshipEntity(const std::string& s, std::unordered_map<std::string, Entity> entityMap){
  if (entityMap.find(s) != entityMap.end()){
    return entityMap[s]; //
  }else if(s == "_"){
    return Entity(EntityType::Wildcard, "_");
  }else if(isInteger(s)){
    return Entity(EntityType::FixedInteger, s);
  }else if(isQuotationIdent(s)){
    return Entity(EntityType::FixedString, s);
  }else{
    return Entity();
  }
}

std::vector<Clause> QueryProcessor::parsePQL(const std::string& parsePQL) {
  bool isValid = true;
  std::vector<Clause> clauseList;
  std::unordered_map<std::string, Entity> entityMap;

  std::vector<std::string> declarationStmtList = extractDeclaration(parsePQL);
  std::vector<std::string> selectStmtList = extractSelect(parsePQL);

  for (const auto& declarationStmt: declarationStmtList){
    std::vector<std::string> designEntityArr = extractDesignEntityAndSynonym(declarationStmt);

    if (!checkDesignEntitySynonyms(designEntityArr)){
      isValid = false;
    }else{
      entityMap = createDeclarationObjects(designEntityArr, entityMap);
    }
  }

  if (isValid){
    for (const auto& selectStmt: selectStmtList){
      Clause newClause = Clause();
      std::vector<std::string> variablesToSelect = extractVariablesToSelect(selectStmt);
      std::vector<std::string> SuchThatClauses = extractSuchThatClauses(selectStmt);
      std::vector<std::string> PatternClauses = extractPatternClauses(selectStmt);

      for (const auto& s: variablesToSelect){
        isValid = isValid && isIdent(s) && (entityMap.find(s) != entityMap.end());
        if (isValid){
          newClause.appendEntityToFind(entityMap[s]);
        }else{
          isValid = false;
        }
      }

      for (const auto& s: SuchThatClauses){
        std::vector<std::string> relRefList = extractItemsInBrackets(s);
        isValid = isValid && checkRelRefArr(relRefList);
        if (isValid){
          RelationshipRef newRef = createRelationshipObject(relRefList, entityMap);
          newClause.appendRef(newRef);
        }
      }

      std::cout << newClause.toString() << "\n";
      /*
      FOR NEXT WEEK
      for (auto s: PatternClauses){
        //validate pattern
        //create relationship object
      }
      */
    }
  }

  if (isValid){
    return clauseList;
  }else{
    std::vector<Clause> emptyClause;
    return emptyClause;
  }
}
