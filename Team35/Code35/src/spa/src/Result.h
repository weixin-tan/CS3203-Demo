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
  std::set<std::pair<ProgramElement, ProgramElement>> entRefElements = {};
  Entity resultEntity;
  Entity leftSuchThatEntity;
  Entity rightSuchThatEntity;
  Entity assignEntity;
  Entity assignEntRef;

  Result();

  void setNoClauseElements(std::set<ProgramElement> elements);
  void setSuchThatElements(std::set<std::pair<ProgramElement, ProgramElement>> elements);
  void setPatternElements(std::set<ProgramElement> elements);
  void setEntRefElements(std::set<std::pair<ProgramElement, ProgramElement>> elements);
  void setResultEntity(Entity entityToGet);
  void setLeftSuchThatEntity(Entity left);
  void setRightSuchThatEntity(Entity right);
  void setAssignEntity(Entity assign);
  void setAssignEntRef(Entity entRef);
  std::set<ProgramElement> getNoClauseElements();
  std::set<std::pair<ProgramElement, ProgramElement>> getSuchThatElements();
  std::set<ProgramElement> getPatternElements();
  std::set<std::pair<ProgramElement, ProgramElement>> getEntRefElements();
  Entity getResultEntity();
  Entity getLeftSuchThatEntity();
  Entity getRightSuchThatEntity();
  Entity getAssignEntity();
  Entity getAssignEntRef();
  bool allSuchThatAndPatternEntitiesNull();
};


#endif //SPA_RESULT_H
