#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PARENTTGETTER_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PARENTTGETTER_H_

#include "DB.h"
#include "ProgramElement.h"
#include "TemplateGetter.h"
#include <set>

class ParentTGetter : TemplateGetter {
private:
    DB* db;

public:
    explicit ParentTGetter(DB* db);

    bool isParentT(const ProgramElement& leftSide, const ProgramElement& rightSide) const;
    std::set<ProgramElement> getLeftParentT(const ProgramElement& rightSide, const ElementType& typeToGet) const;
    std::set<ProgramElement> getRightParentT(const ProgramElement& leftSide, const ElementType& typeToGet) const;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PARENTTGETTER_H_
