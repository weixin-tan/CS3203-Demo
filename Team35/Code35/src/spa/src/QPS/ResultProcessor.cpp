#include "ResultProcessor.h"
#include "Table.h"

FormattedResult ResultProcessor::processResults(const ResultGroup& resultGroup) {
    std::vector<std::vector<Result>> resultLists = resultGroup.getResultLists();

    FormattedResult emptyFormattedResult;
    if (resultGroup.getIsBoolReturn()) {
        emptyFormattedResult.setBoolReturn(true);
    }
    if (!resultGroup.getIsValid()) {
        return emptyFormattedResult;
    }
    std::vector<Result> noClauseResults = resultLists[0];
    if (resultLists.size() == 1) {
        return handleZeroClause(resultGroup.getEntitiesToReturn(), noClauseResults);
    }
    resultLists.erase(resultLists.begin());

    std::vector<Table> intermediateTables = buildAllIntermediateTables(resultLists);
    if (intermediateTables.empty()) {
        return emptyFormattedResult;
    }
    if (resultGroup.getIsBoolReturn()) {
        emptyFormattedResult.setValid(true);
        return emptyFormattedResult;
    }

    std::vector<Entity> entitiesToReturn = resultGroup.getEntitiesToReturn();
    std::set<Entity> tableEntities = extractEntitySet(entitiesToReturn);
    std::vector<Table> necessaryTables = extractNecessaryTables(intermediateTables, tableEntities);
    if (necessaryTables.empty()) {
        return handleZeroClause(entitiesToReturn, noClauseResults);
    }

    Table finalTable = buildFinalTable(necessaryTables);
    TableRow firstRow = *finalTable.rows.begin();
    std::set<Entity> finalTableEntities = extractTableEntities(firstRow);

    if (tableEntities.size() != finalTableEntities.size()) {
        std::set<Entity> missingEntities = findMissingEntities(tableEntities, finalTableEntities);
        std::vector<Result> resultsOfMissingEntities = findMissingResults(missingEntities, noClauseResults);
        for (const auto& r : resultsOfMissingEntities) {
            finalTable = Table(&finalTable, &Table(&r));
        }
    }
    emptyFormattedResult.setValid(true);
    emptyFormattedResult.setEntityList(entitiesToReturn);
    emptyFormattedResult.setFinalTable(finalTable);
    return emptyFormattedResult;
}

FormattedResult ResultProcessor::handleZeroClause(const std::vector<Entity>& entities, const std::vector<Result>& resultList) {
    FormattedResult formattedResult;
    formattedResult.setValid(true);
    std::vector<Result> scrubbedResultList = scrubResultList(resultList);

    if (entities[0].eType == EntityType::BOOLEAN) {
        formattedResult.setBoolReturn(true);
        return formattedResult;
    }

    formattedResult.setEntityList(entities);
    Table table = Table(&scrubbedResultList[0]);
    if (scrubbedResultList.size() != 1) {
        for (int i = 1; i < entities.size(); i++) {
            table = Table(&table, &Table(&scrubbedResultList[i]));
        }
    }
    formattedResult.setFinalTable(table);
    return formattedResult;
}

std::vector<Result> ResultProcessor::scrubResultList(const std::vector<Result>& resultList) {
    std::vector<Result> scrubbedResults;
    for (const auto& result : resultList) {
        Result scrubbedResult = result;
        Entity scrubbedEntity = result.getOneSynEntity();
        scrubbedEntity.aType = EntityAttributeType::NULL_ATTRIBUTE;
        scrubbedResult.setOneSynEntity(scrubbedEntity);
        scrubbedResults.push_back(scrubbedResult);
    }
    return scrubbedResults;
}

Table ResultProcessor::buildIntermediateTable(const std::vector<Result>& results) {
    Table intermediateTable = Table(&results[0]);

    if (results.size() == 1) {
        return intermediateTable;
    }

    for (int i = 1; i < results.size(); i++) {
        intermediateTable = Table(&intermediateTable, &Table(&results[i]));
        if (intermediateTable.rows.empty()) {
            break;
        }
    }
    return intermediateTable;
}

std::vector<Table> ResultProcessor::buildAllIntermediateTables(const std::vector<std::vector<Result>>& resultLists) {
    std::vector<Table> intermediateTables;
    for (const auto& resultList : resultLists) {
        Table intermediateTable = buildIntermediateTable(resultList);
        if (intermediateTable.rows.empty()) {
            return {};
        }
        intermediateTables.push_back(intermediateTable);
    }
    return intermediateTables;
}

std::set<Entity> ResultProcessor::extractEntitySet(const std::vector<Entity>& entityList) {
    std::set<Entity> entitySet;
    for (const auto& entity : entityList) {
        Entity scrubbedEntity = entity;
        scrubbedEntity.aType = EntityAttributeType::NULL_ATTRIBUTE;
        entitySet.insert(scrubbedEntity);
    }
    return entitySet;
}

std::vector<Entity> ResultProcessor::getNecessaryEntities(const std::set<Entity>& tableEntities, const TableRow& row) {
    std::vector<Entity> entities;
    for (const auto& r : row.row) {
        if (tableEntities.find(r.first) != tableEntities.end()) {
            entities.push_back(r.first);
        }
    }
    return entities;
}

std::vector<Table> ResultProcessor::extractNecessaryTables(const std::vector<Table>& intermediateTables,
                                                           const std::set<Entity>& tableEntities) {
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

Table ResultProcessor::buildFinalTable(const std::vector<Table>& tables) {
    Table finalTable = tables[0];

    if (tables.size() != 1) {
        for (int i = 1; i < tables.size(); i++) {
            finalTable = Table(&finalTable, &tables[i]);
        }
    }
    return finalTable;
}

std::set<Entity> ResultProcessor::extractTableEntities(const TableRow& tableRow) {
    std::set<Entity> entities;
    for (const auto& r : tableRow.row) {
        entities.insert(r.first);
    }
    return entities;
}

std::set<Entity> ResultProcessor::findMissingEntities(const std::set<Entity>& returnEntities,
                                                      const std::set<Entity>& finalTableEntities) {
    std::set<Entity> missingEntities;
    for (const auto& e : returnEntities) {
        if (finalTableEntities.find(e) == finalTableEntities.end()) {
            missingEntities.insert(e);
        }
    }
    return missingEntities;
}

std::vector<Result> ResultProcessor::findMissingResults(const std::set<Entity>& entities,
                                                        const std::vector<Result>& results) {
    std::vector<Result> missingResults;
    for (const auto& r : results) {
        Entity scrubbedEntity = r.getOneSynEntity();
        scrubbedEntity.aType = EntityAttributeType::NULL_ATTRIBUTE;
        if (entities.find(scrubbedEntity) != entities.end()) {
            Result scrubbedResult = r;
            scrubbedResult.setOneSynEntity(scrubbedEntity);
            missingResults.push_back(scrubbedResult);
        }
    }
    return missingResults;
}
