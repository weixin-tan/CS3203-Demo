#include "PkbGetter.h"
#include "../Type.h"  // KIV: might be problematic
#include "DB.h"
#include <set>
#include <cassert>

#define NULL_STMT_NO -1

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

  switch (r) {
    case Modifies: {
      assert(rightSide.eType == Variable);
      if (isStatement(leftSide)) {
        int leftStmtNo = std::stoi(leftSide.name);
        assert(db->stmtTypeTable[leftStmtNo] == leftSide.eType);
        const std::set<std::string>& modifiedVars = db->modifyStmtToVarTable[leftStmtNo];
        result = modifiedVars.find(rightSide.name) != modifiedVars.end();
        if (leftSide.eType == EntityType::If || leftSide.eType == EntityType::While) {
          for (const int& childStmtNo: db->parentToChildTable[leftStmtNo]) {
            if (result) break;
            result = isRelationship(r, Entity(db->stmtTypeTable[childStmtNo], std::to_string(childStmtNo)), rightSide);
          }
        }
      } else if (leftSide.eType == Procedure) {
        const std::set<std::string>& modifiedVars = db->modifyProcToVarTable[leftSide.name];
        result = modifiedVars.find(rightSide.name) != modifiedVars.end();
      }
      break;
    }
    case Uses: {
      assert(rightSide.eType == Variable);
      if (isStatement(leftSide)) {
        int leftStmtNo = std::stoi(leftSide.name);
        assert(db->stmtTypeTable[leftStmtNo] == leftSide.eType);
        const std::set<std::string>& usedVars = db->usesStmtToVarTable[leftStmtNo];
        result = usedVars.find(rightSide.name) != usedVars.end();
        if (leftSide.eType == EntityType::If || leftSide.eType == EntityType::While) {
          for (const int& childStmtNo: db->parentToChildTable[leftStmtNo]) {
            if (result) break;
            result = isRelationship(r, Entity(db->stmtTypeTable[childStmtNo], std::to_string(childStmtNo)), rightSide);
          }
        }
      } else if (leftSide.eType == Procedure) {
        const std::set<std::string>& usedVars = db->usesProcToVarTable[leftSide.name];
        result = usedVars.find(rightSide.name) != usedVars.end();
      }
      break;
    }
    case Parent: {
      assert(isStatement(leftSide) && isStatement(rightSide));
      result = db->childToParentTable[std::stoi(rightSide.name)] == std::stoi(leftSide.name);
      break;
    }
    case Follows: {
      assert(isStatement(leftSide) && isStatement(rightSide));
      result = db->stmtFollowing[std::stoi(leftSide.name)] == std::stoi(rightSide.name);
      break;
    }
    case ParentT: {
      int targetStmtNo = std::stoi(leftSide.name);
      int currentStmtNo = std::stoi(rightSide.name);
      while (currentStmtNo != NULL_STMT_NO) {
        currentStmtNo = db->childToParentTable[currentStmtNo];
        if (currentStmtNo == targetStmtNo) {
          result = true;
          break;
        }
      }
      break;
    }
    case FollowsT: {
      int targetStmtNo = std::stoi(leftSide.name);
      int currentStmtNo = std::stoi(rightSide.name);
      while (currentStmtNo != NULL_STMT_NO) {
        currentStmtNo = db->stmtFollowing[currentStmtNo];
        if (currentStmtNo == targetStmtNo) {
          result = true;
          break;
        }
      }
      break;
    }
    default: {
      assert(false);
    }
  }

  return result;
}

std::vector<Entity> PkbGetter::getRelationshipStatements(const RelationshipType& r) const {
  std::vector<Entity> result;

  switch (r) {
    case Modifies: {
      for (const auto&[stmtNo, _]: db->modifyStmtToVarTable)
        result.emplace_back(Entity(db->stmtTypeTable[stmtNo], std::to_string(stmtNo)));
      break;
    }
    case Uses: {
      for (const auto&[stmtNo, _]: db->usesStmtToVarTable)
        result.emplace_back(Entity(db->stmtTypeTable[stmtNo], std::to_string(stmtNo)));
      break;
    }
    default: {
      assert(false);
    }
  }

  return result;
}

std::vector<Entity> PkbGetter::getEntity(const EntityType& typeToGet) const {
  std::vector<Entity> result;

  if (isStatement(typeToGet)) {
    for (const auto&[stmtNo, eType]: db->stmtTypeTable)
      if (eType == typeToGet)
        result.emplace_back(Entity(eType, std::to_string(stmtNo)));
  }
  switch (typeToGet) {
    case Variable: {
      for (const std::string& var: db->variables)
        result.emplace_back(Entity(Variable, var));
      break;
    }
    case Procedure: {
      for (const std::string& proc: db->procedures)
        result.emplace_back(Entity(Procedure, proc));
      break;
    }
    default: {
      assert(false);
    }
  }

  return result;
}

