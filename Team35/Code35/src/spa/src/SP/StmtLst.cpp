#include "StmtLst.h"

class Statement;

StmtLst::StmtLst() {
    std::vector<Statement> s = {};
    this->stmts = s;
}

ContainerType StmtLst::getContainerType() {
    return this->containerType;
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

void StmtLst::SetContainerType(ContainerType containerType) {
    this->containerType = containerType;
}
