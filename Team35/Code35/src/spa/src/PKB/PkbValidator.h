#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PKBVALIDATOR_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PKBVALIDATOR_H_

#include "DB.h"

class PkbValidator {
private:
    DB* db;

public:
    explicit PkbValidator(DB* db);
    void validateNoCyclicCall();
    void validateCallsExists();
    static void validateNoDuplicateProcedure(const std::vector<std::vector<ParsedStatement>>& procedures);
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PKBVALIDATOR_H_
