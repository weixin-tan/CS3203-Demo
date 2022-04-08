#include "QpsTypeToPkbTypeConvertor.h"
#include "SuchThatHandler.h"

//Constructor
SuchThatHandler::SuchThatHandler(PkbGetter* pg) {
    SuchThatHandler::pg = pg;
}

//Handle Such That relationships
Result SuchThatHandler::handleSuchThat(const RelationshipRef& relRef) {
    Result result;
    std::set<ProgramElement*> oneSynSet;
    std::set<std::pair<ProgramElement*, ProgramElement*>> twoSynSet;

    PkbRelationshipType relType = QpsTypeToPkbTypeConvertor::convertToPkbRelType(relRef.rType);
    Entity left = relRef.leftEntity;
    Entity right = relRef.rightEntity;

    result.setResultType(ResultType::SUCH_THAT_CLAUSE);

    if (isNonSynonymEntity(left.eType) && isNonSynonymEntity(right.eType)) {
        result.setValid(handleNoSynonyms(relType, left, right));
        return result;
    } else if (isNonSynonymEntity(left.eType)) {
        oneSynSet = handleRightSyn(relType, left, right);
        result.setOneSynEntity(right);
        result.setOneSynSet(oneSynSet);
    } else if (isNonSynonymEntity(right.eType)) {
        oneSynSet = handleLeftSyn(relType, left, right);
        result.setOneSynEntity(left);
        result.setOneSynSet(oneSynSet);
    } else {
        twoSynSet = handleTwoSyn(relType, left, right);
        result.setTwoSynEntities(std::pair<Entity, Entity>(left, right));
        result.setTwoSynSet(twoSynSet);
    }

    if (oneSynSet.empty() && twoSynSet.empty()) {
        result.setValid(false);
    } else {
        result.setValid(true);
    }

    return result;
}

// Checks if entity is not a synonym
bool SuchThatHandler::isNonSynonymEntity(EntityType e) {
    return e == EntityType::WILDCARD || e == EntityType::FIXED_STRING || e == EntityType::FIXED_INTEGER;
}

// Handles cases where there are no synonyms on either side.
bool SuchThatHandler::handleNoSynonyms(PkbRelationshipType r, const Entity& left, const Entity& right) {
    // IF there are wildcards present, then move to wildcard handlers based on relationship type
    if (left.eType == EntityType::WILDCARD || right.eType == EntityType::WILDCARD) {
        if (r == PkbRelationshipType::CALLS || r == PkbRelationshipType::CALLS_T) {
            return handleNoSynProcWildcard(r, left, right);
        } else if (r == PkbRelationshipType::MODIFIES || r == PkbRelationshipType::USES) {
            return handleNoSynVarWildcard(r, left);
        } else {
            return handleNoSynStmtWildcard(r, left, right);
        }
    }

    // If both sides are fixed entities, then check pkb if the relationship exists
    if (left.eType == EntityType::FIXED_STRING && right.eType == EntityType::FIXED_STRING &&
            (r == PkbRelationshipType::CALLS || r == PkbRelationshipType::CALLS_T)) {
        return pg->isRelationship(r,
                                  ProgramElement::createProcedure(left.name),
                                  ProgramElement::createProcedure(right.name));
    } else if (left.eType == EntityType::FIXED_INTEGER && right.eType == EntityType::FIXED_INTEGER) {
        return pg->isRelationship(r,
                                  ProgramElement::createStatement(ElementType::STATEMENT,
                                                                  std::stoi(left.name)),
                                  ProgramElement::createStatement(ElementType::STATEMENT,
                                                                  std::stoi(right.name)));
    } else if (left.eType == EntityType::FIXED_INTEGER && right.eType == EntityType::FIXED_STRING) {
        return pg->isRelationship(r,
                                  ProgramElement::createStatement(ElementType::STATEMENT,
                                                                  std::stoi(left.name)),
                                  ProgramElement::createVariable(right.name));
    } else if (left.eType == EntityType::FIXED_STRING && right.eType == EntityType::FIXED_STRING) {
        return pg->isRelationship(r,
                                  ProgramElement::createProcedure(left.name),
                                  ProgramElement::createVariable(right.name));
    } else {
        return false;
    }
}

// Handles cases where there are wildcards in Calls/CALLS_T
bool SuchThatHandler::handleNoSynProcWildcard(PkbRelationshipType r, const Entity& left, const Entity& right) {
    bool valid = false;

    if (left.eType == EntityType::WILDCARD && right.eType == EntityType::WILDCARD) {
        std::set<std::pair<ProgramElement*, ProgramElement*>> check = pg->getRelationshipPairs(r,
                                                                                             ElementType::PROCEDURE,
                                                                                             ElementType::PROCEDURE);
        if (!check.empty()) {
            valid = true;
        }
    } else if (left.eType == EntityType::WILDCARD) {
        std::set<ProgramElement*> check = pg->getLeftSide(r,
                                                         ProgramElement::createProcedure(right.name),
                                                         ElementType::PROCEDURE);
        if (!check.empty()) {
            valid = true;
        }
    } else {
        std::set<ProgramElement*> check = pg->getRightSide(r,
                                                          ProgramElement::createProcedure(left.name),
                                                          ElementType::PROCEDURE);
        if (!check.empty()) {
            valid = true;
        }
    }

    return valid;
}

