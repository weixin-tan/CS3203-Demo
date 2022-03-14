#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_FOLLOWSTGETTER_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_FOLLOWSTGETTER_H_

#include "DB.h"
#include "ProgramElement.h"
#include "TemplateGetter.h"
#include <set>

class FollowsTGetter : TemplateGetter {
private:
    DB* db;

public:
    explicit FollowsTGetter(DB* db);

    bool isFollowsT(const ProgramElement& leftSide, const ProgramElement& rightSide) const;
    std::set<ProgramElement> getLeftFollowsT(const ProgramElement& rightSide, const ElementType& typeToGet) const;
    std::set<ProgramElement> getRightFollowsT(const ProgramElement& leftSide, const ElementType& typeToGet) const;
};


#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_FOLLOWSTGETTER_H_
