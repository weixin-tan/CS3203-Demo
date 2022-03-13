#include "WithHandler.h"
WithHandler::WithHandler(PkbGetter *pg) {
  WithHandler::pg = pg;
}

Result WithHandler::handleWith(const RelationshipRef &relRef) {
  Result result;
  std::set<ProgramElement> oneSynSet;
  std::set<std::pair<ProgramElement, ProgramElement>> twoSynSet;

  Entity left = relRef.leftEntity;
  Entity right = relRef.rightEntity;
  result.setResultType(ResultType::WithClause);

  if (isFixedEntity(left) && isFixedEntity(right)){
    result.setValid(handleBothSidesFixed(left, right));
    return result;
  }else if ( isFixedEntity(left) && !isFixedEntity(right)){
    oneSynSet = handleOneSideFixed(left, right);
    result.setOneSynEntity(right);
    result.setOneSynSet(oneSynSet);
  }else if (!isFixedEntity(left) && isFixedEntity(right)){
    oneSynSet = handleOneSideFixed(right, left);
    result.setOneSynEntity(left);
    result.setOneSynSet(oneSynSet);
  }else{
    twoSynSet = handleNoSideFixed(left, right);
    result.setTwoSynEntities(std::pair<Entity, Entity>(left, right));
    result.setTwoSynSet(twoSynSet);
  }

  if (oneSynSet.empty() && twoSynSet.empty()) {
    result.setValid(false);
  } else {
    result.setValid(true);
  }
  return result;
}

bool WithHandler::isFixedEntity(const Entity &entity) {
  return entity.eType == EntityType::FixedString || entity.eType == EntityType::FixedInteger;
}

std::set<ProgramElement> WithHandler::getProgramElements(const EntityType& eType) {
  ElementType elementType = QpsTypeToPkbTypeConvertor::convertToPkbElement(eType);
  return pg->getEntity(elementType);
}

std::string WithHandler::getStringToMatch(const ProgramElement& element, EntityAttributeType elementType) {
  if (elementType == EntityAttributeType::ProcName){
    return element.procName;
  }else if(elementType == EntityAttributeType::VarName){
    return element.varName;
  }else if(elementType == EntityAttributeType::Value){
    return element.varName;
  }else{
    return std::to_string(element.stmtNo);
  }
}

std::set<ProgramElement> WithHandler::matchProgramElements(const std::set<ProgramElement>& setA,
                                                           const Entity& fixedEntity,
                                                           EntityAttributeType aType) {
  std::set<ProgramElement> toReturn;
  std::string stringA;
  std::string stringB;
  for (const auto& element: setA){
    stringA = getStringToMatch(element, aType);
    stringB = fixedEntity.name;
    if (stringA == stringB){
      toReturn.insert(element);
    }
  }
  return toReturn;
}

std::set<std::pair<ProgramElement, ProgramElement>> WithHandler::matchProgramElements(const std::set<ProgramElement>& setA,
                                                                                      const std::set<ProgramElement>& setB,
                                                                                      EntityAttributeType aType,
                                                                                      EntityAttributeType bType) {
  std::set<std::pair<ProgramElement, ProgramElement>> toReturn;
  std::string stringA;
  std::string stringB;
  for (const auto& elementA: setA){
    stringA = getStringToMatch(elementA, aType);
    for (const auto& elementB: setB){
      stringB = getStringToMatch(elementB, bType);
      if (stringA == stringB){
        std::pair<ProgramElement, ProgramElement> tempPair(elementA, elementB);
        toReturn.insert(tempPair);
      }
    }
  }
  return toReturn;
}

bool WithHandler::handleBothSidesFixed(const Entity &leftEntity, const Entity &rightEntity) {
  return leftEntity == rightEntity;
}

std::set<ProgramElement> WithHandler::handleOneSideFixed(const Entity &fixedSide, const Entity &notFixedSide) {
  std::set<ProgramElement> programElementsSet = getProgramElements(notFixedSide.eType);
  return matchProgramElements(programElementsSet, fixedSide, notFixedSide.aType);
}



std::set<std::pair<ProgramElement, ProgramElement>> WithHandler::handleNoSideFixed(const Entity &leftEntity,
                                                                                   const Entity &rightEntity) {
  std::set<ProgramElement> setA = getProgramElements(leftEntity.eType);
  std::set<ProgramElement> setB = getProgramElements(rightEntity.eType);

  if (setA.size() < setB.size()){
    return matchProgramElements(setA, setB, leftEntity.aType, rightEntity.aType);
  }else{
    return matchProgramElements(setB, setA, rightEntity.aType, leftEntity.aType);
  }
}






