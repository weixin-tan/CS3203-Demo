#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_SP_TABLEROW_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_SP_TABLEROW_H_

#include <vector>

#include "Entity.h"
#include "../PKB/ProgramElement.h"

class TableRow {

public:
    static unsigned int counter; 
    int tableId; 

    // Unordered map would use the entity hash function. 
    std::unordered_map<Entity, ProgramElement*, EntityHashFunction> row;
    TableRow(std::unordered_map<Entity, ProgramElement*, EntityHashFunction> row);
    TableRow();

public:
    static std::pair<bool, TableRow> combineRow(TableRow const *row1, TableRow const *row2);
    static TableRow filterRow(TableRow const * row, const std::vector<Entity> &entities);
    bool operator<(TableRow row1) const;
    bool operator==(TableRow row1) const;
};

class TableRowHash {
public: 
    size_t operator()(const TableRow& tableRow) const;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_SP_TABLEROW_H_
