#include "PostOptimiser.h"

PostOptimiser::PostOptimiser() = default;

std::vector<ResultGroup> PostOptimiser::optimise(std::vector<ResultGroup> resultGroups) {
    if (resultGroups.size() == 1) {
        return resultGroups;
    }
    std::vector<ResultGroup> groups;
    groups.push_back(resultGroups[0]);
    resultGroups.erase(resultGroups.begin());
    for (const auto& r : resultGroups) {
        std::vector<Result> results = r.getGroup();
        std::vector<Result> optimisedResults;
        for (Result result : results) {
            if (!(result.getValid() && result.getOneSynSet().empty() && result.getTwoSynSet().empty())) {
                if (result.getOneSynSet().empty()) {
                    std::pair<Entity, Entity> entities = result.getTwoSynEntities();
                    entities.first.aType = EntityAttributeType::NULL_ATTRIBUTE;
                    entities.second.aType = EntityAttributeType::NULL_ATTRIBUTE;
                    result.setTwoSynEntities(entities);
                }
                if (result.getTwoSynSet().empty()) {
                    Entity entity = result.getOneSynEntity();
                    entity.aType = EntityAttributeType::NULL_ATTRIBUTE;
                    result.setOneSynEntity(entity);
                }
                optimisedResults.push_back(result);
            }
        }
        if (!optimisedResults.empty()) {
            ResultGroup resultGroup;
            resultGroup.setGroup(optimisedResults);
            groups.push_back(resultGroup);
        }
    }
    return groups;
}
