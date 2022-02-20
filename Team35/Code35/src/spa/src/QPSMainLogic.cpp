#include "QPSMainLogic.h"

// Global static pointer to ensure only a single instance of the class is initialized.
QPSMainLogic* QPSMainLogic::instance = nullptr; // Null, will be initialized on demand.

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
  QPSMainLogic::resultProcessor = new ResultProcessor();
  QPSMainLogic::resultFormatter = new ResultFormatter();
}

//main function
std::list<std::string> QPSMainLogic::parse(std::string query) {
  std::vector<Clause> clauses = callParser(query);
  std::vector<Result> matchingEntities = callHandler(clauses);
  std::set<ProgramElement> processedEntities = callProcessor(matchingEntities);
  std::list<std::string> finalResult = callFormatter(processedEntities);
  return finalResult;
}

std::vector<Clause> QPSMainLogic::callParser(std::string query) {
  std::vector<Clause> clauses = queryProcessor->parsePQL(query);
  return clauses;
}

std::vector<Result> QPSMainLogic::callHandler(std::vector<Clause> clauses) {
  std::vector<Result> matchingEntities = qpsHandler->processClause(clauses);
  return matchingEntities;
}

std::set<ProgramElement> QPSMainLogic::callProcessor(std::vector<Result> matchingEntities) {
  std::set<ProgramElement> processedEntities = resultProcessor->processResults(matchingEntities);
  return processedEntities;
}

std::list<std::string> QPSMainLogic::callFormatter(std::set<ProgramElement> processedEntities) {
  std::list<std::string> finalResult = resultFormatter->formatResult(processedEntities);
  return finalResult;
}
