#ifndef SPA_RESULTFORMATTER_H
#define SPA_RESULTFORMATTER_H

#include <string>
#include <list>
#include <vector>

#include "FormattedResult.h"
#include "Result.h"

class ResultFormatter {
public:
    ResultFormatter();
    static std::list<std::string> formatResult(const FormattedResult& finalResult);

private:
    static std::list<std::string> extractTableInformation(const std::vector<Entity>& entities, const Table& table);
    static std::string extractTableRowInformation(const std::vector<Entity>& entities, const TableRow& tableRow);
};

#endif //SPA_RESULTFORMATTER_H

