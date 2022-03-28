#include "QPSMainLogic.h"

// Global static pointer to ensure only a single instance of the class is initialized.
QPSMainLogic* QPSMainLogic::instance = nullptr; // NULL_ENTITY, will be initialized on demand.

// Called to create an instance of the class if one has not been created already.
// Otherwise, return the instance that has already been created.
QPSMainLogic* QPSMainLogic::getInstance(PkbGetter* pg) {
    if (!instance) { // Makes sure only one instance of the class is generated.
        instance = new QPSMainLogic(pg);
    }
    return instance;
}

// Constructor
QPSMainLogic::QPSMainLogic(PkbGetter* pg) {
    QPSMainLogic::queryProcessor = new QueryProcessor();
    QPSMainLogic::qpsHandler = new QPSHandler(pg);
    QPSMainLogic::optimiser = new Optimiser();
    QPSMainLogic::resultProcessor = new ResultProcessor();
    QPSMainLogic::resultFormatter = new ResultFormatter();
    QPSMainLogic::preOptimiser = new PreOptimiser();
}

//main function
std::list<std::string> QPSMainLogic::parse(const std::string& query) {
    std::vector<Clause> clauses = callParser(query);
    if (!clauses.empty()){
        GroupedClause groupedClause = callPreOptimiser(clauses);
        std::vector<ResultGroup> resultsGroups = callHandler(groupedClause);
        std::vector<ResultGroup> optimisedResultGroups = callPostOptimiser(resultsGroups);
        FormattedResult processedEntities = callProcessor(optimisedResultGroups);
        std::list<std::string> finalResult = callFormatter(processedEntities);
        return finalResult;
    }else{
        std::list<std::string> emptyList;
        return emptyList;
    }
}

std::vector<Clause> QPSMainLogic::callParser(const std::string& query) {
    return queryProcessor->parsePQL(query);
}

std::vector<ResultGroup> QPSMainLogic::callHandler(const GroupedClause& groupedClause) {
    return qpsHandler->processClause(groupedClause);
}

std::vector<ResultGroup> QPSMainLogic::callPostOptimiser(const std::vector<ResultGroup>& resultsGroups) {
    return optimiser->optimise(resultsGroups);
}

FormattedResult QPSMainLogic::callProcessor(const std::vector<ResultGroup>& optimisedGroups) {
    return resultProcessor->processResults(optimisedGroups);
}

std::list<std::string> QPSMainLogic::callFormatter(FormattedResult processedEntities) {
    return resultFormatter->formatResult(processedEntities);
}

GroupedClause QPSMainLogic::callPreOptimiser(const std::vector<Clause>& clauses) {
    return preOptimiser->optimise(clauses);
}
