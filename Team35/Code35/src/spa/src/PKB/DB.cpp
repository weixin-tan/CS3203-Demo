#include "DB.h"

DB::DB() = default;

void DB::clearCache() {
    nextTTable.clear();
    nextTTableR.clear();
    computedNextTSrc.clear();
    computedNextTRSrc.clear();

    affectsTable.clear();
    affectsTableR.clear();
    computedAffectsSrc.clear();
    computedAffectsRSrc.clear();
}
