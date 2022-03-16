#ifndef PROCEDURELST_H
#define PROCEDURELST_H

#include <vector>
#include "Procedure.h"

class ProcedureLst {
private:
    std::vector<Procedure> procedures;
public:
    ProcedureLst();
    Procedure getProcedureAtIndex(int index) const;
    std::size_t getSize() const;
    void setNextProcedure(Procedure procedure);
};

#endif