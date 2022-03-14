#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_NEXTGETTER_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_NEXTGETTER_H_

#include "DB.h"
#include "ProgramElement.h"
#include "TemplateGetter.h"
#include <set>

class NextGetter : TemplateGetter {
private:
    DB* db;

public:
    explicit NextGetter(DB* db);

    bool isNext(const ProgramElement& leftSide, const ProgramElement& rightSide) const;
    std::set<ProgramElement> getLeftNext(const ProgramElement& rightSide, const ElementType& typeToGet) const;
    std::set<ProgramElement> getRightNext(const ProgramElement& leftSide, const ElementType& typeToGet) const;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_NEXTGETTER_H_
