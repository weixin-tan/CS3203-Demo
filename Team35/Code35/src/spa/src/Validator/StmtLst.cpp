#include "StmtLst.h"

StmtLst::StmtLst() {
	std::vector<Statement> s = {};
	this->stmts = s;
}

Statement StmtLst::getStmtAtIndex(int index) const {
	return this->stmts.at(index);
}

std::size_t StmtLst::getSize() const {
	return this->stmts.size();
}

void StmtLst::setNextStmt(Statement stmt) {
	this->stmts.push_back(stmt);
}