std::vector<Entity> PkbGetter::getLeftSide(const RelationshipType& r, const Entity& rightSide,
                                           const EntityType& typeToGet) const {
  assert(!isUndefined(rightSide));
  std::vector<Entity> result;

  switch (r) {
    case Modifies: {
      assert(isStatement(typeToGet) || typeToGet == Procedure);
      assert(rightSide.eType == Variable);

      if (isStatement(typeToGet)) {
        for (const int& stmtNo: db->varToModifyStmtTable[rightSide.name])
          if (db->stmtTypeTable[stmtNo] == typeToGet)
            result.emplace_back(Entity(typeToGet, std::to_string(stmtNo)));
      } else if (typeToGet == Procedure) {
        for (const std::string& proc: db->varToModifyProcTable[rightSide.name])
          result.emplace_back(Entity(typeToGet, proc));
      }
      break;
    }
    case Uses: {
      assert(isStatement(typeToGet) || typeToGet == Procedure);
      assert(rightSide.eType == Variable);

      if (isStatement(typeToGet)) {
        for (const int& stmtNo: db->varToUsesStmtTable[rightSide.name])
          if (db->stmtTypeTable[stmtNo] == typeToGet)
            result.emplace_back(Entity(typeToGet, std::to_string(stmtNo)));
      } else if (typeToGet == Procedure) {
        for (const std::string& proc: db->varToUsesProcTable[rightSide.name])
          result.emplace_back(Entity(typeToGet, proc));
      }
      break;
    }
    case Follows: {
      assert(isStatement(rightSide) && isStatement(typeToGet));

      int targetStmtNo = db->stmtPreceding[std::stoi(rightSide.name)];
      if (targetStmtNo != NULL_STMT_NO && db->stmtTypeTable[targetStmtNo] == typeToGet)
        result.emplace_back(Entity(typeToGet, std::to_string(targetStmtNo)));
      break;
    }
    case Parent: {
      assert(isStatement(rightSide) && isStatement(typeToGet));

      int targetStmtNo = db->childToParentTable[std::stoi(rightSide.name)];
      if (targetStmtNo != NULL_STMT_NO && db->stmtTypeTable[targetStmtNo] == typeToGet)
        result.emplace_back(Entity(typeToGet, std::to_string(targetStmtNo)));
      break;
    }
    case FollowsT: {
      assert(isStatement(rightSide) && isStatement(typeToGet));

      int currentStmtNo = db->stmtPreceding[std::stoi(rightSide.name)];
      while (currentStmtNo != NULL_STMT_NO) {
        if (db->stmtTypeTable[currentStmtNo] == typeToGet)
          result.emplace_back(Entity(typeToGet, std::to_string(currentStmtNo)));
        currentStmtNo = db->stmtPreceding[currentStmtNo];
      }

      break;
    }
    case ParentT: {
      assert(isStatement(rightSide) && isStatement(typeToGet));

      int currentStmtNo = db->childToParentTable[std::stoi(rightSide.name)];
      while (currentStmtNo != NULL_STMT_NO) {
        if (db->stmtTypeTable[currentStmtNo] == typeToGet)
          result.emplace_back(Entity(typeToGet, std::to_string(currentStmtNo)));
        currentStmtNo = db->childToParentTable[currentStmtNo];
      }

      break;
    }
    default: {
      assert(false);
    }
  }

  return result;
}

std::vector<Entity> PkbGetter::getRightSide(const RelationshipType& r, const Entity& leftSide,
                                            const EntityType& typeToGet) const {
  assert(!isUndefined(leftSide));

  std::vector<Entity> result;
  switch (r) {
    case Modifies: {
      assert(isStatement(leftSide) || leftSide.eType == Procedure);
      assert(typeToGet == Variable);

      if (isStatement(leftSide)) {
        for (const std::string& var: db->modifyStmtToVarTable[std::stoi(leftSide.name)])
          result.emplace_back(Entity(typeToGet, var));
      } else if (leftSide.eType == Procedure) {
        for (const std::string& var: db->modifyProcToVarTable[leftSide.name])
          result.emplace_back(Entity(typeToGet, var));
      }
    }
    case Uses: {
      assert(isStatement(leftSide) || leftSide.eType == Procedure);
      assert(typeToGet == Variable);

      if (isStatement(leftSide)) {
        for (const std::string& var: db->usesStmtToVarTable[std::stoi(leftSide.name)])
          result.emplace_back(Entity(typeToGet, var));
      } else if (leftSide.eType == Procedure) {
        for (const std::string& var: db->usesProcToVarTable[leftSide.name])
          result.emplace_back(Entity(typeToGet, var));
      }
    }
    case Follows: {
      assert(isStatement(leftSide) && isStatement(typeToGet));

      int targetStmtNo = db->stmtFollowing[std::stoi(leftSide.name)];
      if (targetStmtNo != NULL_STMT_NO && db->stmtTypeTable[targetStmtNo] == typeToGet)
        result.emplace_back(Entity(typeToGet, std::to_string(targetStmtNo)));
      break;
    }
    case Parent: {
      assert(isStatement(leftSide) && isStatement(typeToGet));

      for (const int& targetStmtNo : db->parentToChildTable[std::stoi(leftSide.name)])
        if (db->stmtTypeTable[targetStmtNo] == typeToGet)
          result.emplace_back(Entity(typeToGet, std::to_string(targetStmtNo)));
      break;
    }
    case FollowsT: {
      assert(isStatement(leftSide) && isStatement(typeToGet));

      int currentStmtNo = db->stmtFollowing[std::stoi(leftSide.name)];
      while (currentStmtNo != NULL_STMT_NO) {
        if (db->stmtTypeTable[currentStmtNo] == typeToGet)
          result.emplace_back(Entity(typeToGet, std::to_string(currentStmtNo)));
        currentStmtNo = db->stmtPreceding[currentStmtNo];
      }

      break;
    }
    case ParentT: {
      assert(isStatement(leftSide) && isStatement(typeToGet));

      for (const int& childStmtNo : db->parentToChildTable[std::stoi(leftSide.name)]) {
        EntityType childStmtType = db->stmtTypeTable[childStmtNo];
        if (childStmtType == typeToGet)
          result.emplace_back(Entity(typeToGet, std::to_string(childStmtNo)));
        if (childStmtType == If || childStmtType == While) {
          const std::vector<Entity>& childResult = getRightSide(r, Entity(childStmtType, std::to_string(childStmtNo)), typeToGet);
          for (const Entity& e: childResult)
            result.emplace_back(e);
        }
      }
      break;
    }
    default: {
      assert(false);
    }
  }

  return result;
}
