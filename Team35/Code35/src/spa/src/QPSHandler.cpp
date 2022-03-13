#include "QPSHandler.h"
#include "QpsTypeToPkbTypeConvertor.h"

QPSHandler::QPSHandler(PkbGetter* pg) {
  QPSHandler::pg = pg;
  QPSHandler::patternHandler = new PatternHandler(pg);
  QPSHandler::suchThatHandler = new SuchThatHandler(pg);
  QPSHandler::withHandler = new WithHandler(pg);
}

std::vector<Result> QPSHandler::processClause(const std::vector<Clause> &clauses) const {
  std::vector<Result> results;
  for(const Clause& c : clauses) {
    std::vector<Entity> entityToFindList = c.entityToFindList;
    Entity entityToFind = entityToFindList.front(); // Only 1 entity to find for now.
    std::vector<RelationshipRef> refList = c.refList;
    Result result;

    if (refList.empty()) {
      result = handleNoRelationshipRef(entityToFind);
      results.push_back(result);
      continue;
    }

    for (const RelationshipRef& r : refList) {
      assert(("Relationship type is Null", r.rType != RelationshipType::Null));
      bool a = !QpsTypeToPkbTypeConvertor::isValid(r.leftEntity.name) ;
      bool b = !QpsTypeToPkbTypeConvertor::isValid(r.rightEntity.name);
      bool c = !QpsTypeToPkbTypeConvertor::isValid(r.AssignmentEntity.name);
      if (!QpsTypeToPkbTypeConvertor::isValid(r.leftEntity.name) || !QpsTypeToPkbTypeConvertor::isValid(r.rightEntity.name) || !QpsTypeToPkbTypeConvertor::isValid(r.AssignmentEntity.name)) {
        std::vector<Result> empty = {};
        return empty;
      }
      if (r.rType == RelationshipType::Pattern) {
        result = patternHandler->handlePattern(entityToFind, r);
      } else if (r.rType == RelationshipType::With){
        result = withHandler->handleWith(entityToFind, r);
      }else {
        result = suchThatHandler->handleSuchThat(entityToFind, r);
      }
      results.push_back(result);
    }
  }
  return results;
}

Result QPSHandler::handleNoRelationshipRef(const Entity& entityToFind) const {
  Result result;
  ElementType elementTypeToGet = QpsTypeToPkbTypeConvertor::extractElementType(entityToFind);
  std::set<ProgramElement> resultElements = pg->getEntity(elementTypeToGet);
  result.setNoClauseElements(resultElements);
  result.setResultEntity(entityToFind);
  return result;
}
