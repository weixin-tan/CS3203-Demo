#include "QPSHandler.h"

QPSHandler::QPSHandler(PkbGetter* pg) {
    QPSHandler::pg = pg;
    QPSHandler::patternHandler = new PatternHandler(pg);
    QPSHandler::suchThatHandler = new SuchThatHandler(pg);
    QPSHandler::withHandler = new WithHandler(pg);
}

ResultGroup QPSHandler::processClause(const GroupedClause& groupedClause) const {
    ResultGroup emptyGroup;
    ResultGroup resultGroup;
    resultGroup.setValid(true);

    std::vector<Result> selectResultList;
    if (groupedClause.entityToFindList[0].eType == EntityType::BOOLEAN) {
        emptyGroup.setBoolReturn(true);
        resultGroup.setBoolReturn(true);
        Result boolResult;
        boolResult.setValid(true);
        boolResult.setOneSynEntity(groupedClause.entityToFindList[0]);
        selectResultList.push_back(boolResult);
    } else {
        selectResultList = getNoClauseResults(groupedClause.entityToFindList);
        if (selectResultList.empty()) {
            return emptyGroup;
        }
    }
    resultGroup.addResultList(selectResultList);

    if (groupedClause.relRefGroups.empty()) {
        resultGroup.setEntitiesToReturn(groupedClause.entityToFindList);
        return resultGroup;
    }
    for (const auto& group: groupedClause.relRefGroups){
        std::vector<Result> results = handleRelRefGroups(group);
        if (results.empty()) {
            return emptyGroup;
        }
        if (results[0].getOneSynSet().empty() && results[0].getTwoSynSet().empty()) {
            continue;
        }
        resultGroup.addResultList(results);
    }
    resultGroup.setEntitiesToReturn(groupedClause.entityToFindList);
    return resultGroup;
}

std::vector<Result> QPSHandler::getNoClauseResults(const std::vector<Entity>& entitiesToFind) const {
    std::vector<Result> results;
    std::set<Entity> scrubbedEntities;
    for (const auto& entityToFind : entitiesToFind) {
        Entity scrubbedEntity = entityToFind;
        scrubbedEntity.aType = EntityAttributeType::NULL_ATTRIBUTE;
        if (scrubbedEntities.find(scrubbedEntity) != scrubbedEntities.end()) {
            continue;
        } else {
            scrubbedEntities.insert(scrubbedEntity);
        }
        Result result;
        result.setResultType(ResultType::NO_CLAUSE);
        ElementType elementTypeToGet = QpsTypeToPkbTypeConvertor::convertToPkbElement(entityToFind.eType);
        std::set<ProgramElement> oneSyn = pg->getEntity(elementTypeToGet);
        if (oneSyn.empty()) {
            return {};
        }
        result.setValid(true);
        result.setOneSynEntity(entityToFind);
        result.setOneSynSet(oneSyn);
        results.push_back(result);
    }
    return results;
}

std::vector<Result> QPSHandler::handleRelRefGroups(const RelationshipRefGroup& relRefGroup) const {
    std::vector<Result> results;
    for (const auto& r : relRefGroup.relRefGroup) {
        Result tempResult;
        if (r.rType == RelationshipType::PATTERN) {
            tempResult = patternHandler->handlePattern(r);
        } else if (r.rType == RelationshipType::WITH) {
            tempResult = withHandler->handleWith(r);
        } else {
            tempResult = suchThatHandler->handleSuchThat(r);
        }
        if (!tempResult.getValid()) {
            return {};
        }
        results.push_back(tempResult);
    }
    return results;
}
