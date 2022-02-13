#include "PatternHandler.h"


PatternHandler::PatternHandler(PkbGetter* pg) {
  PatternHandler::pg = pg;
}

Result PatternHandler::handlePattern(Entity entityToGet, RelationshipRef relRef) {
  RelationshipType relType = relRef.rType;
  Entity left = relRef.leftEntity;
  Entity right = relRef.rightEntity;
  Entity assign = relRef.AssignmentEntity;

  if (entityToGet != assign) {

  }





    return Result();
}
