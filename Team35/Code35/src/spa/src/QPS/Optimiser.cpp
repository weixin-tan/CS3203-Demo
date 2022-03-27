#include "Optimiser.h"

Optimiser::Optimiser() = default;

std::vector<ResultGroup> Optimiser::optimise(std::vector<ResultGroup> resultsGroups) {
    std::vector<ResultGroup> groups;
    for (const auto& r : resultsGroups) {
        if (!r.getGroup().empty()) {
            groups.push_back(r);
        }
    }
    return groups;
}
