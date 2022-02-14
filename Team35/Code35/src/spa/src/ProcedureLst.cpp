#include "ProcedureLst.h"

ProcedureLst::ProcedureLst() {
	std::vector<Procedure> p = {};
	this->procedures = p;
}

Procedure ProcedureLst::getProcedureAtIndex(int index) const {
	return this->procedures.at(index);
}

std::size_t ProcedureLst::getSize() const {
	return this->procedures.size();
}

void ProcedureLst::setNextProcedure(Procedure procedure) {
	this->procedures.push_back(procedure);
}