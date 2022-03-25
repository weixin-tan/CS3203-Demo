#include "TableRow.h"

std::pair<bool, TableRow> TableRow::combineRow(TableRow row1, TableRow row2) {
    for (const auto&[entity, element] : row1.row) {
        if (row2.row.count(entity) == 0) continue;
        if (!(element == row2.row.at(entity))) return {false, {}};
    }
    std::map<Entity, ProgramElement> newRow;
    for (const auto& entityElemPair : row1.row)
        newRow.insert(entityElemPair);
    for (const auto& entityElemPair : row2.row)
        newRow.insert(entityElemPair);
    return {true, {newRow}};
}

bool TableRow::operator<(TableRow row1) const {
    return row < row1.row;
}