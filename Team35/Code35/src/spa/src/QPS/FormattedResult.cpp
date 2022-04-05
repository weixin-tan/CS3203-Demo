#include "FormattedResult.h"

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
    FormattedResult::entityList = entities;
}

void FormattedResult::setFinalTable(Table table) {
    FormattedResult::finalTable = table;
}
