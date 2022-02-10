#ifndef SPA_PKBGETTER_H
#define SPA_PKBGETTER_H

#include <vector>
#include "ProgramElement.h"
#include "DB.h"

class PkbGetter {
private:
  DB* db;

public:
  explicit PkbGetter(DB* db);
  bool isRelationship(const RelationshipType& r, const ProgramElement &leftSide, const ProgramElement &rightSide) const;
  std::set<ProgramElement> getEntity(const ElementType &typeToGet) const;
  std::set<ProgramElement> getRelationshipStatements(const RelationshipType& r) const;
  std::set<ProgramElement> getLeftSide(const RelationshipType& r, const ProgramElement &rightSide, const ElementType &typeToGet) const;
  std::set<ProgramElement> getRightSide(const RelationshipType& r, const ProgramElement &leftSide, const ElementType &typeToGet) const;
};

#endif //SPA_PKBGETTER_H
