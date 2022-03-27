#include "ResultFormatter.h"

ResultFormatter::ResultFormatter() = default;

std::list<std::string> ResultFormatter::formatResult(FormattedResult finalResult) {
    std::list<std::string> result;
    if (!finalResult.getValid()) {
        if (finalResult.getResultType() == FormattedResultType::BOOLEAN) {
            result.emplace_back("FALSE");
        }
    } else if (finalResult.getResultType() == FormattedResultType::BOOLEAN) {
        result.emplace_back("TRUE");
    } else if (finalResult.getResultType() == FormattedResultType::SINGLE) {
        result = handleSingle(finalResult.getEntityList()[0], finalResult.getProgramElementLists()[0]);
        return result;
    } else if (finalResult.getResultType() == FormattedResultType::TUPLE) {
        result = handleTuple(finalResult.getEntityList(), finalResult.getProgramElementLists());

    } else {

    }
    return result;
}

std::list<std::string> ResultFormatter::handleSingle(Entity entity, std::vector<ProgramElement> elements) {
    std::list<std::string> result;
    if (entity.aType == EntityAttributeType::NULL_ATTRIBUTE) {
        for (const ProgramElement& e : elements) {
            result.push_back(e.toString());
        }
    } else {
        for (const ProgramElement& e : elements) {
            result.push_back(e.toString(entity.aType));
        }
    }
    return result;
}

std::list<std::string>
ResultFormatter::handleTuple(std::vector<Entity> entities, std::vector<std::vector<ProgramElement>> lists) {
    std::list<std::string> result;
    for (int i = 0; i < lists[0].size(); i++) {
        std::string str;
        for (int j = 0; j < entities.size(); j++) {
            if (entities[j].aType == EntityAttributeType::NULL_ATTRIBUTE) {
                str += lists[j][i].toString();
            } else {
                str += lists[j][i].toString(entities[j].aType);
            }
            if (j != entities.size() - 1) {
                str += " ";
            }
        }
        result.push_back(str);
    }
    return result;
}
