#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>

#include "PKB/PKB.h"
#include "Convertor.h"

// include your other headers here
#include "AbstractWrapper.h"

class TestWrapper : public AbstractWrapper {
 private:
	PKB pkb;
	Convertor convertor;
 public:
  // default constructor
  TestWrapper();

  // destructor
  ~TestWrapper();
  
  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);
  
  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results);
	
};

#endif
