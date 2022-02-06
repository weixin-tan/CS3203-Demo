#include "QPSMainLogic.h"

// Global static pointer to ensure only a single instance of the class is initialized.
QPSMainLogic* QPSMainLogic::instance = nullptr; // Null, will be initialized on demand.

// Called to create an instance of the class if one has not been created already.
// Otherwise, return the instance that has already been created.
QPSMainLogic* QPSMainLogic::getInstance(PkbGetter* pg) {
  if (!instance) { // Makes sure only one instance of the class is generated.
    instance = new QPSMainLogic(pg);
  }
  // Insert Logging statement that instance has already been generated
  return instance;
}

// Constructor
QPSMainLogic::QPSMainLogic(PkbGetter* pg) {
  queryProcessor = new QueryProcessor();
  suchThatHandler = new SuchThatHandler(pg);
  resultProcessor = new ResultProcessor();
  resultFormatter = new ResultFormatter();
}

//
std::string QPSMainLogic::parse(std::string query) {
  std::vector<Clause> clauses = callParser(query);
  std::vector<Result> matchingEntities = callHandler(clauses);
  Result processedEntities = callProcessor(matchingEntities);
  std::string finalResult = callFormatter(processedEntities);
  return finalResult;
}

std::vector<Clause> QPSMainLogic::callParser(std::string query) {
  std::vector<Clause> clauses = queryProcessor->parsePQL(query);
  return clauses;
}

std::vector<Result> QPSMainLogic::callHandler(std::vector<Clause> clauses) {
  std::vector<Result> matchingEntities = suchThatHandler->processClause(clauses);
  return matchingEntities;
}

Result QPSMainLogic::callProcessor(std::vector<Result> matchingEntities) {
  Result processedEntities = resultProcessor->processResults(matchingEntities);
  return processedEntities;
}

std::string QPSMainLogic::callFormatter(Result processedEntities) {
  std::string finalResult = resultFormatter->formatResult(processedEntities);
  return finalResult;
}
