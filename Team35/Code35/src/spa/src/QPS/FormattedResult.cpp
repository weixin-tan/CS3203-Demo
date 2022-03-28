#include "FormattedResult.h"

FormattedResult::FormattedResult() {
    valid = false;
    resultType = FormattedResultType::NULL_RESULT;
}

bool FormattedResult::getValid() {
    return valid;
}

FormattedResultType FormattedResult::getResultType() {
    return resultType;
}

std::vector<Entity> FormattedResult::getEntityList() {
    return entityList;
}

std::vector<std::vector<ProgramElement>> FormattedResult::getProgramElementLists() {
    return programElementLists;
}

void FormattedResult::setValid(bool b) {
    valid = b;
}

void FormattedResult::setResultType(FormattedResultType f) {
    resultType = f;
}

void FormattedResult::setEntityList(std::vector<Entity> entities) {
    entityList = entities;
}

void FormattedResult::setProgramElementsLists(std::vector<std::vector<ProgramElement>> lists) {
    programElementLists = lists;
}



