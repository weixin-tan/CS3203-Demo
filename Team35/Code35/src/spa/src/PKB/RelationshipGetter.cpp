#include "RelationshipGetter.h"

bool RelationshipGetter::isStatementTypeToGet(const ElementType& typeToGet, const ElementType& targetType) {
    return (typeToGet == ElementType::STATEMENT) || (typeToGet == targetType);
}

void RelationshipGetter::insertStmtElement(std::set<ProgramElement*>& result, int stmtNo, const ElementType& typeToGet) {
    DB*(db);
    if (isStatementTypeToGet(typeToGet, db->elementStmtTable.at(stmtNo).elementType))
        result.insert(&db->elementStmtTable.at(stmtNo));
}
