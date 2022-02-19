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
  std::set<ProgramElement> getNoClauseElements();
  std::set<std::pair<ProgramElement, ProgramElement>> getSuchThatElements();
  std::set<ProgramElement> getPatternElements();
  Entity getResultEntity();
  Entity getLeftSuchThatEntity();
  Entity getRightSuchThatEntity();
  Entity getAssignEntity();
  bool allSuchThatAndPatternEntitiesNull();
};


#endif //SPA_RESULT_H
