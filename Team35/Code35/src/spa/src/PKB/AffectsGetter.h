#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_AFFECTSGETTER_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_AFFECTSGETTER_H_

#include "RelationshipGetter.h"

class AffectsGetter : public RelationshipGetter {
private:
    DB* db;
    void dfsAffects(int src, const std::map<int, std::set<int>>& nextGraph, std::map<int, std::set<int>>& affectsGraph, const std::string& var);
    void computeAndCacheAffects(int src);
    void computeAndCacheAffectsR(int src);

public:
    explicit AffectsGetter(DB* db);

    bool isRelationship(const ProgramElement& leftSide, const ProgramElement& rightSide) override;
    std::set<ProgramElement> getLeftSide(const ProgramElement& rightSide, const ElementType& typeToGet) override;
    std::set<ProgramElement> getRightSide(const ProgramElement& leftSide, const ElementType& typeToGet) override;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_AFFECTSGETTER_H_
