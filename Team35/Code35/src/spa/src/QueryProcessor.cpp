#include "QueryProcessor.h"

QueryProcessor::QueryProcessor()= default;

void QueryProcessor::createDeclarationObjects(std::vector<std::string> designEntityArr, std::unordered_map<std::string, Entity>* entityMap){
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
    // this entity is NULL -> invalid
    eType = EntityType::Null;
  }
  for(int i=1; i<designEntityArr.size(); i++){
    std::string entityName = designEntityArr[i];
    Entity newEntity = Entity(eType, entityName);
    (*entityMap)[entityName] = newEntity;
  }
}

RelationshipRef QueryProcessor::createRelationshipObject(std::vector<std::string> relRefList, std::unordered_map<std::string, Entity>* entityMap){
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
    //this relationship type is NULL -> invalid
    rType = RelationshipType::Null;
  }
  Entity leftEntity = findRelationshipEntity(relRefList[1], entityMap);
  Entity rightEntity = findRelationshipEntity(relRefList[2], entityMap);

  if (rType == RelationshipType::Null || leftEntity.eType == EntityType::Null || rightEntity.eType == EntityType::Null){
    return RelationshipRef(RelationshipType::Null , leftEntity, rightEntity);
  }else{
    return RelationshipRef(rType, leftEntity, rightEntity);
  }
}

Entity QueryProcessor::findRelationshipEntity(const std::string& s, std::unordered_map<std::string, Entity>* entityMap){
  if (entityMapContains(s, entityMap)){
    return (*entityMap)[s];
  }else if(isWildCard(s)){
    return Entity(EntityType::Wildcard, "_");
  }else if(isInteger(s)){
    return Entity(EntityType::FixedInteger, s);
  }else if(isQuotationIdent(s)){
    std::string s2 = s.substr(1, s.length() - 2);
    return Entity(EntityType::FixedString, s2);
  }else{
    // this entity is NULL -> invalid
    return Entity(EntityType::Null, s);
  }
}

RelationshipRef QueryProcessor::createPatternObject(std::vector<std::string> patternList, std::unordered_map<std::string, Entity>* entityMap){
  RelationshipType rType = RelationshipType::Pattern;
  Entity assignmentEntity = findRelationshipEntity(patternList[0], entityMap);
  Entity leftEntity = findRelationshipEntity(patternList[1], entityMap);
  Entity rightEntity = createExpressionEntity(patternList[2]);

  if (assignmentEntity.eType == EntityType::Null || leftEntity.eType == EntityType::Null || rightEntity.eType == EntityType::Null){
    return RelationshipRef(RelationshipType::Null , leftEntity, rightEntity);
  }else {
    return RelationshipRef(rType, leftEntity, rightEntity, assignmentEntity);
  }
}

Entity QueryProcessor::createExpressionEntity(const std::string &s) {
  if(isWildCard(s)){
    return Entity(EntityType::Wildcard, "_");
  }else if(isQuotationIdent(s)){
    std::string s2 = s.substr(1, s.length() - 2);
    return Entity(EntityType::FixedString, s2);
  }else if(isStringWithinWildCard(s)){
    std::string s2 = s.substr(2, s.length() - 4);
    return Entity(EntityType::FixedStringWithinWildcard, s2);
  }else{
    // this entity is NULL -> invalid
    return Entity(EntityType::Null, s);
  }
}

std::vector<Clause> QueryProcessor::parsePQL(const std::string& parsePQL) {
  bool isValid = true;
  std::vector<Clause> clauseList;
  std::unordered_map<std::string, Entity> entityMap;
  std::vector<std::string> declarationStmtList = extractDeclaration(parsePQL);
  std::vector<std::string> selectStmtList = extractSelect(parsePQL);

  for (const auto& declarationStmt: declarationStmtList){
    std::vector<std::string> designEntityArr = extractDesignEntityAndSynonyms(declarationStmt);
    isValid = isValid && checkDesignEntitySynonyms(designEntityArr);
    if (isValid){
      createDeclarationObjects(designEntityArr, &entityMap);
    }
  }

  if (isValid){
    for (const auto& selectStmt: selectStmtList){
      Clause newClause = Clause();
      std::vector<std::string> variablesToSelect = extractVariablesToSelect(selectStmt);
      std::vector<std::string> SuchThatClauses = extractSuchThatClauses(selectStmt);
      std::vector<std::string> PatternClauses = extractPatternClauses(selectStmt);

      if (existSuchThat(selectStmt)){
        isValid = isValid && (SuchThatClauses.size() > 0);
      }else{
        isValid = isValid && (SuchThatClauses.empty());
      }

      for (const auto& s: variablesToSelect){
        isValid = isValid && isIdent(s) && (entityMap.find(s) != entityMap.end());
        if (isValid){
          newClause.appendEntityToFind(entityMap[s]);
        }
      }

      for (const auto& s: SuchThatClauses){
        std::vector<std::string> relRefList = extractItemsInBrackets(s);
        isValid = isValid && checkRelRefList(relRefList);
        if (isValid){
          RelationshipRef newRef = createRelationshipObject(relRefList, &entityMap);
          if (newRef.rType == RelationshipType::Null){
            isValid = false;
            break;
          }else{
            newClause.appendRef(newRef);
          }
        }
      }

      for (auto s: PatternClauses){
        s = removePattern(s);
        std::vector<std::string> patternList = extractItemsInBrackets(s);
        isValid = isValid && checkPatternList(patternList, &entityMap);

        if (isValid){
          RelationshipRef newRef = createPatternObject(patternList, &entityMap);
          if (newRef.rType == RelationshipType::Null){
            isValid = false;
            break;
          }else{
            newClause.appendRef(newRef);
          }
        }
      }
      //std::cout << "\n" << newClause.toString() << "\n";
      clauseList.push_back(newClause);
    }
  }

  if (isValid){
    //std::cout << "is valid!";
    return clauseList;
  }else{
    //std::cout << "is invalid!";
    std::vector<Clause> emptyClause;
    return emptyClause;
  }
}