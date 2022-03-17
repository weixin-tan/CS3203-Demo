#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_NEXTTGETTER_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_NEXTTGETTER_H_

#include "TemplateGetter.h"

class NextTGetter : TemplateGetter {
private:
    DB* db;
    void dfsNextT(int src, std::set<int>& computedNextTSrc, const std::map<int, std::set<int>>& nextGraph, std::map<int, std::set<int>>& nextTGraph);
    void computeAndCacheNextT(int src);
    void computeAndCacheNextTR(int src);

public:
    explicit NextTGetter(DB* db);

    bool isNextT(const ProgramElement& leftSide, const ProgramElement& rightSide);
    std::set<ProgramElement> getLeftNextT(const ProgramElement& rightSide, const ElementType& typeToGet);
    std::set<ProgramElement> getRightNextT(const ProgramElement& leftSide, const ElementType& typeToGet);
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_NEXTTGETTER_H_
