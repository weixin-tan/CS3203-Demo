#include "TableRow.h"

// Combining a row together. We use the boolean to see if it is correct. 
std::pair<bool, TableRow> TableRow::combineRow(TableRow row1, TableRow row2) {
    for (const auto&[entity, element] : row1.row) {
        // If there are no entities in the row, we continue. 
        if (row2.row.count(entity) == 0) continue;
        // If there the row does not match, we just return false (to indicate its invalid)
        if (!(element == row2.row.at(entity))) return {false, {}};
    }
    // just combine using a map! This will form a larger table. 
    std::map<Entity, ProgramElement> newRow;
    for (const auto& entityElemPair : row1.row)
        newRow.insert(entityElemPair);
    for (const auto& entityElemPair : row2.row)
        newRow.insert(entityElemPair);
    return {true, {newRow}};
}

// for comparing elements in map/set. 
bool TableRow::operator<(TableRow row1) const {
    return row < row1.row;
}