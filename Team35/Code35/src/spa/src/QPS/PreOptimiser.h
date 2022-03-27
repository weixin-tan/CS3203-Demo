#ifndef SPA_SRC_SPA_SRC_QPS_PREOPTIMISER_H_
#define SPA_SRC_SPA_SRC_QPS_PREOPTIMISER_H_

#include <vector>
#include <Clause.h>
#include "RelationshipRef.h"
#include "RelationshipRefGroup.h"
#include "GroupedClause.h"

class PreOptimiser {
public:
    PreOptimiser();
    GroupedClause optimise(const std::vector<Clause>& clauses);
};

#endif //SPA_SRC_SPA_SRC_QPS_PREOPTIMISER_H_
