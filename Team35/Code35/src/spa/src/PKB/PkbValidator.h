#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PKBVALIDATOR_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PKBVALIDATOR_H_

#include "DB.h"

class PkbValidator {
private:
    DB* db;

public:
    explicit PkbValidator(DB* db);
    void validateNoCyclicCall();  // no cyclic call
    void validateCallsExists();  // called target must exist
    void validateNoDuplicateProcedure();  // no procedure with duplicated name
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PKBVALIDATOR_H_
