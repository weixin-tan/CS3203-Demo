#ifndef SPA_PKBSETTER_H
#define SPA_PKBSETTER_H

#include "DB.h"
#include "../ParsedStatement.h"
#include "../StatementType.h"
#include "ElementType.h"
#include "DesignExtractor.h"
#include "PkbValidator.h"
#include <map>

class PkbSetter {
private:
    DB* db;
    DesignExtractor designExtractor;
    PkbValidator pkbValidator;
    static const std::map<StatementType, ElementType> spTypeToElementTypeTable;

private:
    void insertStmt(const ParsedStatement& parsedStatement);

    void handleVariables(const ParsedStatement& parsedStatement);
    void handleProcedure(const ParsedStatement& parsedStatement);
    void handleStatementType(const ParsedStatement& parsedStatement);
    void handleConstants(const ParsedStatement &statement);

    void handleParent(const ParsedStatement& parsedStatement);
    void handleUses(const ParsedStatement& parsedStatement);
    void handleModifies(const ParsedStatement& parsedStatement);

public:
    explicit PkbSetter(DB* db);
    void insertStmts(const std::vector<std::vector<ParsedStatement>>& procedures);
};


#endif //SPA_PKBSETTER_H
