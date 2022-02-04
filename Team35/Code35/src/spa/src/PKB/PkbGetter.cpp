#include "PkbGetter.h"
#include "Type.h"
#include "DB.h"
#include <set>
#include <cassert>

PkbGetter::PkbGetter(DB* db) : db(db) {}

bool inline isUndefined(const Entity& e) {
  return e.cType == Wildcard && e.name.empty();
}

bool inline isStatement(const Entity& e) {
  return e.cType == Assignment
         || e.cType == Read
         || e.cType == While
         || e.cType == If
         || e.cType == Print
         || e.cType == Call;
}

bool PkbGetter::isRelationship(const RelationshipType& r, const Entity& leftSide, const Entity& rightSide) const {
  assert(!(isUndefined(leftSide) || isUndefined(rightSide)));  // must be named
  if (r == Modifies) {
    assert(rightSide.cType == Variable);
    if (isStatement(leftSide)) {
      const int& varId = db->varToIdTable[rightSide.name];
      const std::set<int>& modifiedVars = db->modifiesStmtTable[stoi(leftSide.name)];
      return modifiedVars.find(varId) != modifiedVars.end();
    }
    if (leftSide.cType == Procedure) {
      const int& procId = db->procToIdTable[rightSide.name];
      const std::set<int>& modifiedVars = db->modifiesProcTable[stoi(leftSide.name)];
      return modifiedVars.find(procId) != modifiedVars.end();
    }
  }
  assert(false);  // not implemented yet
}
