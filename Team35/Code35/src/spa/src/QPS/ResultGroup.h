#ifndef SPA_GROUP_H
#define SPA_GROUP_H

#include <vector>

#include "Result.h"

class ResultGroup {
private:
    bool isBoolReturn;
    bool isValid;
    std::vector<Entity> entitiesToReturn;
    std::vector<std::vector<Result>> resultLists;
public:
    ResultGroup();
    void setBoolReturn(bool b);
    void setValid(bool b);
    void setEntitiesToReturn(std::vector<Entity> e);
    void addResultList(const std::vector<Result>& r);
    bool getIsBoolReturn() const;
    bool getIsValid() const;
    std::vector<Entity> getEntitiesToReturn() const;
    std::vector<std::vector<Result>> getResultLists() const;
};

#endif //SPA_GROUP_H
