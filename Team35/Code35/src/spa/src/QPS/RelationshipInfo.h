#ifndef SPA_SRC_SPA_SRC_QPS_RELATIONSHIPINFO_H_
#define SPA_SRC_SPA_SRC_QPS_RELATIONSHIPINFO_H_

#include <string>
#include <sstream>
class RelationshipInfo {

public:
    RelationshipInfo(int visitedNum, int commonSyn, int fixedEntities, int ease, int relationshipNum);
    int visited;
    int numberOfCommonSynoymns;
    int numberOfFixedEntities;
    int relationshipEase;
    int relationshipNumber;

    std::string toString() const;
};

#endif //SPA_SRC_SPA_SRC_QPS_RELATIONSHIPINFO_H_
