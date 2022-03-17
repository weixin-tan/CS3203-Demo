#include "QPSHandler.h"

QPSHandler::QPSHandler(PkbGetter* pg) {
    QPSHandler::pg = pg;
    QPSHandler::patternHandler = new PatternHandler(pg);
    QPSHandler::suchThatHandler = new SuchThatHandler(pg);
    QPSHandler::withHandler = new WithHandler(pg);
}

std::vector<Result> QPSHandler::processClause(const std::vector<Clause>& clauses) const {
    std::vector<Result> results;

    Clause c = clauses[0];
    Entity entityToFind = c.entityToFindList.front(); // Only returning 1 entity is supported

    Result noClauseResult = getNoClauseResult(entityToFind);
    results.push_back(noClauseResult);

    for (const auto& r : c.refList) {
        Result result;
        if (r.rType == RelationshipType::PATTERN) {
            result = patternHandler->handlePattern(r);
        } else if (r.rType == RelationshipType::WITH) {
            result = withHandler->handleWith(r);
        } else {
            result = suchThatHandler->handleSuchThat(r);
        }
        results.push_back(result);
    }

    return results;
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
