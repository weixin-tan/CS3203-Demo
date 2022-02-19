#include <iostream>
#include "ResultProcessor.h"

ResultProcessor::ResultProcessor() = default;

std::set<ProgramElement> ResultProcessor::processResults(std::vector<Result> results) {
  std::set<ProgramElement> emptyResultElements = {};

  if (results.size() == 1) {
    Result onlyResult = results[0];

    // Handle no such that and no pattern clause
    if (onlyResult.getPatternElements().empty() && onlyResult.getSuchThatElements().empty()
    && onlyResult.allSuchThatAndPatternEntitiesNull()) {
      return onlyResult.getNoClauseElements();
    }

    // Handle pattern clause
    if (onlyResult.getAssignEntity().eType != EntityType::Null) {
      if (onlyResult.getAssignEntity() == onlyResult.getResultEntity()) {
        return onlyResult.getPatternElements();
      } else {
        if (onlyResult.getPatternElements().empty()) {
          return emptyResultElements;
        } else {
          return onlyResult.getNoClauseElements();
        }
      }
    }

    // Handle such that clause
    if (onlyResult.getLeftSuchThatEntity().eType != EntityType::Null && onlyResult.getRightSuchThatEntity().eType != EntityType::Null) {
      std::set<std::pair<ProgramElement, ProgramElement>> resultPairs = onlyResult.getSuchThatElements();
      std::set<ProgramElement> resultElements;
      if (onlyResult.getResultEntity() == onlyResult.getLeftSuchThatEntity() && onlyResult.getResultEntity() == onlyResult.getRightSuchThatEntity()) {
        return emptyResultElements;
      } else if (onlyResult.getResultEntity() == onlyResult.getLeftSuchThatEntity()) {
        for (const auto& r : resultPairs) {
          resultElements.insert(r.first);
        }
        return resultElements;
      } else if (onlyResult.getResultEntity() == onlyResult.getRightSuchThatEntity()) {
        for (const auto& r : resultPairs) {
          resultElements.insert(r.second);
        }
        return resultElements;
      } else {
        return onlyResult.getNoClauseElements();
      }
    }
    return emptyResultElements; // In case something goes wrong.
  } else if (results.size() == 2) {
    Result patternResult;
    Result suchThatResult;
    Entity resultEntity = suchThatResult.getResultEntity(); // Both results have the same result entity.

    if (results[0].getAssignEntity().eType == EntityType::Null && results[0].getRightSuchThatEntity().eType != EntityType::Null
    && results[0].getLeftSuchThatEntity().eType != EntityType::Null) {
      suchThatResult = results[0];
      patternResult = results[1];
    } else {
      suchThatResult = results[1];
      patternResult = results[0];
    }

    // Takes care of cases where left and right such that are the same as result element
    if (suchThatResult.getSuchThatElements().empty() || patternResult.getPatternElements().empty()) {
      return emptyResultElements;
    }

    std::set<std::pair<ProgramElement, ProgramElement>> resultPairs = suchThatResult.getSuchThatElements();
    std::set<ProgramElement> patternElements = patternResult.getPatternElements();
    std::set<ProgramElement> extractLeftEntity;
    std::set<ProgramElement> extractRightEntity;
    std::set<ProgramElement> finalResult;

    for (const auto& r : resultPairs) {
      extractLeftEntity.insert(r.first);
      extractRightEntity.insert(r.second);
    }

    if (resultEntity == patternResult.getAssignEntity() && resultEntity == suchThatResult.getLeftSuchThatEntity()) {
      //Return all left such that entity
      for (const auto& e : extractLeftEntity) {
        if (patternElements.count(e)) {
          finalResult.insert(e);
        }
      }
      return finalResult;

    } else if (resultEntity == patternResult.getAssignEntity() && resultEntity == suchThatResult.getRightSuchThatEntity()){
      //Return all right such that entity
      for (const auto& e : extractRightEntity) {
        if (patternElements.count(e)) {
          finalResult.insert(e);
        }
      }
      return finalResult;

    } else if (resultEntity == patternResult.getAssignEntity()) { // Not equal to left and right such that entities
      //Return all pattern entities
      return patternElements;

    } else if (resultEntity != patternResult.getAssignEntity() && resultEntity == suchThatResult.getLeftSuchThatEntity()) {

      if (patternResult.getAssignEntity() == suchThatResult.getRightSuchThatEntity()) {
        for (const auto& e : resultPairs) {
          if (patternElements.count(e.second)) {
            finalResult.insert(e.first);
          }
        }
        return finalResult;
      } else {
        return extractLeftEntity;
      }

    } else if (resultEntity != patternResult.getAssignEntity() && resultEntity == suchThatResult.getRightSuchThatEntity()) {

      if (patternResult.getAssignEntity() == suchThatResult.getLeftSuchThatEntity()) {
        for (const auto& e : resultPairs) {
          if (patternElements.count(e.first)) {
            finalResult.insert(e.second);
          }
        }
        return finalResult;
      } else {
        return extractRightEntity;
      }

    } else { // All three don't match result entity

      if (patternResult.getAssignEntity() == suchThatResult.getLeftSuchThatEntity()) {
        for (const auto& e : resultPairs) {
          if (patternElements.count(e.first)) {
            finalResult.insert(e.second);
          }
        }
        if (!finalResult.empty()) {
          return suchThatResult.getNoClauseElements();
        } else {
          return emptyResultElements;
        }
      } else if (patternResult.getAssignEntity() == suchThatResult.getRightSuchThatEntity()) {
        for (const auto& e : resultPairs) {
          if (patternElements.count(e.second)) {
            finalResult.insert(e.first);
          }
        }
        if (!finalResult.empty()) {
          return suchThatResult.getNoClauseElements();
        } else {
          return emptyResultElements;
        }
      } else {
        return suchThatResult.getNoClauseElements();
      }
    }

  } else {
      return emptyResultElements;
  }
}
