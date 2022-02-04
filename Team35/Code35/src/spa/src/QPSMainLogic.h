//
// Created by viv on 4/2/2022.
//

#ifndef SPA_QPSMAINLOGIC_H
#define SPA_QPSMAINLOGIC_H


#include <string>
#include "QueryProcessor.h"
#include "SuchThatHandler.h"
#include "Result.h"
#include "ResultProcessor.h"
#include "ResultFormatter.h"

class QPSMainLogic {
 public:
  static QPSMainLogic* getInstance(); // Static access method
  std::string parse(std::string query);

 private:
  QPSMainLogic(); // Make constructor private
  QPSMainLogic(const QPSMainLogic&); // Make copy constructor private
  QPSMainLogic& operator = (const QPSMainLogic&); // Make assignment operator private
  static QPSMainLogic* instance; // The instance of QPSMainLogic will be stored here
  QueryProcessor* queryProcessor;
  SuchThatHandler* suchThatHandler;
  ResultProcessor* resultProcessor;
  ResultFormatter* resultFormatter;

  std::vector<Clause> callParser(std::string query);
  std::vector<Result> callHandler(std::vector<Clause> clauses);
  Result callProcessor(std::vector<Result> results);
  std::string callFormatter(Result result);
};


#endif //SPA_QPSMAINLOGIC_H
