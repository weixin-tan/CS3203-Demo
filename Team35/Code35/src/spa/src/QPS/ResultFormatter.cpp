#include "ResultFormatter.h"

ResultFormatter::ResultFormatter() = default;

std::list<std::string> ResultFormatter::formatResult(const FormattedResult& finalResult) {
    std::list<std::string> result;
    if (!finalResult.getValid()) {
        if (finalResult.getBoolReturn()) {
            result.emplace_back("FALSE");
        }
    } else if (finalResult.getBoolReturn()) {
        result.emplace_back("TRUE");
    } else {
        result = extractTableInformation(finalResult.getEntityList(), finalResult.getFinalTable());
    }
    return result;
}

std::list<std::string> ResultFormatter::extractTableInformation(const std::vector<Entity>& entities,
                                                                const Table& table) {
    std::list<std::string> result;
    for (const auto& row : table.rows) {
        std::string str = extractTableRowInformation(entities, row);
        result.emplace_back(str);
    }
    return result;
}

std::string ResultFormatter::extractTableRowInformation(const std::vector<Entity>& entities,
                                                        const TableRow& tableRow) {
    std::string str;
    for (const auto& entity : entities) {
        Entity scrubbedEntity = entity;
        scrubbedEntity.aType = EntityAttributeType::NULL_ATTRIBUTE;
        ProgramElement* e = tableRow.row.at(scrubbedEntity);
        if (entity.aType == EntityAttributeType::NULL_ATTRIBUTE) {
            str += e->toString();
        } else {
            str += e->toString(entity.aType);
        }
        str += " ";
    }
    str.pop_back();
    return str;
}
