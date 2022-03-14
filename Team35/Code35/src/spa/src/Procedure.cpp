#include "Procedure.h"

Procedure::Procedure() {

}

std::string Procedure::getProcName() const {
    return this->procName;
}

StmtLst Procedure::getStmtLst() const {
    return this->stmtLst;
}
void Procedure::setProcName(std::string name) {
    this->procName = name;
}

void Procedure::setStmtLst(StmtLst stmtLst) {
    this->stmtLst = stmtLst;
}

void Procedure::setSize(int size) {
    this->size = size;
}

int Procedure::getSize() {
    return this->size;
}