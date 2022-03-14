#ifndef STMTLST_H
#define STMTLST_H

#include <vector>
#include "Statement.h"

enum class ContainerType {
    kprocedure,
    kifthen,
    kwhile,
    kifelse
};

class Statement;

class StmtLst {
private: 
    std::vector<Statement> stmts;
public:
    StmtLst();
    ContainerType containerType;

    ContainerType getContainerType(); 
    Statement getStmtAtIndex(int index) const;
    std::size_t getSize() const;
    void setNextStmt(Statement stmt);
    void SetContainerType(ContainerType containerType);
};



#endif