#include "QPSHandler.h"


QPSHandler::QPSHandler(PkbGetter* pg) {
    QPSHandler::pg = pg;
    QPSHandler::patternHandler = new PatternHandler(pg);
    QPSHandler::suchThatHandler = new SuchThatHandler(pg);
    QPSHandler::withHandler = new WithHandler(pg);
}

std::vector<ResultGroup> QPSHandler::processClause(const GroupedClause& groupedClause) const {
    std::vector<ResultGroup> resultGroups;

    //variables to select in the first group
    ResultGroup selectGroup = ResultGroup();
    std::vector<Result> selectResultList;
    Result noClauseResult;
    for (const auto& entityToFind: groupedClause.entityToFindList){
        noClauseResult = getNoClauseResult(entityToFind);
        selectResultList.push_back(noClauseResult);
    }
    selectGroup.setGroup(selectResultList);
    resultGroups.push_back(selectGroup);

    ResultGroup tempGroup;
    Result tempResult;
    for (const auto& group: groupedClause.relRefGroups){
        std::vector<Result> results;
        tempGroup = ResultGroup();
        for (const auto& r : group.relRefGroup) {
            if (r.rType == RelationshipType::PATTERN) {
                tempResult = patternHandler->handlePattern(r);
            } else if (r.rType == RelationshipType::WITH) {
                tempResult = withHandler->handleWith(r);
            } else {
                tempResult = suchThatHandler->handleSuchThat(r);
            }
            results.push_back(tempResult);
        }
        tempGroup.setGroup(results);
        resultGroups.push_back(tempGroup);
    }
    return resultGroups;
}

Result QPSHandler::getNoClauseResult(const Entity& entityToFind) const {
    Result result;
    result.setResultType(ResultType::NO_CLAUSE);

    ElementType elementTypeToGet = QpsTypeToPkbTypeConvertor::convertToPkbElement(entityToFind.eType);
    std::set<ProgramElement> oneSyn = pg->getEntity(elementTypeToGet);

    if (oneSyn.empty()) {
        result.setValid(false);
    } else {
        result.setValid(true);
        result.setOneSynEntity(entityToFind);
        result.setOneSynSet(oneSyn);
    }

    return result;
}
