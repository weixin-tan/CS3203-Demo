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
	procedureLst.push_back(concrete.parseProcedure(tokeniser.putInQueue(buffer.str())));
	convertor.ProcedureReader(procedureLst);
}