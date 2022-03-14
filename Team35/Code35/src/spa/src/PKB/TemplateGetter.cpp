#include "TemplateGetter.h"


bool TemplateGetter::isStatementTypeToGet(const ElementType& typeToGet, const ElementType& targetType) {
    return (typeToGet == ElementType::kStatement) || (typeToGet == targetType);
}

void TemplateGetter::insertStmtElement(std::set<ProgramElement>& result, const ProgramElement& statement, const ElementType& typeToGet) {
    if (isStatementTypeToGet(typeToGet, statement.elementType))
        result.insert(statement);
}