// Handles cases where there are wildcards in Follows/FOLLOWS_T/Parent/PARENT_T/Next/NEXT_T/Affects/AFFECTS_T
bool SuchThatHandler::handleNoSynStmtWildcard(PkbRelationshipType r, const Entity& left, const Entity& right) {
    bool valid = false;

    if (left.eType == EntityType::WILDCARD && right.eType == EntityType::WILDCARD) {
        std::set<std::pair<ProgramElement*, ProgramElement*>> check;
        check = pg->getRelationshipPairs(r, ElementType::STATEMENT, ElementType::STATEMENT);

        if (!check.empty()) {
            valid = true;
        }

    } else if (left.eType == EntityType::WILDCARD) {
        std::set<ProgramElement*> check;
        check = pg->getLeftSide(r,
                                ProgramElement::createStatement(ElementType::STATEMENT,
                                                                    std::stoi(right.name)),
                                ElementType::STATEMENT);

        if (!check.empty()) {
            valid = true;
        }

    } else {
        std::set<ProgramElement*> check;
        check = pg->getRightSide(r, ProgramElement::createStatement(ElementType::STATEMENT,
                                                                        std::stoi(left.name)),
                                 ElementType::STATEMENT);

        if (!check.empty()) {
            valid = true;
        }
    }

    return valid;
}

// Handles cases where there are wildcards in Modifies/Uses
bool SuchThatHandler::handleNoSynVarWildcard(PkbRelationshipType r, const Entity& left) {
    bool valid = false;

    if (left.eType == EntityType::FIXED_STRING) {
        std::set<ProgramElement*> check = pg->getRightSide(r,
                                                          ProgramElement::createProcedure(left.name),
                                                          ElementType::VARIABLE);
        if (!check.empty()) {
            valid = true;
        }
    }
    if (left.eType == EntityType::FIXED_INTEGER) {
        std::set<ProgramElement*> check = pg->getRightSide(r,
                                                          ProgramElement::createStatement(ElementType::STATEMENT,
                                                                                          std::stoi(left.name)),
                                                          ElementType::VARIABLE);
        if (!check.empty()) {
            valid = true;
        }
    }

    return valid;
}

// Handles cases where there is a synonym on right side
std::set<ProgramElement*> SuchThatHandler::handleRightSyn(PkbRelationshipType r, const Entity& left, const Entity& right) {
    std::set<ProgramElement*> oneSynSet;
    ElementType rightElemType = QpsTypeToPkbTypeConvertor::convertToPkbElement(right.eType);

    if (left.eType == EntityType::FIXED_STRING) {
        oneSynSet = pg->getRightSide(r,
                                     ProgramElement::createProcedure(left.name),
                                     rightElemType);
    } else if (left.eType == EntityType::FIXED_INTEGER) {
        oneSynSet = pg->getRightSide(r,
                                     ProgramElement::createStatement(ElementType::STATEMENT, std::stoi(left.name)),
                                     rightElemType);
    } else { // Handles cases where left side is a wildcard
        std::set<std::pair<ProgramElement*, ProgramElement*>> pairs;
        if (r == PkbRelationshipType::CALLS || r == PkbRelationshipType::CALLS_T) {
            pairs = pg->getRelationshipPairs(r, ElementType::PROCEDURE, rightElemType);
        } else {
            pairs = pg->getRelationshipPairs(r, ElementType::STATEMENT, rightElemType);
        }
        for (const auto& p : pairs) {
            oneSynSet.insert(p.second);
        }
    }

    return oneSynSet;
}

// Handles cases where there is a synonym on left side
std::set<ProgramElement*> SuchThatHandler::handleLeftSyn(PkbRelationshipType r, const Entity& left, const Entity& right) {
    std::set<ProgramElement*> oneSynSet;
    ElementType leftElemType = QpsTypeToPkbTypeConvertor::convertToPkbElement(left.eType);

    if (right.eType == EntityType::FIXED_INTEGER) {
        oneSynSet = pg->getLeftSide(r,
                                    ProgramElement::createStatement(ElementType::STATEMENT, std::stoi(right.name)),
                                    leftElemType);
    } else if (right.eType == EntityType::FIXED_STRING) {
        if (r == PkbRelationshipType::MODIFIES || r == PkbRelationshipType::USES) {
            oneSynSet = pg->getLeftSide(r, ProgramElement::createVariable(right.name), leftElemType);
        } else {
            oneSynSet = pg->getLeftSide(r, ProgramElement::createProcedure(right.name), leftElemType);
        }
    } else {
        std::set<std::pair<ProgramElement*, ProgramElement*>> pairs;
        if (r == PkbRelationshipType::CALLS || r == PkbRelationshipType::CALLS_T) {
            pairs = pg->getRelationshipPairs(r, leftElemType, ElementType::PROCEDURE);
        } else if (r == PkbRelationshipType::MODIFIES || r == PkbRelationshipType::USES) {
            pairs = pg->getRelationshipPairs(r, leftElemType, ElementType::VARIABLE);
        } else {
            pairs = pg->getRelationshipPairs(r, leftElemType, ElementType::STATEMENT);
        }
        for (const auto& p : pairs) {
            oneSynSet.insert(p.first);
        }
    }

    return oneSynSet;
}

// Handles cases where there are synonyms on both sides
std::set<std::pair<ProgramElement*, ProgramElement*>>
SuchThatHandler::handleTwoSyn(PkbRelationshipType r, const Entity& left, const Entity& right) {
    ElementType leftElemType = QpsTypeToPkbTypeConvertor::convertToPkbElement(left.eType);
    ElementType rightElemType = QpsTypeToPkbTypeConvertor::convertToPkbElement(right.eType);
    std::set<std::pair<ProgramElement*, ProgramElement*>> allPairs = pg->getRelationshipPairs(r,leftElemType,
                                                                                            rightElemType);
    if (left == right) {
        std::set<std::pair<ProgramElement*, ProgramElement*>> resultPairs;
        for (const auto& p : allPairs) {
            if (p.first == p.second) {
                resultPairs.insert(p);
            }
        }
        return resultPairs;
    } else {
        return allPairs;
    }
}
