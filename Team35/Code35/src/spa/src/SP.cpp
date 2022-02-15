#include "SP.h"

#include <iostream>
#include <fstream>
#include <vector>

SP::SP(PkbSetter* pkb_setter) : tokeniser(), concrete(), convertor(pkb_setter) {

}

void SP::Parse(std::string filename) {
	std::ifstream t(filename);
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::vector<Procedure> procedureLst;

	// Creating the queue of tokens
	tokenQueue = tokeniser.putInQueue(buffer.str());
	
	// Create Procedure
	procedure = concrete.parseProcedure(tokenQueue);

	// Push back the procedure to the procedure list
	procedureLst.push_back(procedure);

	// Convert the procedure
	convertor.ProcedureReader(procedureLst);
}

