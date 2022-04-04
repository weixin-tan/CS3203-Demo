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
    std::list<std::string> formatResult(FormattedResult finalResult);

private:
<<<<<<< Updated upstream
    std::list<std::string> handleSingle(Entity entity, std::vector<ProgramElement> elements);
=======
    std:: list <std:: string> handleSingle(Entity entity, std:: vector <ProgramElement> elements);
>>>>>>> Stashed changes
    std::list<std::string> handleTuple(std::vector<Entity> entities, std::vector<std::vector<ProgramElement>> lists);
};

#endif //SPA_RESULTFORMATTER_H
