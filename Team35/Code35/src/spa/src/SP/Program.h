#ifndef PROGRAM_H
#define PROGRAM_H

#include "ProcedureLst.h"

class Program {
private:
    ProcedureLst procedure_lst;

public:
    Program();
    ProcedureLst getProcedureLst() const;
    void setProcedureLst(ProcedureLst procedure_lst);
};

#endif