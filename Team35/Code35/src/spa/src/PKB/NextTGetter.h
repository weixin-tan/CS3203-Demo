#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_NEXTTGETTER_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_NEXTTGETTER_H_

#include "RelationshipGetter.h"

class NextTGetter : public RelationshipGetter {
private:
    DB* db;
    static void dfsNextT(int src, std::set<int>& computedNextTSrc, const std::map<int, std::set<int>>& nextGraph, std::map<int, std::set<int>>& nextTGraph);
    void computeAndCacheNextT(int src);
    void computeAndCacheNextTR(int src);

public:
    explicit NextTGetter(DB* db);

    bool isRelationship(const ProgramElement& leftSide, const ProgramElement& rightSide) override;
    std::set<ProgramElement> getLeftSide(const ProgramElement& rightSide, const ElementType& typeToGet) override;
    std::set<ProgramElement> getRightSide(const ProgramElement& leftSide, const ElementType& typeToGet) override;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_NEXTTGETTER_H_
