#include <vector>
#include "Statement.h"

#ifndef STMTLST_H
#define STMTLST_H

class StmtLst {
private: 
	std::vector<Statement> stmts;
public:
	StmtLst();
	Statement getStmtAtIndex(int index) const;
	std::size_t getSize() const;
	void setNextStmt(Statement stmt);
};

#endif