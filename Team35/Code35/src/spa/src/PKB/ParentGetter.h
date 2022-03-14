#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PARENTGETTER_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PARENTGETTER_H_

#include "DB.h"
#include "ProgramElement.h"
#include "TemplateGetter.h"
#include <set>

class ParentGetter : TemplateGetter {
private:
    DB* db;

public:
    explicit ParentGetter(DB* db);

    bool isParent(const ProgramElement& leftSide, const ProgramElement& rightSide) const;
    std::set<ProgramElement> getLeftParent(const ProgramElement& rightSide, const ElementType& typeToGet) const;
    std::set<ProgramElement> getRightParent(const ProgramElement& leftSide, const ElementType& typeToGet) const;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PARENTGETTER_H_
