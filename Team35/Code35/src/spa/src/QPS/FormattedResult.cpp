#include "FormattedResult.h"

FormattedResult::FormattedResult() {
    valid = false;
    resultType = FormattedResultType::NULL_RESULT;
    entityList = {};
    programElementLists = {};
}

bool FormattedResult::getValid() {
    return FormattedResult::valid;
}

FormattedResultType FormattedResult::getResultType() {
    return FormattedResult::resultType;
}

std::vector<Entity> FormattedResult::getEntityList() {
    return FormattedResult::entityList;
}

std::vector<std::vector<ProgramElement>> FormattedResult::getProgramElementLists() {
    return FormattedResult::programElementLists;
}

void FormattedResult::setValid(bool b) {
    FormattedResult::valid = b;
}

void FormattedResult::setResultType(FormattedResultType f) {
    FormattedResult::resultType = f;
}

void FormattedResult::setEntityList(std::vector<Entity> entities) {
    FormattedResult::entityList = entities;
}

void FormattedResult::setProgramElementsLists(std::vector<std::vector<ProgramElement>> lists) {
    FormattedResult::programElementLists = lists;
}
