#include "QpsTypeToPkbTypeConvertor.h"
#include "SuchThatHandler.h"

//Constructor
SuchThatHandler::SuchThatHandler(PkbGetter *pg) {
    SuchThatHandler::pg = pg;
}

//Handles Such That relationships
Result SuchThatHandler::handleSuchThat(const RelationshipRef& relRef) {
    Result result;
    std::set<ProgramElement> oneSynSet;
    std::set<std::pair<ProgramElement, ProgramElement>> twoSynSet;

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
    return e == EntityType::Wildcard || e == EntityType::FixedString || e == EntityType::FixedInteger;
}

// Handles cases where there are no synonyms on either side.
bool SuchThatHandler::handleNoSynonyms(PkbRelationshipType r, Entity left, Entity right) {
    // If there are wildcards present, then move to wildcard handlers based on relationship type
    if (left.eType == EntityType::Wildcard || right.eType == EntityType::Wildcard) {
        if (r == PkbRelationshipType::CALLS || r == PkbRelationshipType::CALLS_T) {
            return handleNoSynProcWildcard(r, left, right);
        } else if (r == PkbRelationshipType::MODIFIES || r == PkbRelationshipType::USES) {
            return handleNoSynVarWildcard(r, left);
        } else {
            return handleNoSynStmtWildcard(r, left, right);
        }
    }

    // If both sides are fixed entities, then check pkb if the relationship exists
    if (left.eType == EntityType::FixedString && right.eType == EntityType::FixedString) {
        return pg->isRelationship(r,
                                  ProgramElement::createProcedure(left.name),
                                  ProgramElement::createProcedure(left.name));
    } else if (left.eType == EntityType::FixedInteger && right.eType == EntityType::FixedInteger) {
        return pg->isRelationship(r,
                                  ProgramElement::createStatement(ElementType::STATEMENT,
                                                                         std::stoi(left.name)),
                                  ProgramElement::createStatement(ElementType::STATEMENT,
                                                                          std::stoi(right.name)));
    } else if (left.eType == EntityType::FixedInteger && right.eType == EntityType::FixedString) {
        return pg->isRelationship(r,
                                  ProgramElement::createStatement(ElementType::STATEMENT,
                                                                         std::stoi(left.name)),
                                  ProgramElement::createVariable(right.name));
    } else if (left.eType == EntityType::FixedString && right.eType == EntityType::FixedString) {
        return pg->isRelationship(r,
                                  ProgramElement::createProcedure(left.name),
                                  ProgramElement::createVariable(right.name));
    } else {
        return false;
    }
}

// Handles cases where there are wildcards in Calls/CallsT
bool SuchThatHandler::handleNoSynProcWildcard(PkbRelationshipType r, Entity left, Entity right) {
    bool valid = false;

    if (left.eType == EntityType::Wildcard && right.eType == EntityType::Wildcard) {
        std::set<std::pair<ProgramElement, ProgramElement>> check = pg->getRelationshipPairs(r,
                                                                                             ElementType::PROCEDURE,
                                                                                             ElementType::PROCEDURE);
        if (!check.empty()) {
            valid = true;
        }
    } else if (left.eType == EntityType::Wildcard) {
        std::set<ProgramElement> check = pg->getLeftSide(r,
                                                         ProgramElement::createProcedure(right.name),
                                                         ElementType::PROCEDURE);
        if (!check.empty()) {
            valid = true;
        }
    } else {
        std::set<ProgramElement> check = pg->getRightSide(r,
                                                          ProgramElement::createProcedure(left.name),
                                                          ElementType::PROCEDURE);
        if (!check.empty()) {
            valid = true;
        }
    }

    return valid;
}

