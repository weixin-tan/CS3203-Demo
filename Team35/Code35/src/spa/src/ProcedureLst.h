#include <vector>
#include "Procedure.h"

#ifndef PROCEDURELST_H
#define PROCEDURELST_H

class ProcedureLst {
private:
	std::vector<Procedure> procedures;
public:
	ProcedureLst();
	Procedure getProcedureAtIndex(int index) const;
	std::size_type getSize() const;
	void setNextProcedure(Procedure procedure);
};

#endif