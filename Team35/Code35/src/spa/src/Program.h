#include "ProcedureLst.h"

#ifndef PROGRAM_H
#define PROGRAM_H

class Program {
private:
	ProcedureLst procedure_lst;

public:
	Program();
	ProcedureLst getProcedureLst() const;
	void setProcedureLst(ProcedureLst procedure_lst);
};

#endif