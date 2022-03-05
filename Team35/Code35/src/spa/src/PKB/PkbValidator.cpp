#include "PkbValidator.h"

PkbValidator::PkbValidator(DB *db) : db(db) {}

void PkbValidator::validateNoCyclicCall() {
    for (const auto&[proc, calledTProc] : db->callsTTable) {
        if (calledTProc.count(proc) != 0)
            throw std::invalid_argument("Cyclic call detected!\n" + proc);
    }
}

// TODO
void PkbValidator::validateCallsExists() {
    throw "PkbValidator::validateCallsExists not implemented!";
}

void PkbValidator::validateNoDuplicateProcedure(const std::vector<std::vector<ParsedStatement>>& procedures) {
    std::set<std::string> definedProcs;
    for (const auto& procedure : procedures) {
        std::string proc = procedure.begin()->procedure_name;
        if (definedProcs.count(proc) != 0)
            throw std::invalid_argument("Procedure redefined!\n" + proc);
        definedProcs.insert(proc);
    }
}
