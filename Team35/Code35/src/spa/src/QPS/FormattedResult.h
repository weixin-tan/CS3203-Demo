#ifndef SPA_FORMATTEDRESULT_H
#define SPA_FORMATTEDRESULT_H

#include "Entity.h"
#include "Type.h"
#include "../PKB/ProgramElement.h"

class FormattedResult {
private:
    bool valid;
    FormattedResultType resultType;
    std::vector<Entity> entityList;
    std::vector<std::vector<ProgramElement>> programElementLists;
public:
    FormattedResult();
    bool getValid();
    FormattedResultType getResultType();
    std::vector<Entity> getEntityList();
    std::vector<std::vector<ProgramElement>> getProgramElementLists();

    void setValid(bool b);
    void setResultType(FormattedResultType f);
    void setEntityList(std::vector<Entity> entities);
    void setProgramElementsLists(std::vector<std::vector<ProgramElement>> lists);
};


#endif //SPA_FORMATTEDRESULT_H
