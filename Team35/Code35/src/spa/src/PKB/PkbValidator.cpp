#include "PkbValidator.h"

PkbValidator::PkbValidator(DB* db) : db(db) {}

void PkbValidator::validateNoCyclicCall() {
    for (const auto&[proc, calledTProc] : db->callsTTable) {
        if (calledTProc.count(proc) != 0)
            throw std::invalid_argument("Cyclic call detected! Offending procedure: " + proc);
    }
}

void PkbValidator::validateCallsExists() {
    std::set<std::string> calledProcs;
    for (const auto&[_, currentCalledProcs] : db->callsTable)
        for (const auto& calledProc : currentCalledProcs)
            calledProcs.insert(calledProc);

    for (const auto& calledProc : calledProcs)
        if (db->procedures.count(calledProc) == 0)
            throw std::invalid_argument("Undefined " + calledProc + " procedure called!");
}

void PkbValidator::validateNoDuplicateProcedure(const std::vector<std::vector<ParsedStatement>>& procedures) {
    std::set<std::string> definedProcs;
    for (const auto& procedure : procedures) {
        std::string proc = procedure.begin()->procedureName;
        if (definedProcs.count(proc) != 0)
            throw std::invalid_argument("PROCEDURE " + proc + " redefined!\n");
        definedProcs.insert(proc);
    }
}

void PkbValidator::validate(const std::vector<std::vector<ParsedStatement>>& procedures, bool testing) {
    try {
        validateNoCyclicCall();
        validateCallsExists();
        PkbValidator::validateNoDuplicateProcedure(procedures);
    } catch (const std::exception& e) {
        if (testing) throw e;  // testing purpose
        std::cout <<
                  "SIMPLE source semantic error detected. Details following:\n" <<
                  e.what() << '\n';
        exit(1);
    }
}