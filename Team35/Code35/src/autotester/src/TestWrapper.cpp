#include "TestWrapper.h"
#include "Convertor.h"
#include "PKB/PKB.h"


// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper(): pkb(), convertor(pkb.getSetter()) {
  TestWrapper::qpsMainLogic = QPSMainLogic::getInstance(pkb.getGetter());
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	// call your parser to do the parsing
  // ...rest of your code...
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
  std::string resultStr = qpsMainLogic->parse(query);
  results.push_back(resultStr);
}
