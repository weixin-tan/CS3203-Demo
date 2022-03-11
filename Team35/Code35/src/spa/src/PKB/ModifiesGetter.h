#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_MODIFIESGETTER_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_MODIFIESGETTER_H_

#include "DB.h"
#include "ProgramElement.h"
#include "TemplateGetter.h"
#include <set>

class ModifiesGetter : TemplateGetter {
private:
    DB* db;
public:
    explicit ModifiesGetter(DB* db);

    bool isModifies(const ProgramElement& leftSide, const ProgramElement& rightSide) const;
    std::set<ProgramElement> getLeftModifies(const ProgramElement& rightSide, const ElementType& typeToGet) const;
    std::set<ProgramElement> getRightModifies(const ProgramElement& leftSide, const ElementType& typeToGet) const;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_MODIFIESGETTER_H_
