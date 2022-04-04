#ifndef SPA_FORMATTEDRESULT_H
#define SPA_FORMATTEDRESULT_H

#include <vector>

#include "Entity.h"
#include "Table.h"
#include "Type.h"
#include "../PKB/ProgramElement.h"
#include "vector"

class FormattedResult {
private:
    bool valid;
    bool isBoolReturn;
    std::vector<Entity> entityList;
    Table finalTable;
public:
    FormattedResult();
    bool getValid() const;
    bool getBoolReturn() const;
    std::vector<Entity> getEntityList() const;
    Table getFinalTable() const;

    void setValid(bool b);
    void setBoolReturn(bool b);
    void setEntityList(std::vector<Entity> entities);
    void setFinalTable(Table table);
};


#endif //SPA_FORMATTEDRESULT_H
