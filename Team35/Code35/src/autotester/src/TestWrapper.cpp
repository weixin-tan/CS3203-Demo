#include "TestWrapper.h"
#include "Tokeniser.h"
#include "ConcreteSyntaxBasic.h"
#include "Convertor.h"
#include "PKB/PKB.h"
#include <iostream>
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

// a default constructor
TestWrapper::TestWrapper(): pkb(), convertor(pkb.getSetter()) {
  TestWrapper::qpsMainLogic = QPSMainLogic::getInstance(pkb.getGetter());
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	std::ifstream t(filename);
	std::stringstream buffer;
	buffer << t.rdbuf();

	Tokeniser tokeniser;
	ConcreteSyntaxBasic concrete;
	std::vector<Procedure> procedureLst;
	procedureLst.push_back(concrete.parseProcedure(tokeniser.putInQueue(buffer.str())));
	convertor.ProcedureReader(procedureLst);
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
  std::list<std::string> resultStr = qpsMainLogic->parse(query);
  for (const auto& s : resultStr) {
      results.push_back(s);
  }
}
