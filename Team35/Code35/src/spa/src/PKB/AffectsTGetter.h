#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_AFFECTSTGETTER_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_AFFECTSTGETTER_H_

#include "RelationshipGetter.h"
#include "../DesignExtractor/DesignExtractor.h"

class AffectsTGetter : public RelationshipGetter {
private:
    DB* db;
    DesignExtractor de;

public:
    explicit AffectsTGetter(DB* db);

    bool isRelationship(const ProgramElement& leftSide, const ProgramElement& rightSide) override;
    std::set<ProgramElement> getLeftSide(const ProgramElement& rightSide, const ElementType& typeToGet) override;
    std::set<ProgramElement> getRightSide(const ProgramElement& leftSide, const ElementType& typeToGet) override;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_AFFECTSTGETTER_H_
