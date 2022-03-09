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
	
	// Create Program
	try {
		program = concrete.parseProgram(tokenQueue);
	}
	catch (const std::invalid_argument& e) {
		std::terminate();
	}

	procedureLst = program.getProcedureLst();

	// Convert the procedure
	convertor.ProcedureReader(procedureLst);
}

