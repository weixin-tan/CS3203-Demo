#include "QPSMainLogic.h"

QPSMainLogic::QPSMainLogic(PkbGetter* pg) {
    QPSMainLogic::queryProcessor = new QueryProcessor();
    QPSMainLogic::preOptimiser = new PreOptimiser();
    QPSMainLogic::qpsHandler = new QPSHandler(pg);
    QPSMainLogic::resultFormatter = new ResultFormatter();
}

std::list<std::string> QPSMainLogic::parse(const std::string& query) {
    std::list<std::string> emptyList;
    std::vector<Clause> clauses = callParser(query);
    if (clauses.empty()) {
        return emptyList;
    }
    if (checkSemanticBoolError(clauses[0].entityToFindList[0])) {
        emptyList.emplace_back("FALSE");
        return emptyList;
    }
    GroupedClause groupedClause = callPreOptimiser(clauses);
    FormattedResult formattedResult;
    try {
        formattedResult = callHandler(groupedClause);
    } catch (std::invalid_argument& e){
        return emptyList;
    }
    std::list<std::string> finalResult = callFormatter(formattedResult);
    return finalResult;
}

std::vector<Clause> QPSMainLogic::callParser(const std::string& query) {
    return queryProcessor->parsePQL(query);
}

GroupedClause QPSMainLogic::callPreOptimiser(const std::vector<Clause>& clauses) {
    return preOptimiser->optimise(clauses);
}

FormattedResult QPSMainLogic::callHandler(const GroupedClause& groupedClause) {
    return qpsHandler->processClause(groupedClause);
}

std::list<std::string> QPSMainLogic::callFormatter(const FormattedResult& processedResults) {
    return resultFormatter->formatResult(processedResults);
}

bool QPSMainLogic::checkSemanticBoolError(const Entity& e) {
    return e.eType == EntityType::BOOLEAN && e.name == "FALSE";
}
