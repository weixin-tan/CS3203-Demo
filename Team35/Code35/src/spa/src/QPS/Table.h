#ifndef SPA_TABLE_H
#define SPA_TABLE_H

#include <vector>
#include <unordered_set>
#include "TableRow.h"
#include "Result.h"

class Table {
public:

    // Requires TableRowHash
    std::unordered_set<TableRow, TableRowHash> rows;

public:
    explicit Table(Result r);
    Table(std::unordered_set<TableRow, TableRowHash> rows);
    Table(Table t1, Table t2);
};

#endif //SPA_TABLE_H
