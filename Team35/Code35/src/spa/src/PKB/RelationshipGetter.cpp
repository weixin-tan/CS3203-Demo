#include "RelationshipGetter.h"


bool RelationshipGetter::isStatementTypeToGet(const ElementType& typeToGet, const ElementType& targetType) {
    return (typeToGet == ElementType::STATEMENT) || (typeToGet == targetType);
}

void RelationshipGetter::insertStmtElement(std::set<ProgramElement>& result, const ProgramElement& statement, const ElementType& typeToGet) {
    if (isStatementTypeToGet(typeToGet, statement.elementType))
        result.insert(statement);
}
