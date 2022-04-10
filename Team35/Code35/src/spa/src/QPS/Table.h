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
    Table();
    explicit Table(const Result* r);
    Table(std::unordered_set<TableRow, TableRowHash> rows);
    Table(const Table* t1, const Table* t2);
    Table extractColumns(const std::vector<Entity>* entites);
};

#endif //SPA_TABLE_H
