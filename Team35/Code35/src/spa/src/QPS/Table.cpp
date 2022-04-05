#include "Table.h"

Table::Table() = default;

Table::Table(const Result *r) {
    // Check if the result is valid. If not no table is created.
    if (!r->getValid()) return;

    // Get the One Synonym Set/ Two Synonym sets for program elements
    // in the result. 
    std::set<ProgramElement> oneSynSet = r->getOneSynSet();
    std::set<std::pair<ProgramElement, ProgramElement>> twoSynSet = r->getTwoSynSet();
    // Else, we will just add them individually into the table. 
    // The set will have pair indicating the entity and the element itself
    if (!oneSynSet.empty())
        for (const ProgramElement& elem : oneSynSet) {
            r->getOneSynEntity().clear_aType();
            rows.insert({ {std::make_pair(r->getOneSynEntity(), elem)} });
        }


    if (!twoSynSet.empty())
        for (const auto& [elem1, elem2] : twoSynSet) {
            r->getTwoSynEntities().first.clear_aType();
            r->getTwoSynEntities().second.clear_aType(); 
            rows.insert({ {
                std::make_pair(r->getTwoSynEntities().first, elem1),
                std::make_pair(r->getTwoSynEntities().second, elem2),
                } });
        }
}

Table::Table(std::unordered_set<TableRow, TableRowHash> rows) : rows(rows) {};


// Combining tables together.
// Optimises by merging on the one with fewer columns. 
Table::Table(const Table *t1, const Table *t2) {
    std::unordered_set<TableRow, TableRowHash> result;
    for (const auto &row1 : t1->rows) {
        for (const auto &row2 : t2->rows) {
            std::pair<bool, TableRow> newRow = TableRow::combineRow(&row1, &row2);
            // if it is valid, then we will insert into the result. 
            if (newRow.first) result.insert(newRow.second);
        }
    }
    rows = result;
}

// Getting specific columns out of table
Table Table::extractColumns(const std::vector<Entity> *entities) {
    std::unordered_set<TableRow, TableRowHash> result;
    for (const auto &row : rows) {
        TableRow newRow = TableRow::filterRow(&row, *entities);
        // Note that this will only happen at the VERY first iteration. 
        // The result will always be empty. 
        if (newRow.row.size() == 0) {
            return result;
        }
        else {
            result.insert(newRow);
        }
    }
    return result; 
}