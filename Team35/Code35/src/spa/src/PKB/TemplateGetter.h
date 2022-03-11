#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_TEMPLATEGETTER_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_TEMPLATEGETTER_H_

#include <set>

#include "ProgramElement.h"
#include "DB.h"

class TemplateGetter {
public:
    static bool isStatementTypeToGet(const ElementType& typeToGet, const ElementType& targetType);
    static void insertStmtElement(std::set<ProgramElement>& result, const ProgramElement& statement, const ElementType& typeToGet);
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_TEMPLATEGETTER_H_
