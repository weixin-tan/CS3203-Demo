#include "TableRow.h"
#include <vector>

// this is required to label all the tablerows present. 
unsigned int TableRow::counter = 0; 

// creating a tablerow
TableRow::TableRow(std::unordered_map<Entity, ProgramElement, EntityHashFunction> row) {
    tableId = counter; 
    // increasing the counter by 1 to give each row a unique id. 
    counter++; 
    this->row = row; 
}

// Creating an empty tablerow. 
TableRow::TableRow() {
    tableId = counter; 
    counter++;
    this->row = {};
}

// Combining a row together. We use the boolean to see if it is correct. 
// optimises by merging the rows that are the shortest together. 
std::pair<bool, TableRow> TableRow::combineRow(TableRow const *row1, TableRow const *row2) {
    for (const auto&[entity, element] : row2->row) {
        // If there are no entities in the row, we continue. 
        if (row1->row.count(entity) == 0) continue;
        // If there the row does not match, we just return false (to indicate its invalid)
        if (!(element == row1->row.at(entity))) return {false, TableRow()};
    }
    // just combine using a map! This will form a larger table. 
    std::unordered_map<Entity, ProgramElement, EntityHashFunction> newRow;
    for (const auto& entityElemPair : row1->row)
        newRow.insert(entityElemPair);
    for (const auto& entityElemPair : row2->row)
        newRow.insert(entityElemPair);
    return {true, {newRow}};
}

// Filters the entities to get only the tableRow with the following entites. 
TableRow TableRow::filterRow(TableRow const *row, const std::vector<Entity> &entities) {
    std::unordered_map<Entity, ProgramElement, EntityHashFunction> newRow;
    for (const auto& entity : entities) {
        if (row->row.find(entity) != row->row.end()) {
            newRow.emplace(entity, row->row.at(entity));
        }
        else {
            return TableRow();
        }
    }
    return newRow;
}

// for comparing elements for map
bool TableRow::operator<(TableRow row1) const {
    return this->tableId < row1.tableId;
}

bool TableRow::operator==(TableRow row1) const {
    return this->tableId == row1.tableId;
}


size_t TableRowHash::operator()(const TableRow& tableRow) const
{
    return std::hash<int>()(tableRow.tableId);
}