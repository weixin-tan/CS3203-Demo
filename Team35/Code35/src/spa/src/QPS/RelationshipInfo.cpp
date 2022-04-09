#include "RelationshipInfo.h"

RelationshipInfo::RelationshipInfo(int visitedNum, int commonSyn, int fixedEntities, int ease, int relationshipNum) {
    visited = visitedNum;
    numberOfCommonSynoymns = commonSyn;
    numberOfFixedEntities = fixedEntities;
    relationshipEase = ease;
    relationshipNumber = relationshipNum;
}

std::string RelationshipInfo::toString() const {

    std::ostringstream buffer;
    buffer << "visited: " << visited
           << " numberOfCommonSynoymns: " << numberOfCommonSynoymns
           << " numberOfFixedEntities: " << numberOfFixedEntities
           << " relationshipEase: " << relationshipEase
           << " relationshipNumber: " << relationshipNumber;

    return buffer.str();
}
