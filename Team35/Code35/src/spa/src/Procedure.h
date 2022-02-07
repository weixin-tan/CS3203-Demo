#include <string>
#include "StmtLst.h"

#ifndef PROCEDURE_H
#define PROCEDURE_H

class Procedure {
private:
	std::string proc_name;
	StmtLst stmtLst;
	int size;
public:
	Procedure();
	std::string getProcName() const;
	StmtLst getStmtLst() const;
	void setProcName(std::string name);
	void setStmtLst(StmtLst stmtLst);
	void setSize(int size);
	int getSize();
};

#endif