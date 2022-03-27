#include "ResultProcessor.h"
#include "Table.h"

std::vector<ProgramElement> ResultProcessor::processResults(std::vector<ResultGroup> groups) {
    ResultGroup noClauseGroups = groups[0];

    for (const auto& result : noClauseGroups.getGroup()) {
        if (!result.getValid()) {
            return {};
        }
    }

    //For future improvement, to avoid having to merge all the no clause elements
    //groups.erase(groups.begin());

    std::vector<Table> intermediateTables;
    for (const auto& group : groups) {
        std::vector<Result> results = group.getGroup();
        Table intermediateTable = buildIntermediateTable(results);
        if (intermediateTable.rows.empty()) {
            return {};
        }
        intermediateTables.push_back(intermediateTable);
    }

    Table finalTable = buildFinalTable(intermediateTables);
    if (finalTable.rows.empty()) {
        return {};
    }

    std::vector<ProgramElement> finalResult;
    for (const auto& row : finalTable.rows)
        finalResult.push_back(row.row.at(noClauseGroups.getGroup()[0].getOneSynEntity()));

    return finalResult;
}

Table ResultProcessor::buildIntermediateTable(std::vector<Result> results) {
    Table intermediateTable = Table(results[0]);
    results.erase(results.begin());

    if (!results.empty()) {
        for (const auto& result : results) {
            if (result.getValid() && result.getOneSynSet().empty() && result.getTwoSynSet().empty()) {
                continue;
            }
            intermediateTable = Table(intermediateTable, Table(result));
            if (intermediateTable.rows.empty()) {
                break;
            }
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
            if (finalTable.rows.empty()) {
                break;
            }
        }
    }
    return finalTable;
}

