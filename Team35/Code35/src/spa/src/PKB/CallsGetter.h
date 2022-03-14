#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_CALLSGETTER_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_CALLSGETTER_H_

#include "DB.h"
#include "ProgramElement.h"
#include "TemplateGetter.h"
#include <set>

class CallsGetter : TemplateGetter {
private:
    DB* db;

public:
    explicit CallsGetter(DB* db);

    bool isCalls(const ProgramElement& leftSide, const ProgramElement& rightSide) const;
    std::set<ProgramElement> getLeftCalls(const ProgramElement& rightSide, const ElementType& typeToGet) const;
    std::set<ProgramElement> getRightCalls(const ProgramElement& leftSide, const ElementType& typeToGet) const;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_CALLSGETTER_H_