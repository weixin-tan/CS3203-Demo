#ifndef SPA_RESULT_H
#define SPA_RESULT_H

#include <set>
#include <utility>

#include "../PKB/ProgramElement.h"
#include "RelationshipRef.h"

class Result {
private:
    ResultType type;
    bool valid;
    Entity oneSynEntity;
    std::set<ProgramElement> oneSynSet;
    std::pair<Entity, Entity> twoSynEntities;
    std::set<std::pair<ProgramElement, ProgramElement>> twoSynSet;

public:
    Result();

    void setResultType(ResultType r);
    void setValid(bool b);
    void setOneSynEntity(Entity e);
    void setOneSynSet(std::set<ProgramElement> s);
    void setTwoSynEntities(std::pair<Entity, Entity> p);
    void setTwoSynSet(std::set<std::pair<ProgramElement, ProgramElement>> t);

    ResultType getResultType();
    bool getValid() const;
    Entity getOneSynEntity();
    std::set<ProgramElement> getOneSynSet() const;
    std::pair<Entity, Entity> getTwoSynEntities();
    std::set<std::pair<ProgramElement, ProgramElement>> getTwoSynSet() const;
    bool operator==(const Result& r1) const;
};

#endif //SPA_RESULT_H