// Handles cases where there are wildcards in Follows/FollowsT/Parent/ParentT/Next/NextT/Affects/AffectsT
bool SuchThatHandler::handleNoSynStmtWildcard(PkbRelationshipType r, Entity left, Entity right) {
    bool valid = false;

    if (left.eType == EntityType::Wildcard && right.eType == EntityType::Wildcard) {
        std::set<std::pair<ProgramElement, ProgramElement>> check = pg->getRelationshipPairs(r,
                                                                                             ElementType::STATEMENT,
                                                                                             ElementType::STATEMENT);
        if (!check.empty()) {
            valid = true;
        }
    } else if (left.eType == EntityType::Wildcard) {
        std::set<ProgramElement> check = pg->getLeftSide(r,
                                                         ProgramElement::createStatement(ElementType::STATEMENT,
                                                                                                 std::stoi(right.name)),
                                                         ElementType::STATEMENT);
        if (!check.empty()) {
            valid = true;
        }
    } else {
        std::set<ProgramElement> check = pg->getRightSide(r,
                                                          ProgramElement::createStatement(ElementType::STATEMENT,
                                                                                                 std::stoi(left.name)),
                                                          ElementType::STATEMENT);
        if (!check.empty()) {
            valid = true;
        }
    }

    return valid;
}

// Handles cases where there are wildcards in Modifies/Uses
bool SuchThatHandler::handleNoSynVarWildcard(PkbRelationshipType r, Entity left) {
    bool valid = false;

    if (left.eType == EntityType::FixedString) {
        std::set<ProgramElement> check = pg->getRightSide(r,
                                                          ProgramElement::createProcedure(left.name),
                                                          ElementType::VARIABLE);
        if (!check.empty()) {
            valid = true;
        }
    }
    if (left.eType == EntityType::FixedInteger) {
        std::set<ProgramElement> check = pg->getRightSide(r,
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
std::set<ProgramElement> SuchThatHandler::handleRightSyn(PkbRelationshipType r, Entity left, Entity right) {
    std::set<ProgramElement> oneSynSet;
    ElementType rightElemType = QpsTypeToPkbTypeConvertor::convertToPkbElement(right.eType);

    if (left.eType == EntityType::FixedString) {
        oneSynSet = pg->getRightSide(r,
                                     ProgramElement::createProcedure(left.name),
                                     rightElemType);
    } else if (left.eType == EntityType::FixedInteger) {
        oneSynSet = pg->getRightSide(r,
                                     ProgramElement::createStatement(ElementType::STATEMENT, std::stoi(left.name)),
                                     rightElemType);
    } else { // Handles cases where left side is a wildcard
        std::set<std::pair<ProgramElement, ProgramElement>> pairs = pg->getRelationshipPairs(r,
                                                                                             ElementType::STATEMENT,
                                                                                             rightElemType);
        for (const auto& p : pairs) {
            oneSynSet.insert(p.second);
        }
    }

    return oneSynSet;
}

// Handles cases where there is a synonym on left side
std::set<ProgramElement> SuchThatHandler::handleLeftSyn(PkbRelationshipType r, Entity left, Entity right) {
    std::set<ProgramElement> oneSynSet;
    ElementType leftElemType = QpsTypeToPkbTypeConvertor::convertToPkbElement(left.eType);

    if (right.eType == EntityType::FixedInteger) {
        oneSynSet = pg->getLeftSide(r,
                                    ProgramElement::createStatement(ElementType::STATEMENT, std::stoi(right.name)),
                                    leftElemType);
    } else if (right.eType == EntityType::FixedString) {
        if (r == PkbRelationshipType::MODIFIES || r == PkbRelationshipType::USES) {
            oneSynSet = pg->getLeftSide(r, ProgramElement::createVariable(right.name), leftElemType);
        } else {
            oneSynSet = pg->getLeftSide(r, ProgramElement::createProcedure(right.name), leftElemType);
        }
    } else {
        std::set<std::pair<ProgramElement, ProgramElement>> pairs;
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
std::set<std::pair<ProgramElement, ProgramElement>>
SuchThatHandler::handleTwoSyn(PkbRelationshipType r, Entity left, Entity right) {
    ElementType leftElemType = QpsTypeToPkbTypeConvertor::convertToPkbElement(left.eType);
    ElementType rightElemType = QpsTypeToPkbTypeConvertor::convertToPkbElement(right.eType);
    return pg->getRelationshipPairs(r, leftElemType, rightElemType);
}
