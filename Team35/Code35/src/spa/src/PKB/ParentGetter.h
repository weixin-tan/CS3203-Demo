#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PARENTGETTER_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PARENTGETTER_H_

#include "DB.h"
#include "ProgramElement.h"
#include "RelationshipGetter.h"
#include <set>

class ParentGetter : public RelationshipGetter {
private:
    DB* db;

public:
    explicit ParentGetter(DB* db);

    bool isRelationship(const ProgramElement& leftSide, const ProgramElement& rightSide) override;
    std::set<ProgramElement> getLeftSide(const ProgramElement& rightSide, const ElementType& typeToGet) override;
    std::set<ProgramElement> getRightSide(const ProgramElement& leftSide, const ElementType& typeToGet) override;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PARENTGETTER_H_
