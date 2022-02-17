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
	ProcedureLst procedureLst;

	// Creating the queue of tokens
	tokenQueue = tokeniser.putInQueue(buffer.str());
	
	// Create Procedure
	try {
		procedure = concrete.parseProcedure(tokenQueue);
	}
	catch (const std::invalid_argument& e) {
		std::terminate();
	}

	// Push back the procedure to the procedure list
	procedureLst.setNextProcedure(procedure);

	// Convert the procedure
	convertor.ProcedureReader(procedureLst);
}

