#include "Program.h"

Program::Program() {

}

ProcedureLst Program::getProcedureLst() const {
    return this->procedure_lst;
}

void Program::setProcedureLst(ProcedureLst procedure_lst) {
    this->procedure_lst = procedure_lst;
}