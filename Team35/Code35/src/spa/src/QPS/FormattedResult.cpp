#include "FormattedResult.h"

#include <utility>

FormattedResult::FormattedResult() {
    valid = false;
    isBoolReturn = false;
    entityList = {};
    const Result result = Result();
    finalTable = Table(&result);
}

bool FormattedResult::getValid() const {
    return FormattedResult::valid;
}

bool FormattedResult::getBoolReturn() const {
    return FormattedResult::isBoolReturn;
}

std::vector<Entity> FormattedResult::getEntityList() const {
    return FormattedResult::entityList;
}

Table FormattedResult::getFinalTable() const {
    return FormattedResult::finalTable;
}

void FormattedResult::setValid(bool b) {
    FormattedResult::valid = b;
}

void FormattedResult::setBoolReturn(bool b) {
    FormattedResult::isBoolReturn = b;
}

void FormattedResult::setEntityList(std::vector<Entity> entities) {
    FormattedResult::entityList = std::move(entities);
}

void FormattedResult::setFinalTable(Table table) {
    FormattedResult::finalTable = std::move(table);
}

bool FormattedResult::operator==(const FormattedResult &f1) const {
    return FormattedResult::isBoolReturn == f1.isBoolReturn &&
           FormattedResult::valid == f1.valid &&
           FormattedResult::entityList == f1.entityList &&
           FormattedResult::finalTable.rows.size() == f1.finalTable.rows.size();
}
