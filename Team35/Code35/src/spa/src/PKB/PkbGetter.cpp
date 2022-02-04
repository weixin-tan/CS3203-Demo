#include "PkbGetter.h"
#include "../Type.h"  // KIV: might be problematic
#include "DB.h"
#include <set>
#include <cassert>

PkbGetter::PkbGetter(DB* db) : db(db) {}

bool inline isUndefined(const Entity& e) {
  return e.eType == Wildcard && e.name.empty();
}

bool inline isStatement(const EntityType& eType) {
  return eType == Assignment || eType == Read || eType == While || eType == If || eType == Print || eType == Call;
}

bool inline isStatement(const Entity& e) {
  return isStatement(e.eType);
}

bool PkbGetter::isRelationship(const RelationshipType& r, const Entity& leftSide, const Entity& rightSide) const {
  assert(!(isUndefined(leftSide) || isUndefined(rightSide)));  // must be named
  if (r == Modifies) {
    assert(rightSide.eType == Variable);
    if (isStatement(leftSide)) {
      const std::set<std::string>& modifiedVars = db->modifyStmtToVarTable[stoi(leftSide.name)];
      return modifiedVars.find(rightSide.name) != modifiedVars.end();
    }
    if (leftSide.eType == Procedure) {
      const std::set<std::string>& modifiedVars = db->modifyProcToVarTable[leftSide.name];
      return modifiedVars.find(rightSide.name) != modifiedVars.end();
    }
  }
  assert(false);  // not implemented yet
}

std::vector<Entity> PkbGetter::getRelationshipStatements(const RelationshipType& r) const {
  std::vector<Entity> result;
  if (r == Modifies) {
    for (const auto& [stmtNo, _]: db->modifyStmtToVarTable)
      result.emplace_back(Entity(Assignment, std::to_string(stmtNo)));
  }
  return result;
}

std::vector<Entity> PkbGetter::getEntity(const EntityType& typeToGet) const {
  std::vector<Entity> result;
  if (typeToGet == Variable)
    for (const std::string& var : db->variables)
      result.emplace_back(Entity(Variable, var));
  if (isStatement(typeToGet))
    for (const auto& [stmtNo, eType] : db->stmtTypeTable)
      if (eType == typeToGet)
        result.emplace_back(Entity(eType, std::to_string(stmtNo)));

  return result;
}

std::vector<Entity> PkbGetter::getLeftSide(const RelationshipType& r, const Entity& rightSide,
                                           const EntityType& typeToGet) const {
  std::vector<Entity> result;
  assert(!isUndefined(rightSide));
  if (r == Modifies) {
    assert(rightSide.eType == Variable);
    if (isStatement(typeToGet))
      for (const int& stmtNo: db->varToModifyStmtTable[rightSide.name])
        if (db->stmtTypeTable[stmtNo] == typeToGet)
          result.emplace_back(Entity(typeToGet, std::to_string(stmtNo)));
  }
  return result;
}

std::vector<Entity> PkbGetter::getRightSide(const RelationshipType& r, const Entity& leftSide,
                                            const EntityType& typeToGet) const {
  std::vector<Entity> result;
  assert(!isUndefined(leftSide));
  if (r == Modifies) {
    assert(typeToGet == Variable);
    assert(isStatement(leftSide) || leftSide.eType == Procedure);
    if (isStatement(leftSide)) {
      for (const std::string& var : db->modifyStmtToVarTable[std::stoi(leftSide.name)]) {
        result.emplace_back(Entity(Variable, var));
      }
    }
  }
  return result;
}
