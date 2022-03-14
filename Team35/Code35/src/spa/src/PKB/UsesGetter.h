#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_USESGETTER_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_USESGETTER_H_

#include "DB.h"
#include "ProgramElement.h"
#include "TemplateGetter.h"
#include <set>

class UsesGetter : TemplateGetter {
private:
    DB* db;

private:
    std::set<int> getUsesStmtNosGivenConstant(const std::string& c) const;

public:
    explicit UsesGetter(DB* db);

    bool isUses(const ProgramElement& leftSide, const ProgramElement& rightSide) const;
    std::set<ProgramElement> getLeftUses(const ProgramElement& rightSide, const ElementType& typeToGet) const;
    std::set<ProgramElement> getRightUses(const ProgramElement& leftSide, const ElementType& typeToGet) const;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_USESGETTER_H_
