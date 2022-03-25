#ifndef SPA_TABLE_H
#define SPA_TABLE_H

#include <vector>
#include "TableRow.h"
#include "Result.h"

class Table {
public:
    std::set<TableRow> rows;

public:
    explicit Table(Result r);
    Table(std::set<TableRow> rows);
    Table(Table t1, Table t2);
};

#endif //SPA_TABLE_H
