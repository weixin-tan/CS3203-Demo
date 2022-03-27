#ifndef SPA_QPSMAINLOGIC_H
#define SPA_QPSMAINLOGIC_H

#include <string>

#include "Optimiser.h"
#include "QPSHandler.h"
#include "QueryProcessor.h"
#include "Result.h"
#include "ResultFormatter.h"
#include "ResultProcessor.h"
#include "PreOptimiser.h"

class QPSMainLogic {
public:
    static QPSMainLogic* getInstance(PkbGetter* pg); // Static access method
    std::list<std::string> parse(const std::string& query);

private:
    explicit QPSMainLogic(PkbGetter* pg); // Make constructor private
    QPSMainLogic(const QPSMainLogic&); // Make copy constructor private
    QPSMainLogic& operator=(const QPSMainLogic&); // Make assignment operator private
    static QPSMainLogic* instance; // The instance of QPSMainLogic will be stored here
    QueryProcessor* queryProcessor;
    QPSHandler* qpsHandler;
    Optimiser* optimiser;
    ResultProcessor* resultProcessor;
    ResultFormatter* resultFormatter;
    PreOptimiser* preOptimiser;

    std::vector<Clause> callParser(const std::string& query);
    std::vector<ResultGroup> callHandler(const GroupedClause& groupedClause);
    std::vector<ResultGroup> callPostOptimiser(const std::vector<ResultGroup>& resultsGroups) ;
    std::vector<ProgramElement> callProcessor(const std::vector<ResultGroup>& groups);
    std::list<std::string> callFormatter(const std::vector<ProgramElement>& result);
    GroupedClause callPreOptimiser(const std::vector<Clause>& clauses);
};

#endif //SPA_QPSMAINLOGIC_H
