#ifndef SPA_QPSMAINLOGIC_H
#define SPA_QPSMAINLOGIC_H

#include <string>
#include "QueryProcessor.h"
#include "Result.h"
#include "ResultProcessor.h"
#include "ResultFormatter.h"
#include "QPSHandler.h"

class QPSMainLogic {
 public:
  static QPSMainLogic* getInstance(PkbGetter* pg); // Static access method
  std::list<std::string> parse(std::string query);

 private:
  explicit QPSMainLogic(PkbGetter* pg); // Make constructor private
  QPSMainLogic(const QPSMainLogic&); // Make copy constructor private
  QPSMainLogic& operator = (const QPSMainLogic&); // Make assignment operator private
  static QPSMainLogic* instance; // The instance of QPSMainLogic will be stored here
  QueryProcessor* queryProcessor;
  QPSHandler* qpsHandler;
  ResultProcessor* resultProcessor;
  ResultFormatter* resultFormatter;

  std::vector<Clause> callParser(std::string query);
  std::vector<Result> callHandler(std::vector<Clause> clauses);
  std::set<ProgramElement> callProcessor(std::vector<Result> results);
  std::list<std::string> callFormatter(std::set<ProgramElement> result);
};


#endif //SPA_QPSMAINLOGIC_H
