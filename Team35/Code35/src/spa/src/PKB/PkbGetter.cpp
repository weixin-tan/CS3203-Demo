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

  bool result = false;

  if (r == Modifies) {
    assert(rightSide.eType == Variable);
    if (isStatement(leftSide)) {
      const std::set<std::string>& modifiedVars = db->modifyStmtToVarTable[stoi(leftSide.name)];
      result = modifiedVars.find(rightSide.name) != modifiedVars.end();
    } else if (leftSide.eType == Procedure) {
      const std::set<std::string>& modifiedVars = db->modifyProcToVarTable[leftSide.name];
      result = modifiedVars.find(rightSide.name) != modifiedVars.end();
    }
  }

  return result;
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

  if (isStatement(typeToGet)) {
    for (const auto&[stmtNo, eType]: db->stmtTypeTable)
      if (eType == typeToGet)
        result.emplace_back(Entity(eType, std::to_string(stmtNo)));
  } else if (typeToGet == Variable) {
    for (const std::string& var: db->variables)
      result.emplace_back(Entity(Variable, var));
  } else if (typeToGet == Procedure) {
    for (const std::string& proc: db->procedures)
      result.emplace_back(Entity(Procedure, proc));
  }

  return result;
}

std::vector<Entity> PkbGetter::getLeftSide(const RelationshipType& r, const Entity& rightSide,
                                           const EntityType& typeToGet) const {
  assert(!isUndefined(rightSide));
  std::vector<Entity> result;

  if (r == Modifies) {
    assert(rightSide.eType == Variable);

    if (isStatement(typeToGet)) {
      for (const int& stmtNo : db->varToModifyStmtTable[rightSide.name])
        if (db->stmtTypeTable[stmtNo] == typeToGet)
          result.emplace_back(Entity(typeToGet, std::to_string(stmtNo)));
    } else if (typeToGet == Procedure) {
      for (const std::string& proc : db->varToModifyProcTable[rightSide.name])
        result.emplace_back(Entity(typeToGet, proc));
    }
  }
  return result;
}

std::vector<Entity> PkbGetter::getRightSide(const RelationshipType& r, const Entity& leftSide,
                                            const EntityType& typeToGet) const {
  assert(!isUndefined(leftSide));

  std::vector<Entity> result;
  if (r == Modifies) {
    assert(typeToGet == Variable);

    if (isStatement(leftSide)) {
      for (const std::string& var : db->modifyStmtToVarTable[std::stoi(leftSide.name)])
        result.emplace_back(Entity(Variable, var));
    } else if (leftSide.eType == Procedure) {
      for (const std::string& proc : db->modifyProcToVarTable[leftSide.name])
        result.emplace_back(Entity(Variable, proc));
    }
  }

  return result;
}
