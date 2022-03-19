#include "Table.h"

Table::Table(Result r) {
    if (!r.getValid()) return;
    std::set<ProgramElement> oneSynSet = r.getOneSynSet();
    std::set<std::pair<ProgramElement, ProgramElement>> twoSynSet = r.getTwoSynSet();
    if (oneSynSet.empty() && twoSynSet.empty())
        rows.insert({});
    if (!oneSynSet.empty())
        for (const ProgramElement& elem : oneSynSet)
            rows.insert({{std::make_pair(r.getOneSynEntity(), elem)}});
    if (!twoSynSet.empty())
        for (const auto& [elem1, elem2] : twoSynSet)
            rows.insert({{
                std::make_pair(r.getTwoSynEntities().first, elem1),
                std::make_pair(r.getTwoSynEntities().second, elem2),
                }});
}

Table::Table(std::set<TableRow> rows) : rows(rows) {};

Table::Table(Table t1, Table t2) {
    std::set<TableRow> result;
    for (const auto row1 : t1.rows) {
        for (const auto row2 : t2.rows) {
            std::pair<bool, TableRow> newRow = TableRow::combineRow(row1, row2);
            if (newRow.first) result.insert(newRow.second);
        }
    }
    rows = result;
}