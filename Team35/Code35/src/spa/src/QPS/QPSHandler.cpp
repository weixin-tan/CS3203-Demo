#include "QPSHandler.h"
#include "FormattedResult.h"

QPSHandler::QPSHandler(PkbGetter* pg) {
    QPSHandler::pg = pg;
    QPSHandler::patternHandler = new PatternHandler(pg);
    QPSHandler::suchThatHandler = new SuchThatHandler(pg);
    QPSHandler::withHandler = new WithHandler(pg);
}

bool isFixedEntityGroup(const RelationshipRef& r) {
    return (r.leftEntity.eType == EntityType::FIXED_INTEGER || r.leftEntity.eType == EntityType::FIXED_STRING ||
            r.leftEntity.eType == EntityType::FIXED_STRING_WITHIN_WILDCARD ||
            r.leftEntity.eType == EntityType::WILDCARD)
           &&
           (r.rightEntity.eType == EntityType::FIXED_INTEGER || r.rightEntity.eType == EntityType::FIXED_STRING ||
            r.rightEntity.eType == EntityType::FIXED_STRING_WITHIN_WILDCARD ||
            r.rightEntity.eType == EntityType::WILDCARD)
           &&
           (r.rType != RelationshipType::PATTERN);
}

FormattedResult QPSHandler::processClause(const GroupedClause& groupedClause) const {
    FormattedResult emptyFormattedResult;
    FormattedResult finalFormattedResult;
    finalFormattedResult.setValid(true);

    if (groupedClause.entityToFindList[0].eType == EntityType::BOOLEAN) {
        emptyFormattedResult.setBoolReturn(true);
        finalFormattedResult.setBoolReturn(true);
    }

    if (groupedClause.relRefGroups.empty()) {
        return handleZeroClause(groupedClause.entityToFindList);
    }

    finalFormattedResult.setEntityList(groupedClause.entityToFindList);
    std::vector<Table> intermediateTables;

    if (isFixedEntityGroup(groupedClause.relRefGroups[0].relRefGroup[0])) {
        if(!handleFixedEntityGroup(groupedClause.relRefGroups[0])) {
            return emptyFormattedResult;
        }
    }

    for (const auto& group: groupedClause.relRefGroups){
        if (isFixedEntityGroup(group.relRefGroup[0])) {
            continue;
        }
        Table intermediateTable = handleRelRefGroups(group);
        if (intermediateTable.rows.empty()) {
            return emptyFormattedResult;
        }
        intermediateTables.push_back(intermediateTable);
    }

    if (finalFormattedResult.getBoolReturn()) {
        return finalFormattedResult;
    }

    std::set<Entity> tableEntities = extractEntitySet(groupedClause.entityToFindList);
    std::vector<Table> necessaryTables = extractNecessaryTables(intermediateTables, tableEntities);
    if (necessaryTables.empty()) {
        return handleZeroClause(groupedClause.entityToFindList);
    }
    Table finalTable = buildFinalTable(necessaryTables);
    TableRow firstRow = *finalTable.rows.begin();
    std::set<Entity> finalTableEntities = extractTableEntities(firstRow);

    if (tableEntities.size() != finalTableEntities.size()) {
        std::set<Entity> missingEntities = findMissingEntities(tableEntities, finalTableEntities);
        std::vector<Result> resultsOfMissingEntities = findMissingResults(missingEntities);
        for (const auto& r : resultsOfMissingEntities) {
            const Table tempTable = Table(&r);
            finalTable = Table(&finalTable, &tempTable);
        }
    }

    finalFormattedResult.setFinalTable(finalTable);
    return finalFormattedResult;
}

bool QPSHandler::handleFixedEntityGroup(const RelationshipRefGroup &group) const {
    for (const auto& r : group.relRefGroup) {
        Result tempResult = getResult(r);
        if (!tempResult.getValid()) {
            return false;
        }
    }
    return true;
}

FormattedResult QPSHandler::handleZeroClause(const std::vector<Entity>& entitiesToFind) const {
    FormattedResult formattedResult;
    formattedResult.setValid(true);

    if (entitiesToFind[0].eType == EntityType::BOOLEAN) {
        formattedResult.setBoolReturn(true);
        return formattedResult;
    }
    
    formattedResult.setEntityList(entitiesToFind);

    std::set<Entity> tableEntities = extractEntitySet(entitiesToFind);
    Table finalTable = Table();
    for (const auto& e : tableEntities) {
        Result result = getAllElements(e);
        Table tempTable = Table(&result);
        if (finalTable.rows.empty()) {
            finalTable = tempTable;
        } else {
            finalTable = Table(&finalTable, &tempTable);
        }
    }
    formattedResult.setFinalTable(finalTable);
    return formattedResult;
}

