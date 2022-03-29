#include "QPSMainLogic.h"

QPSMainLogic::QPSMainLogic(PkbGetter* pg) {
    QPSMainLogic::queryProcessor = new QueryProcessor();
    QPSMainLogic::preOptimiser = new PreOptimiser();
    QPSMainLogic::qpsHandler = new QPSHandler(pg);
    QPSMainLogic::postOptimiser = new PostOptimiser();
    QPSMainLogic::resultProcessor = new ResultProcessor();
    QPSMainLogic::resultFormatter = new ResultFormatter();
}

std::list<std::string> QPSMainLogic::parse(const std::string& query) {
    std::vector<Clause> clauses = callParser(query);
    if (!clauses.empty()){
        GroupedClause groupedClause = callPreOptimiser(clauses);
        std::vector<ResultGroup> resultGroups = callHandler(groupedClause);
        std::vector<ResultGroup> optimisedResultGroups = callPostOptimiser(resultGroups);
        FormattedResult processedEntities = callProcessor(optimisedResultGroups);
        std::list<std::string> finalResult = callFormatter(processedEntities);
        return finalResult;
    } else{
        std::list<std::string> emptyList;
        return emptyList;
    }
}

std::vector<Clause> QPSMainLogic::callParser(const std::string& query) {
    return queryProcessor->parsePQL(query);
}

GroupedClause QPSMainLogic::callPreOptimiser(const std::vector<Clause>& clauses) {
    return preOptimiser->optimise(clauses);
}

std::vector<ResultGroup> QPSMainLogic::callHandler(const GroupedClause& groupedClause) {
    return qpsHandler->processClause(groupedClause);
}

std::vector<ResultGroup> QPSMainLogic::callPostOptimiser(const std::vector<ResultGroup>& resultsGroups) {
    return postOptimiser->optimise(resultsGroups);
}

FormattedResult QPSMainLogic::callProcessor(const std::vector<ResultGroup>& optimisedGroups) {
    return resultProcessor->processResults(optimisedGroups);
}

std::list<std::string> QPSMainLogic::callFormatter(FormattedResult processedEntities) {
    return resultFormatter->formatResult(processedEntities);
}
