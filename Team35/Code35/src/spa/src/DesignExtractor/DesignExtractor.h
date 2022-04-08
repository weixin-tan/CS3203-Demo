#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_DESIGNEXTRACTOR_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_DESIGNEXTRACTOR_H_

#include "../PKB/DB.h"
#include "PrecomputableRelationshipExtractor.h"
#include "ModifiesSExtractor.h"
#include "ModifiesPExtractor.h"
#include "UsesPExtractor.h"
#include "UsesSExtractor.h"
#include "FollowsExtractor.h"
#include "FollowsTExtractor.h"
#include "ParentExtractor.h"
#include "ParentTExtractor.h"
#include "CallsExtractor.h"
#include "CallsTExtractor.h"
#include "NextExtractor.h"

class DesignExtractor {
private:
    DB* db;
    ModifiesSExtractor modifiesSExtractor;
    ModifiesPExtractor modifiesPExtractor;
    UsesSExtractor usesSExtractor;
    UsesPExtractor usesPExtractor;
    FollowsExtractor followsExtractor;
    FollowsTExtractor followsTExtractor;
    ParentExtractor parentExtractor;
    ParentTExtractor parentTExtractor;
    CallsExtractor callsExtractor;
    CallsTExtractor callsTExtractor;
    NextExtractor nextExtractor;
    std::vector<PrecomputableRelationshipExtractor*> precomputableExtractors;


public:
    explicit DesignExtractor(DB* db);
    void precompute();
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_DESIGNEXTRACTOR_H_
