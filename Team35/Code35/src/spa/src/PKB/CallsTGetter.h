#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_CALLSTGETTER_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_CALLSTGETTER_H_

#include "DB.h"
#include "ProgramElement.h"
#include "TemplateGetter.h"
#include <set>

class CallsTGetter : TemplateGetter {
private:
    DB* db;

public:
    explicit CallsTGetter(DB* db);

    bool isCallsT(const ProgramElement& leftSide, const ProgramElement& rightSide) const;
    std::set<ProgramElement> getLeftCallsT(const ProgramElement& rightSide, const ElementType& typeToGet) const;
    std::set<ProgramElement> getRightCallsT(const ProgramElement& leftSide, const ElementType& typeToGet) const;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_CALLSTGETTER_H_
