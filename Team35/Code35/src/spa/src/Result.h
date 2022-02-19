#ifndef SPA_RESULT_H
#define SPA_RESULT_H

#include <set>
#include <utility>
#include "ProgramElement.h"
#include "RelationshipRef.h"

class Result {
 public:
  std::set<ProgramElement> noClauseElements = {};
  std::set<std::pair<ProgramElement, ProgramElement>> suchThatElements = {};
  std::set<ProgramElement> patternElements = {};
  Entity resultEntity;
  Entity leftSuchThatEntity;
  Entity rightSuchThatEntity;
  Entity assignEntity;

  Result();

  void setNoClauseElements(std::set<ProgramElement> elements);
  void setSuchThatElements(std::set<std::pair<ProgramElement, ProgramElement>> elements);
  void setPatternElements(std::set<ProgramElement> elements);
  void setResultEntity(Entity entityToGet);
  void setLeftSuchThatEntity(Entity left);
  void setRightSuchThatEntity(Entity right);
  void setAssignEntity(Entity assign);
  std::set<ProgramElement> getNoClauseElements() const;
  std::set<std::pair<ProgramElement, ProgramElement>> getSuchThatElements() const;
  std::set<ProgramElement> getPatternElements() const;
  Entity getResultEntity() const;
  Entity getLeftSuchThatEntity() const;
  Entity getRightSuchThatEntity() const;
  Entity getAssignEntity() const;
  bool allSuchThatAndPatternEntitiesNull() const;
};


#endif //SPA_RESULT_H
