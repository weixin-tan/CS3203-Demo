#ifndef PROCEDURE_H
#define PROCEDURE_H

#include <string>
#include "StmtLst.h"

class Procedure {
private:
    std::string procName;
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