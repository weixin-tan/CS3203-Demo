#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_SP_TABLEROW_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_SP_TABLEROW_H_

#include "Entity.h"
#include "../PKB/ProgramElement.h"

class TableRow {
public:
    std::map<Entity, ProgramElement> row;

public:
    static std::pair<bool, TableRow> combineRow(TableRow row1, TableRow row2);
    bool operator<(TableRow row1) const;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_SP_TABLEROW_H_
