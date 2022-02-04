#include "PkbGetter.h"
#include "../Type.h"  // KIV: might be problematic
#include "DB.h"
#include <set>
#include <cassert>

PkbGetter::PkbGetter(DB* db) : db(db) {}

bool inline isUndefined(const Entity& e) {
  return e.eType == Wildcard && e.name.empty();
}

bool inline isStatement(const Entity& e) {
  return e.eType == Assignment
         || e.eType == Read
         || e.eType == While
         || e.eType == If
         || e.eType == Print
         || e.eType == Call;
}

bool PkbGetter::isRelationship(const RelationshipType& r, const Entity& leftSide, const Entity& rightSide) const {
  assert(!(isUndefined(leftSide) || isUndefined(rightSide)));  // must be named
  if (r == Modifies) {
    assert(rightSide.eType == Variable);
    if (isStatement(leftSide)) {
      const std::set<std::string>& modifiedVars = db->modifiesStmtTable[stoi(leftSide.name)];
      return modifiedVars.find(rightSide.name) != modifiedVars.end();
    }
    if (leftSide.eType == Procedure) {
      const std::set<std::string>& modifiedVars = db->modifiesProcTable[stoi(leftSide.name)];
      return modifiedVars.find(rightSide.name) != modifiedVars.end();
    }
  }
  assert(false);  // not implemented yet
}
