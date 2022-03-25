#include "ResultProcessor.h"
#include "Table.h"
#include <set>

std::vector<ProgramElement> ResultProcessor::processResults(std::vector<ResultGroup> groups) {
    std::vector<ProgramElement> finalResult;
    Entity targetEntity = groups[0].getGroup()[0].getOneSynEntity();
    Result initialResult = groups[0].getGroup()[0];
    Table t(initialResult);
    if (groups.size() > 1)
        for (const auto& result : groups[1].getGroup())
            t = Table(t, Table(result));
    for (const auto& row : t.rows)
        finalResult.push_back(row.row.at(targetEntity));
    return finalResult;
}




