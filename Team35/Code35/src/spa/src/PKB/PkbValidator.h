#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PKBVALIDATOR_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PKBVALIDATOR_H_

#include "DB.h"

class PkbValidator {
private:
    DB* db;

private:
    void validateNoCyclicCall();
    void validateCallsExists();
    static void validateNoDuplicateProcedure(const std::vector<std::vector<ParsedStatement>>& procedures);

public:
    explicit PkbValidator(DB* db);
    void validate(const std::vector<std::vector<ParsedStatement>>& procedures, bool testing);
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PKBVALIDATOR_H_
