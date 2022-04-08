#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_AFFECTSTEXTRACTOR_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_AFFECTSTEXTRACTOR_H_

#include "../PKB/DB.h"
#include "AffectsExtractor.h"

class AffectsTExtractor {
private:
    DB* db;
    AffectsExtractor affectsExtractor;

private:
    void dfsAffectsT(int src);
    void dfsAffectsTR(int src);

public:
    explicit AffectsTExtractor(DB* db);
    void extractAffectsT(int src);
    void extractAffectsTR(int src);
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_DESIGNEXTRACTOR_AFFECTSTEXTRACTOR_H_
