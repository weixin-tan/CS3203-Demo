#include "ResultGroup.h"

ResultGroup::ResultGroup() {
    ResultGroup::results = {};
}

void ResultGroup::setGroup(std::vector<Result> r) {
    ResultGroup::results = r;
}

std::vector<Result> ResultGroup::getGroup() const {
    return ResultGroup::results;
}
