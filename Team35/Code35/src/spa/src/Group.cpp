#include "Group.h"

Group::Group() {
  Group::results = {};
}

void Group::setGroup(std::vector<Result> r) {
  Group::results = r;
}

std::vector<Result> Group::getGroup() {
    return Group::results;
}
