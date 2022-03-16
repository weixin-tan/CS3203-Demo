#include "TestWrapper.h"
#include "AbstractWrapper.h"
#include <fstream>
#include <vector>


// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;
AbstractWrapper::~AbstractWrapper() = default;

// a default constructor
TestWrapper::TestWrapper() : pkb(), sp(pkb.getSetter()) {
	TestWrapper::qpsMainLogic = QPSMainLogic::getInstance(pkb.getGetter());
}

TestWrapper::~TestWrapper() = default;

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	sp.Parse(filename);
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
	std::list<std::string> resultStr = qpsMainLogic->parse(query);
	for (const auto& s : resultStr) {
		results.push_back(s);
	}
    pkb.clearCache();  // should this be moved inside QPS?
}