Result QPSHandler::getAllElements(const Entity& e) const {
    ElementType elemType = QpsTypeToPkbTypeConvertor::convertToPkbElement(e.eType);
    std::set<ProgramElement*> elements = pg->getEntity(elemType);
    Result result;
    result.setValid(true);
    result.setOneSynEntity(e);
    result.setOneSynSet(elements);
    return result;
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
        std::set<ProgramElement*> oneSyn = pg->getEntity(elementTypeToGet);
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

Table QPSHandler::handleRelRefGroups(const RelationshipRefGroup& relRefGroup) const {
    Table intermediateTable;
    for (const auto& r : relRefGroup.relRefGroup) {
        Result tempResult = getResult(r);
        if (!tempResult.getValid()) {
            return {};
        }
        Table tempTable = Table(&tempResult);
        if (intermediateTable.rows.empty()) {
            intermediateTable = tempTable;
        } else {
            intermediateTable = Table(&intermediateTable, &tempTable);
        }
        if (intermediateTable.rows.empty()) {
            return {};
        }
    }
    return intermediateTable;
}

Result QPSHandler::getResult(const RelationshipRef& r) const {
    Result tempResult;
    if (r.rType == RelationshipType::PATTERN) {
        tempResult = patternHandler->handlePattern(r);
    } else if (r.rType == RelationshipType::WITH) {
        tempResult = withHandler->handleWith(r);
    } else {
        tempResult = suchThatHandler->handleSuchThat(r);
    }
    return tempResult;
}

std::set<Entity> QPSHandler::extractEntitySet(const std::vector<Entity>& entityList) const {
    std::set<Entity> entitySet;
    for (const auto& entity : entityList) {
        Entity scrubbedEntity = entity;
        scrubbedEntity.aType = EntityAttributeType::NULL_ATTRIBUTE;
        entitySet.insert(scrubbedEntity);
    }
    return entitySet;
}

std::vector<Entity> QPSHandler::getNecessaryEntities(const std::set<Entity>& tableEntities, const TableRow& row) const {
    std::vector<Entity> entities;
    for (const auto& r : row.row) {
        if (tableEntities.find(r.first) != tableEntities.end()) {
            entities.push_back(r.first);
        }
    }
    return entities;
}

std::vector<Table> QPSHandler::extractNecessaryTables(const std::vector<Table>& intermediateTables,
                                                      const std::set<Entity>& tableEntities) const {
    std::vector<Table> necessaryTables;
    for (Table table : intermediateTables) {
        TableRow firstRow = *table.rows.begin();
        std::vector<Entity> entities = getNecessaryEntities(tableEntities, firstRow);
        if (!entities.empty()) {
            necessaryTables.push_back(table.extractColumns(&entities));
        }
    }
    return necessaryTables;
}

Table QPSHandler::buildFinalTable(const std::vector<Table>& tables) const {
    Table finalTable = tables[0];

    if (tables.size() != 1) {
        for (int i = 1; i < tables.size(); i++) {
            finalTable = Table(&finalTable, &tables[i]);
        }
    }
    return finalTable;
}

std::set<Entity> QPSHandler::extractTableEntities(const TableRow& tableRow) const{
    std::set<Entity> entities;
    for (const auto& r : tableRow.row) {
        entities.insert(r.first);
    }
    return entities;
}

std::set<Entity> QPSHandler::findMissingEntities(const std::set<Entity>& returnEntities,
                                                 const std::set<Entity>& finalTableEntities) const {
    std::set<Entity> missingEntities;
    for (const auto& e : returnEntities) {
        if (finalTableEntities.find(e) == finalTableEntities.end()) {
            missingEntities.insert(e);
        }
    }
    return missingEntities;
}

std::vector<Result> QPSHandler::findMissingResults(const std::set<Entity>& entities) const {
    std::vector<Result> missingResults;
    missingResults.reserve(entities.size());
    for (const auto& e : entities) {
        missingResults.push_back(getAllElements(e));
    }
    return missingResults;
}
