#include <set>
#include <cassert>

#include "PkbGetter.h"
#include "ProgramElement.h"
#include "DB.h"

#define NULL_STMT_NO -1

PkbGetter::PkbGetter(DB* db) : db(db) {}

bool PkbGetter::isRelationship(const RelationshipType& r, const ProgramElement &leftSide, const ProgramElement &rightSide) const {
  bool result = false;

  switch (r) {
    case RelationshipType::Modifies : {
      assert(rightSide.eType == EntityType::Variable);
      if (isStatement(leftSide)) {
        int leftStmtNo = std::stoi(leftSide.name);
        assert(db->stmtTypeTable[leftStmtNo] == leftSide.eType);
        const std::set<std::string>& modifiedVars = db->modifyStmtToVarTable[leftStmtNo];
        result = modifiedVars.find(rightSide.name) != modifiedVars.end();
        for (const int& childStmtNo: db->parentToChildTable[leftStmtNo]) {
          if (result) break;
          result = isRelationship(r, Entity(db->stmtTypeTable[childStmtNo], std::to_string(childStmtNo)), rightSide);
        }
      } else if (leftSide.eType == EntityType::Procedure) {
        assert(false);
      }
      break;
    }
    case RelationshipType::Uses: {
      assert(rightSide.eType == EntityType::Variable);
      if (isStatement(leftSide)) {
        int leftStmtNo = std::stoi(leftSide.name);
        assert(db->stmtTypeTable[leftStmtNo] == leftSide.eType);
        const std::set<std::string>& usedVars = db->usesStmtToVarTable[leftStmtNo];
        result = usedVars.find(rightSide.name) != usedVars.end();
        for (const int& childStmtNo: db->parentToChildTable[leftStmtNo]) {
          if (result) break;
          result = isRelationship(r, Entity(db->stmtTypeTable[childStmtNo], std::to_string(childStmtNo)), rightSide);
        }
      } else if (leftSide.eType == EntityType::Procedure) {
        assert(false);
      }
      break;
    }
    case RelationshipType::Parent: {
      assert(isStatement(leftSide) && isStatement(rightSide));
      result = db->childToParentTable[std::stoi(rightSide.name)] == std::stoi(leftSide.name);
      break;
    }
    case RelationshipType::Follows: {
      assert(isStatement(leftSide) && isStatement(rightSide));
      result = db->stmtFollowing[std::stoi(leftSide.name)] == std::stoi(rightSide.name);
      break;
    }
    case RelationshipType::ParentT: {
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
    case RelationshipType::FollowsT: {
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

std::set<ProgramElement> PkbGetter::getRelationshipStatements(const RelationshipType& r) const {
  std::vector<Entity> result;

  switch (r) {
    case RelationshipType::Modifies: {
      for (const auto&[stmtNo, _]: db->modifyStmtToVarTable)
        result.emplace_back(Entity(db->stmtTypeTable[stmtNo], std::to_string(stmtNo)));
      break;
    }
    case RelationshipType::Uses: {
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

std::set<ProgramElement> PkbGetter::getEntity(const ElementType &typeToGet) const {
  std::vector<Entity> result;

  if (isStatement(typeToGet)) {
    for (const auto&[stmtNo, eType]: db->stmtTypeTable)
      if (eType == typeToGet)
        result.emplace_back(Entity(eType, std::to_string(stmtNo)));
  }
  switch (typeToGet) {
    case EntityType::Variable: {
      for (const std::string& var: db->variables)
        result.emplace_back(Entity(EntityType::Variable, var));
      break;
    }
    case EntityType::Procedure: {
      assert(false);
    }
    default: {
      assert(isStatement(typeToGet));
    }
  }

  return result;
}

std::set<ProgramElement> PkbGetter::getLeftSide(const RelationshipType& r, const ProgramElement &rightSide,
                                                const ElementType &typeToGet) const {
  assert(!isUndefined(rightSide));
  std::vector<Entity> result;

  switch (r) {
    case RelationshipType::Modifies: {
      assert(isStatement(typeToGet) || typeToGet == EntityType::Procedure);
      assert(rightSide.eType == EntityType::Variable);

      if (isStatement(typeToGet)) {
        std::set<int> stmtNos = {};
        for (const int &stmtNo : db->varToModifyStmtTable[rightSide.name]) {
          int curStmtNo = stmtNo;
          while (curStmtNo != NULL_STMT_NO && stmtNos.find(curStmtNo) == stmtNos.end()) {
            if (db->stmtTypeTable.at(curStmtNo) == typeToGet)
              stmtNos.insert(curStmtNo);
            curStmtNo = db->childToParentTable.at(curStmtNo);
          }
        }
        for (const int &stmtNo : stmtNos)
          result.emplace_back(Entity(typeToGet, std::to_string(stmtNo)));
      }
      break;
    }
    case RelationshipType::Uses: {
      assert(isStatement(typeToGet) || typeToGet == EntityType::Procedure);
      assert(rightSide.eType == EntityType::Variable);

      if (isStatement(typeToGet)) {
        std::set<int> stmtNos = {};
        for (const int& stmtNo: db->varToUsesStmtTable[rightSide.name]) {
          int curStmtNo = stmtNo;
          while (curStmtNo != NULL_STMT_NO && stmtNos.find(curStmtNo) == stmtNos.end()) {
            if (db->stmtTypeTable.at(curStmtNo) == typeToGet)
              stmtNos.insert(curStmtNo);
            curStmtNo = db->childToParentTable.at(curStmtNo);
          }
        }
        for (const int& stmtNo: stmtNos)
          result.emplace_back(Entity(typeToGet, std::to_string(stmtNo)));
      } else if (typeToGet == EntityType::Procedure) {
        assert(false);
      }
      break;
    }
    case RelationshipType::Follows: {
      assert(isStatement(rightSide) && isStatement(typeToGet));

      int targetStmtNo = db->stmtPreceding[std::stoi(rightSide.name)];
      if (targetStmtNo != NULL_STMT_NO && db->stmtTypeTable[targetStmtNo] == typeToGet)
        result.emplace_back(Entity(typeToGet, std::to_string(targetStmtNo)));
      break;
    }
    case RelationshipType::Parent: {
      assert(isStatement(rightSide) && isStatement(typeToGet));

      int targetStmtNo = db->childToParentTable[std::stoi(rightSide.name)];
      if (targetStmtNo != NULL_STMT_NO && db->stmtTypeTable[targetStmtNo] == typeToGet)
        result.emplace_back(Entity(typeToGet, std::to_string(targetStmtNo)));
      break;
    }
    case RelationshipType::FollowsT: {
      assert(isStatement(rightSide) && isStatement(typeToGet));

      int currentStmtNo = db->stmtPreceding[std::stoi(rightSide.name)];
      while (currentStmtNo != NULL_STMT_NO) {
        if (db->stmtTypeTable[currentStmtNo] == typeToGet)
          result.emplace_back(Entity(typeToGet, std::to_string(currentStmtNo)));
        currentStmtNo = db->stmtPreceding[currentStmtNo];
      }

      break;
    }
    case RelationshipType::ParentT: {
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

std::set<ProgramElement> PkbGetter::getRightSide(const RelationshipType& r, const ProgramElement &leftSide,
                                                 const ElementType &typeToGet) const {
  std::set<ProgramElement> result;
  switch (r) {
    case RelationshipType::Modifies: {
      assert(isStatement(leftSide) || leftSide.eType == EntityType::Procedure);
      assert(typeToGet == EntityType::Variable);

      std::set<std::string> vars = {};

      if (isStatement(leftSide)) {
        for (const std::string& var : db->modifyStmtToVarTable[std::stoi(leftSide.name)])
          vars.insert(var);

        for (const int& childStmtNo: db->parentToChildTable[std::stoi(leftSide.name)])
          for (const Entity& e: getRightSide(RelationshipType::Modifies, Entity(db->stmtTypeTable[childStmtNo], std::to_string(childStmtNo)), EntityType::Variable))
            vars.insert(e.name);

        for (const std::string& var : vars)
          result.emplace_back(Entity(EntityType::Variable, var));
      } else {
        assert(false);
      }
      break;
    }
    case RelationshipType::Uses: {
      assert(isStatement(leftSide) || leftSide.eType == EntityType::Procedure);
      assert(typeToGet == EntityType::Variable);

      std::set<std::string> vars = {};
      if (isStatement(leftSide)) {
        for (const std::string& var : db->usesStmtToVarTable[std::stoi(leftSide.name)])
          vars.insert(var);

        for (const int& childStmtNo: db->parentToChildTable[std::stoi(leftSide.name)])
          for (const Entity& e: getRightSide(RelationshipType::Uses, Entity(db->stmtTypeTable[childStmtNo], std::to_string(childStmtNo)), typeToGet))
            vars.insert(e.name);

        for (const std::string& var : vars)
          result.emplace_back(Entity(EntityType::Variable, var));
      } else {
        assert(false);
      }
      break;
    }
    case RelationshipType::Follows: {
      assert(isStatement(leftSide) && isStatement(typeToGet));

      int targetStmtNo = db->stmtFollowing[std::stoi(leftSide.name)];
      if (targetStmtNo != NULL_STMT_NO && db->stmtTypeTable[targetStmtNo] == typeToGet)
        result.emplace_back(Entity(typeToGet, std::to_string(targetStmtNo)));
      break;
    }
    case RelationshipType::Parent: {
      assert(isStatement(leftSide) && isStatement(typeToGet));

      for (const int& targetStmtNo : db->parentToChildTable[std::stoi(leftSide.name)])
        if (db->stmtTypeTable[targetStmtNo] == typeToGet)
          result.emplace_back(Entity(typeToGet, std::to_string(targetStmtNo)));
      break;
    }
    case RelationshipType::FollowsT: {
      assert(isStatement(leftSide) && isStatement(typeToGet));

      int currentStmtNo = db->stmtFollowing[std::stoi(leftSide.name)];
      while (currentStmtNo != NULL_STMT_NO) {
        if (db->stmtTypeTable[currentStmtNo] == typeToGet)
          result.emplace_back(Entity(typeToGet, std::to_string(currentStmtNo)));
        currentStmtNo = db->stmtPreceding[currentStmtNo];
      }

      break;
    }
    case RelationshipType::ParentT: {
      assert(isStatement(leftSide) && isStatement(typeToGet));

      for (const int& childStmtNo : db->parentToChildTable[std::stoi(leftSide.name)]) {
        EntityType childStmtType = db->stmtTypeTable[childStmtNo];
        if (childStmtType == typeToGet)
          result.emplace_back(Entity(typeToGet, std::to_string(childStmtNo)));
        const std::vector<Entity>& childResult = getRightSide(r, Entity(childStmtType, std::to_string(childStmtNo)), typeToGet);
          for (const Entity& e: childResult)
            result.emplace_back(e);
      }
      break;
    }
    default: {
      assert(false);
    }
  }

  return result;
}
