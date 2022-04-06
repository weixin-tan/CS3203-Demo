#include "WithHandler.h"
WithHandler::WithHandler(PkbGetter* pg) {
    WithHandler::pg = pg;
}

Result WithHandler::handleWith(const RelationshipRef& relRef) {
    Result result;
    std::set<ProgramElement> oneSynSet;
    std::set<std::pair<ProgramElement, ProgramElement>> twoSynSet;

    Entity left = relRef.leftEntity;
    Entity right = relRef.rightEntity;
    result.setResultType(ResultType::WITH_CLAUSE);

    if (isFixedEntity(left) && isFixedEntity(right)) {
        result.setValid(handleBothSidesFixed(left, right));
        return result;
    } else if (isFixedEntity(left) && !isFixedEntity(right)) {
        oneSynSet = handleOneSideFixed(left, right);
        right.aType = EntityAttributeType::NULL_ATTRIBUTE;
        result.setOneSynEntity(right);
        result.setOneSynSet(oneSynSet);
    } else if (!isFixedEntity(left) && isFixedEntity(right)) {
        oneSynSet = handleOneSideFixed(right, left);
        left.aType = EntityAttributeType::NULL_ATTRIBUTE;
        result.setOneSynEntity(left);
        result.setOneSynSet(oneSynSet);
    } else {
        twoSynSet = handleNoSideFixed(left, right);
        left.aType = EntityAttributeType::NULL_ATTRIBUTE;
        right.aType = EntityAttributeType::NULL_ATTRIBUTE;
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

bool WithHandler::isFixedEntity(const Entity& entity) {
    return entity.eType == EntityType::FIXED_STRING || entity.eType == EntityType::FIXED_INTEGER;
}

std::set<ProgramElement> WithHandler::getProgramElements(const EntityType& eType) {
    ElementType elementType = QpsTypeToPkbTypeConvertor::convertToPkbElement(eType);
    return pg->getEntity(elementType);
}

std::set<ProgramElement> WithHandler::matchProgramElements(const std::set<ProgramElement>& setA,
                                                           const Entity& fixedEntity,
                                                           EntityAttributeType aType) {
    std::set<ProgramElement> toReturn;
    std::string stringA;
    std::string stringB = fixedEntity.name;
    for (const auto& element : setA) {
        stringA = element.toString(aType);
        if (stringA == stringB) {
            toReturn.insert(element);
        }
    }
    return toReturn;
}

void WithHandler::insertIfSameAttributeString(const std::string& stringA, const std::string& stringB,
                                              const ProgramElement& elementA, const ProgramElement& elementB,
                                              std::set<std::pair<ProgramElement, ProgramElement>>* toReturn) {
    if (stringA == stringB) {
        std::pair<ProgramElement, ProgramElement> tempPair(elementA, elementB);
        (*toReturn).insert(tempPair);
    }
}

std::set<std::pair<ProgramElement,
                   ProgramElement>> WithHandler::matchProgramElements(const std::set<ProgramElement>& setA,
                                                                      const std::set<ProgramElement>& setB,
                                                                      EntityAttributeType aType,
                                                                      EntityAttributeType bType) {
    std::set<std::pair<ProgramElement, ProgramElement>> toReturn;
    std::string stringA;
    std::string stringB;
    for (const auto& elementA : setA) {
        stringA = elementA.toString(aType);
        for (const auto& elementB : setB) {
            stringB = elementB.toString(bType);
            insertIfSameAttributeString(stringA, stringB, elementA, elementB, &toReturn);
        }
    }
    return toReturn;
}

bool WithHandler::handleBothSidesFixed(const Entity& leftEntity, const Entity& rightEntity) {
    return leftEntity == rightEntity;
}

std::set<ProgramElement> WithHandler::handleOneSideFixed(const Entity& fixedSide, const Entity& notFixedSide) {
    std::set<ProgramElement> programElementsSet = getProgramElements(notFixedSide.eType);
    return matchProgramElements(programElementsSet, fixedSide, notFixedSide.aType);
}

std::set<std::pair<ProgramElement, ProgramElement>> WithHandler::handleNoSideFixed(const Entity& leftEntity,
                                                                                   const Entity& rightEntity) {
    std::set<ProgramElement> setA = getProgramElements(leftEntity.eType);
    std::set<ProgramElement> setB = getProgramElements(rightEntity.eType);

    return matchProgramElements(setA, setB, leftEntity.aType, rightEntity.aType);
}






