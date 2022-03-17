#include "ResultFormatter.h"

ResultFormatter::ResultFormatter() = default;

std::list<std::string> ResultFormatter::formatResult(const std::vector<ProgramElement>& finalResult) {
    std::list<std::string> result;
    for(const ProgramElement& e : finalResult)
        result.push_back(e.toString());  // toString can take in EntityAttributeType to specify
    return result;
}
