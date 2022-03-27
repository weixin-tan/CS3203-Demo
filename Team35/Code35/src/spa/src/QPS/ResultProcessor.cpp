#include "ResultProcessor.h"
#include "Table.h"

FormattedResult ResultProcessor::processResults(std::vector<ResultGroup> groups) {
    FormattedResult result;
    std::vector<Result> noClauseResults = groups[0].getGroup();

    for (const auto& r : noClauseResults) {
        if (!r.getValid()) {
            return handleInvalidResult(r);
        }
    }

    groups.erase(groups.begin());
    if (groups.empty()) {
        return handleZeroClause(noClauseResults);
    }

    std::vector<Table> intermediateTables;
    for (const auto& group : groups) {
        std::vector<Result> results = group.getGroup();
        Table intermediateTable = buildIntermediateTable(results);
        if (intermediateTable.rows.empty()) {
            intermediateTables = {};
            break;
        }
        intermediateTables.push_back(intermediateTable);
    }
    if (intermediateTables.empty()) {
        return handleInvalidResult(noClauseResults[0]);
    }

    std::set<Entity> entitiesToReturn = extractEntities(noClauseResults);
    std::vector<Table> necessaryTables;
    for (const auto& table : intermediateTables) {
        TableRow firstRow = *table.rows.begin();
        std::vector<Entity> entities;
        for (const auto& r : firstRow.row) {
            if (entitiesToReturn.find(r.first) != entitiesToReturn.end()) {
                entities.push_back(r.first);
            }
        }
        necessaryTables.push_back(table.extractColumns(entities));
    }

    if (necessaryTables.empty()) {
        return handleZeroClause(noClauseResults);
    } else {
        if (entitiesToReturn.begin()->eType == EntityType::BOOLEAN) {
            result.setValid(true);
            result.setResultType(FormattedResultType::BOOLEAN);
            return result;
        }
        Table finalTable = buildFinalTable(necessaryTables);
        std::vector<Entity> finalTableEntities = extractTableEntities(finalTable);
        if (finalTableEntities.size() == entitiesToReturn.size()) {
            return extractTableInformation(finalTable);
        }
        std::set<Entity> missingEntities;
        for (const auto& e : finalTableEntities) {
            if (entitiesToReturn.find(e) == entitiesToReturn.end()) {
                missingEntities.insert(e);
            }
        }
        std::vector<Result> missingResults;
        for (const auto& r : noClauseResults) {
            if (missingEntities.find(r.getOneSynEntity()) != missingEntities.end()) {
                missingResults.push_back(r);
            }
        }
        for (const auto& r : missingResults) {
            finalTable = Table(finalTable, Table(r));
        }
        return extractTableInformation(finalTable);
    }
}

Table ResultProcessor::buildIntermediateTable(std::vector<Result> results) {
    Table intermediateTable = Table(results[0]);
    results.erase(results.begin());

    if (results.empty()) {
        return intermediateTable;
    }

    for (const auto& result : results) {
        if (result.getValid() && result.getOneSynSet().empty() && result.getTwoSynSet().empty()) {
            continue;
        }
        intermediateTable = Table(intermediateTable, Table(result));
        if (intermediateTable.rows.empty()) {
            break;
        }
    }
    return intermediateTable;
}

Table ResultProcessor::buildFinalTable(std::vector<Table> tables) {
    Table finalTable = tables[0];
    tables.erase(tables.begin());

    if (!tables.empty()) {
        for (const auto& table : tables) {
            finalTable = Table(finalTable, table);
        }
    }
    return finalTable;
}

std::set<Entity> ResultProcessor::extractEntities(std::vector<Result> resultList) {
    std::set<Entity> entityList;
    for (const auto& r : resultList) {
        entityList.insert(r.getOneSynEntity());
    }
    return entityList;
}

FormattedResult ResultProcessor::handleInvalidResult(Result r) {
    FormattedResult formattedResult;
    if (r.getOneSynEntity().eType == EntityType::BOOLEAN) {
        formattedResult.setResultType(FormattedResultType::BOOLEAN);
    }
    return formattedResult;
}

FormattedResult ResultProcessor::handleZeroClause(std::vector<Result> resultList) {
    std::vector<std::vector<ProgramElement>> returnElements;
    FormattedResult formattedResult;
    formattedResult.setValid(true);

    Result firstResult = resultList[0];
    if (firstResult.getOneSynEntity().eType == EntityType::BOOLEAN) {
        formattedResult.setResultType(FormattedResultType::BOOLEAN);
        return formattedResult;
    }

    Table table = Table(resultList[0]);
    resultList.erase(resultList.begin());
    if (!resultList.empty()) {
        for (const auto& r : resultList) {
            table = Table(table, Table(r));
        }
    }
    return extractTableInformation(table);
}

FormattedResult ResultProcessor::extractTableInformation(Table table) {
    std::vector<Entity> entities = extractTableEntities(table);

    std::vector<std::vector<ProgramElement>> programElementLists;
    for (const auto& row : table.rows) {
        for (int i = 0; i < entities.size(); i++) {
            programElementLists[i].push_back(row.row.at(entities[i]));
        }
    }

    FormattedResult formattedResult;
    formattedResult.setValid(true);
    if (entities.size() == 1) {
        formattedResult.setResultType(FormattedResultType::SINGLE);
    } else {
        formattedResult.setResultType(FormattedResultType::TUPLE);
    }
    formattedResult.setEntityList(entities);
    formattedResult.setProgramElementsLists(programElementLists);
    return formattedResult;
}

std::vector<Entity> ResultProcessor::extractTableEntities(Table table) {
    std::vector<Entity> entities;
    TableRow firstRow = *table.rows.begin();
    for (const auto& r : firstRow.row) {
        entities.push_back(r.first);
    }
    return entities;
}
