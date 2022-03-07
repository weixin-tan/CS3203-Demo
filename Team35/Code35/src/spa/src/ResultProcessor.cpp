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
    if (onlyResult.getAssignEntRef().eType == EntityType::Null && onlyResult.getAssignEntity().eType != EntityType::Null) {
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
    if (onlyResult.getAssignEntRef().eType != EntityType::Null && onlyResult.getAssignEntity().eType != EntityType::Null){
      std::set<ProgramElement> assigns;
      std::set<ProgramElement> vars;
      for (const auto &r : onlyResult.getEntRefElements()) {
        assigns.insert(r.first);
        vars.insert(r.second);
      }
      if (onlyResult.getAssignEntity() == onlyResult.getResultEntity()) {
        return assigns;
      } else if (onlyResult.getResultEntity() == onlyResult.getAssignEntRef()) {
        return vars;
      } else {
        if (onlyResult.getEntRefElements().empty()) {
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


    if (results[0].getAssignEntity().eType == EntityType::Null && results[0].getRightSuchThatEntity().eType != EntityType::Null
    && results[0].getLeftSuchThatEntity().eType != EntityType::Null) {
      suchThatResult = results[0];
      patternResult = results[1];
    } else {
      suchThatResult = results[1];
      patternResult = results[0];
    }

    Entity resultEntity = suchThatResult.getResultEntity(); // Both results have the same result entity.

    if (suchThatResult.getSuchThatElements().empty() || (patternResult.getPatternElements().empty() && patternResult.getEntRefElements().empty())) {
      return emptyResultElements;
    }

    std::set<std::pair<ProgramElement, ProgramElement>> suchThatPairs = suchThatResult.getSuchThatElements();
    std::set<std::pair<ProgramElement, ProgramElement>> patternEntRefPairs = patternResult.getEntRefElements();
    std::set<ProgramElement> patternPairs = patternResult.getPatternElements();
    std::set<std::pair<ProgramElement, ProgramElement>> resultWithTwoEntities;
    std::set<std::tuple<ProgramElement, ProgramElement, ProgramElement>> resultWithThreeEntities;
    std::set<ProgramElement> resultForProcessor;
    bool common = false;

    if (suchThatResult.getLeftSuchThatEntity() == patternResult.getAssignEntity() && suchThatResult.getRightSuchThatEntity() == patternResult.getAssignEntRef()) {
      common = true;
      for (const auto& r : suchThatPairs) {
        for (const auto& a : patternEntRefPairs) {
          if (r.first == a.first && r.second == a.second) {
            resultWithTwoEntities.insert(r);
          }
        }
      }
    } else if (suchThatResult.getLeftSuchThatEntity() == patternResult.getAssignEntity() && patternResult.getAssignEntRef().eType == EntityType::Null) {
      common = true;
      for (const auto& r : suchThatPairs) {
        for (const auto& a : patternPairs) {
          if (r.first == a) {
            resultWithTwoEntities.insert(r);
          }
        }
      }
    } else if (suchThatResult.getRightSuchThatEntity() == patternResult.getAssignEntity() && patternResult.getAssignEntRef().eType == EntityType::Null) {
      common = true;
      for (const auto& r : suchThatPairs) {
        for (const auto& a : patternPairs) {
          if (r.second == a) {
            resultWithTwoEntities.insert(r);
          }
        }
      }
    } else if (suchThatResult.getLeftSuchThatEntity() == patternResult.getAssignEntity()) {
      common = true;
      for (const auto& r : suchThatPairs) {
        for (const auto& a : patternEntRefPairs) {
          if (r.first == a.first) {
            std::tuple<ProgramElement, ProgramElement, ProgramElement> tup (r.first, r.second, a.second);
            resultWithThreeEntities.insert(tup);
          }
        }
      }
    } else if (suchThatResult.getRightSuchThatEntity() == patternResult.getAssignEntity()) {
      common = true;
      for (const auto& r : suchThatPairs) {
        for (const auto& a : patternEntRefPairs) {
          if (r.second == a.first) {
            std::tuple<ProgramElement, ProgramElement, ProgramElement> tup (r.first, r.second, a.second);
            resultWithThreeEntities.insert(tup);
          }
        }
      }
    } else if (suchThatResult.getRightSuchThatEntity() == patternResult.getAssignEntRef()) {
      common = true;
      for (const auto& r : suchThatPairs) {
        for (const auto& a : patternEntRefPairs) {
          if (r.second == a.second) {
            std::tuple<ProgramElement, ProgramElement, ProgramElement> tup (r.first, r.second, a.first);
            resultWithThreeEntities.insert(tup);
          }
        }
      }
    } else {
      common = false;
    }

    if (common) {
      if (resultWithTwoEntities.empty() && resultWithThreeEntities.empty()) {
        return emptyResultElements;
      }
      if (resultEntity == suchThatResult.getLeftSuchThatEntity() && patternResult.getAssignEntRef().eType == EntityType::Null) {
        for (const auto& r :resultWithTwoEntities) {
          resultForProcessor.insert(r.first);
        }
      } else if (resultEntity == suchThatResult.getRightSuchThatEntity() && patternResult.getAssignEntRef().eType == EntityType::Null) {
        for (const auto& r :resultWithTwoEntities) {
          resultForProcessor.insert(r.second);
        }
      } else if (resultEntity == suchThatResult.getLeftSuchThatEntity()) {
        for (const auto& r :resultWithThreeEntities) {
          resultForProcessor.insert(std::get<0>(r));
        }
      } else if (resultEntity == suchThatResult.getRightSuchThatEntity()) {
        for (const auto& r :resultWithThreeEntities) {
          resultForProcessor.insert(std::get<1>(r));
        }
      } else if (resultEntity == patternResult.getAssignEntity() || resultEntity == patternResult.getAssignEntRef()) {
        for (const auto& r :resultWithThreeEntities) {
          resultForProcessor.insert(std::get<2>(r));
        }
      } else {

      }

      if (resultForProcessor.empty()) {
        return suchThatResult.getNoClauseElements();
      } else {
        return resultForProcessor;
      }
    } else {
      std::set<ProgramElement> firstElem;
      std::set<ProgramElement> secondElem;
      std::set<ProgramElement> assignElem;
      std::set<ProgramElement> assignEntRef;

      for (const auto& s: suchThatResult.getSuchThatElements()) {
        firstElem.insert(s.first);
        secondElem.insert(s.second);
      }
      for (const auto& s: patternResult.getEntRefElements()) {
        assignElem.insert(s.first);
        assignEntRef.insert(s.second);
      }

      if (resultEntity == suchThatResult.getLeftSuchThatEntity()) {
          return firstElem;
      } else if (resultEntity == suchThatResult.getRightSuchThatEntity()) {
          return secondElem;
      } else if (resultEntity == patternResult.getAssignEntity()) {
        if (patternResult.getAssignEntity().eType == EntityType::Null) {
          return patternResult.getPatternElements();
        } else {
          return assignElem;
        }
      } else if (resultEntity == patternResult.getAssignEntRef()) {
        return assignEntRef;
      } else {
        return suchThatResult.getNoClauseElements();
      }
    }
  } else {
      return emptyResultElements;
  }
}
