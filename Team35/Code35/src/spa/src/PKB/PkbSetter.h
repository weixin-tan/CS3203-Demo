#ifndef SPA_PKBSETTER_H
#define SPA_PKBSETTER_H

#include "DB.h"
#include "../SP/ParsedStatement.h"
#include "../SP/StatementType.h"
#include "ElementType.h"
#include "../DesignExtractor/DesignExtractor.h"
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
    static ProgramElement convertParsedStatement(const ParsedStatement& parsedStatement);
    void handleVariables(const ParsedStatement& parsedStatement);
    void handleProcedure(const ParsedStatement& parsedStatement);
    void handleConstants(const ParsedStatement &statement);
    void handleExpression(const ParsedStatement& statement);

public:
    explicit PkbSetter(DB* db);
    // TODO: change this to use global debug flag
    void insertStmts(const std::vector<std::vector<ParsedStatement>>& procedures, bool testing = false);
};


#endif //SPA_PKBSETTER_H
