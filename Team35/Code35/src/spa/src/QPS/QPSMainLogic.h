#ifndef SPA_QPSMAINLOGIC_H
#define SPA_QPSMAINLOGIC_H

#include <string>

#include "FormattedResult.h"
#include "QPSHandler.h"
#include "QueryProcessor.h"
#include "Result.h"
#include "ResultFormatter.h"
#include "PreOptimiser.h"

class QPSMainLogic {
public:
    explicit QPSMainLogic(PkbGetter* pg);
    std::list<std::string> parse(const std::string& query);

private:
    QueryProcessor* queryProcessor;
    PreOptimiser* preOptimiser;
    QPSHandler* qpsHandler;
    ResultFormatter* resultFormatter;

    std::vector<Clause> callParser(const std::string& query);
    GroupedClause callPreOptimiser(const std::vector<Clause>& clauses);
    FormattedResult callHandler(const GroupedClause& groupedClause);
    std::list<std::string> callFormatter(const FormattedResult& processedResults);
    bool checkSemanticBoolError(const Entity& e);
};

#endif //SPA_QPSMAINLOGIC_H
