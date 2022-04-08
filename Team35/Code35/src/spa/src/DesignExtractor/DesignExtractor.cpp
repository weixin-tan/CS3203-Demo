#include "DesignExtractor.h"

DesignExtractor::DesignExtractor(DB* db) :
    db(db),
    modifiesSExtractor(db),
    modifiesPExtractor(db),
    usesSExtractor(db),
    usesPExtractor(db),
    followsExtractor(db),
    followsTExtractor(db),
    parentExtractor(db),
    parentTExtractor(db),
    callsExtractor(db),
    callsTExtractor(db),
    nextExtractor(db),
    precomputableExtractors({
                                    &callsExtractor,
                                    &callsTExtractor,
                                    &followsExtractor,
                                    &followsTExtractor,
                                    &parentExtractor,
                                    &parentTExtractor,
                                    &modifiesPExtractor,
                                    &modifiesSExtractor,
                                    &usesPExtractor,
                                    &usesSExtractor,
                                    &nextExtractor
                            })
{}

void DesignExtractor::precompute() {
    for (auto extractor : precomputableExtractors)
        extractor->precomputeRelationship();
}
