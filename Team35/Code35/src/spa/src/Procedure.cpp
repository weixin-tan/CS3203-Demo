#include "Procedure.h"

Procedure::Procedure() {

}

std::string Procedure::getProcName() const {
	return this->proc_name;
}

StmtLst Procedure::getStmtLst() const {
	return this->stmtLst;
}
void Procedure::setProcName(std::string name) {
	this->proc_name = name;
}

void Procedure::setStmtLst(StmtLst stmtLst) {
	this->stmtLst = stmtLst;
}