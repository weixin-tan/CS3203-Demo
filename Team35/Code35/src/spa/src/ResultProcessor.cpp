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
    std::set<std::pair<ProgramElement, ProgramElement>> resultWithCommonEntities;
    std::set<ProgramElement> resultForProcessor;
    bool common = false;

    if (suchThatResult.getLeftSuchThatEntity() == patternResult.getAssignEntity() && patternResult.getAssignEntRef().eType == EntityType::Null) {
      common = true;
      for (const auto& r : suchThatPairs) {
        for (const auto& a : patternPairs) {
          if (r.first == a) {
            resultWithCommonEntities.insert(r);
          }
        }
      }
    }
    if (suchThatResult.getRightSuchThatEntity() == patternResult.getAssignEntity() && patternResult.getAssignEntRef().eType == EntityType::Null) {
      common = true;
      for (const auto& r : suchThatPairs) {
        for (const auto& a : patternPairs) {
          if (r.second == a) {
            resultWithCommonEntities.insert(r);
          }
        }
      }
    }
    if (suchThatResult.getLeftSuchThatEntity() == patternResult.getAssignEntity() && suchThatResult.getRightSuchThatEntity() == patternResult.getAssignEntRef()) {
      common = true;
      for (const auto& r : suchThatPairs) {
        for (const auto& a : patternEntRefPairs) {
          if (r.first == a.first && r.second == a.second) {
            resultWithCommonEntities.insert(r);
          }
        }
      }
    }

    if (common) {
      if (resultWithCommonEntities.empty()) {
        return emptyResultElements;
      }
      if (resultEntity == suchThatResult.getLeftSuchThatEntity()) {
        for (const auto& r :resultWithCommonEntities) {
          resultForProcessor.insert(r.first);
        }
      }
      if (resultEntity == suchThatResult.getRightSuchThatEntity()) {
        for (const auto& r :resultWithCommonEntities) {
          resultForProcessor.insert(r.second);
        }
      }
      if (resultForProcessor.empty()) {
        return suchThatResult.getNoClauseElements();
      } else {
        return resultForProcessor;
      }
    } else {
      if (suchThatResult.getRightSuchThatEntity() == patternResult.getAssignEntRef()) {
        std::set<std::tuple<ProgramElement, ProgramElement, ProgramElement>> commonResult;
        for (const auto& r : suchThatPairs) {
          for (const auto& a : patternEntRefPairs) {
            if (r.second == a.second) {
              std::tuple<ProgramElement, ProgramElement, ProgramElement> commons (r.first, r.second, a.first);
              commonResult.insert(commons);
            }
          }
        }
        if (commonResult.empty()) {
          return emptyResultElements;
        }
        if (resultEntity == suchThatResult.getLeftSuchThatEntity()) {
          for (const auto& r :commonResult) {
            resultForProcessor.insert(std::get<0>(r));
          }
        }
        if (resultEntity == suchThatResult.getRightSuchThatEntity()) {
          for (const auto& r :commonResult) {
            resultForProcessor.insert(std::get<1>(r));
          }
        }
        if (resultEntity == patternResult.getAssignEntity()) {
          for (const auto& r :commonResult) {
            resultForProcessor.insert(std::get<2>(r));
          }
        }
        if (resultForProcessor.empty()) {
          return suchThatResult.getNoClauseElements();
        } else {
          return resultForProcessor;
        }
      } else {
        return suchThatResult.getNoClauseElements();
      }
    }
  } else {
      return emptyResultElements;
  }
}
