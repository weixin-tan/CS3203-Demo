#include "Optimiser.h"

Optimiser::Optimiser() = default;

std::vector<Group> Optimiser::optimize(std::vector<Result> results) {
    std::vector<Group> groups;

    std::vector<Result> firstResult;
    firstResult.push_back(results[0]);
    Group firstGroup;
    firstGroup.setGroup(firstResult);

    results.erase(results.begin());
    Group secondGroup;
    secondGroup.setGroup(results);

    groups.push_back(firstGroup);
    groups.push_back(secondGroup);
    return groups;
}
