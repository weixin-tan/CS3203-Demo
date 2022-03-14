#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_FOLLOWSGETTER_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_FOLLOWSGETTER_H_

#include "DB.h"
#include "ProgramElement.h"
#include "TemplateGetter.h"
#include <set>

class FollowsGetter : TemplateGetter {
private:
    DB* db;

public:
    explicit FollowsGetter(DB* db);

    bool isFollows(const ProgramElement& leftSide, const ProgramElement& rightSide) const;
    std::set<ProgramElement> getLeftFollows(const ProgramElement& rightSide, const ElementType& typeToGet) const;
    std::set<ProgramElement> getRightFollows(const ProgramElement& leftSide, const ElementType& typeToGet) const;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_FOLLOWSGETTER_H_
