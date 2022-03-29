#ifndef SPA_QPSMAINLOGIC_H
#define SPA_QPSMAINLOGIC_H

#include <string>

#include "FormattedResult.h"
#include "PostOptimiser.h"
#include "QPSHandler.h"
#include "QueryProcessor.h"
#include "Result.h"
#include "ResultFormatter.h"
#include "ResultProcessor.h"
#include "PreOptimiser.h"

class QPSMainLogic {
public:
    explicit QPSMainLogic(PkbGetter* pg);
    std::list<std::string> parse(const std::string& query);

private:
    QueryProcessor* queryProcessor;
    PreOptimiser* preOptimiser;
    QPSHandler* qpsHandler;
    PostOptimiser* postOptimiser;
    ResultProcessor* resultProcessor;
    ResultFormatter* resultFormatter;

    std::vector<Clause> callParser(const std::string& query);
    GroupedClause callPreOptimiser(const std::vector<Clause>& clauses);
    std::vector<ResultGroup> callHandler(const GroupedClause& groupedClause);
    std::vector<ResultGroup> callPostOptimiser(const std::vector<ResultGroup>& resultsGroups) ;
    FormattedResult callProcessor(const std::vector<ResultGroup>& groups);
    std::list<std::string> callFormatter(FormattedResult result);
};

#endif //SPA_QPSMAINLOGIC_H
