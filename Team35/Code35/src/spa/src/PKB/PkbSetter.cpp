#include "PkbSetter.h"
#include "../ParsedStatement.h"
#include "../StatementType.h"
#include "ElementType.h"

const std::map<StatementType, ElementType> PkbSetter::spTypeToElementTypeTable = {
        {StatementType::kassign_stmt, ElementType::kAssignment},
        {StatementType::kprint_stmt,  ElementType::kPrint},
        {StatementType::kcall_stmt,   ElementType::kCall},
        {StatementType::kif_stmt,     ElementType::kIf},
        {StatementType::kwhile_stmt,  ElementType::kWhile},
        {StatementType::kread_stmt,   ElementType::kRead}
};

PkbSetter::PkbSetter(DB* db) : db(db), designExtractor(db), pkbValidator(db) {}

void PkbSetter::handleVariables(const ParsedStatement& parsedStatement) {
    for (const auto& var: parsedStatement.var_modified)
        db->variables.insert(var);
    for (const auto& var: parsedStatement.var_used)
        db->variables.insert(var);
}

void PkbSetter::handleProcedure(const ParsedStatement& parsedStatement) {
    db->procedures.insert(parsedStatement.procedure_name);
}

void PkbSetter::handleConstants(const ParsedStatement& statement) {
    for (const std::string& c : statement.constant) {
        db->constantToStmtTable[c].insert(statement.stmt_no);
        db->usesStmtToConstantTable[statement.stmt_no].insert(c);
        db->constants.insert(c);
    }
}

ProgramElement PkbSetter::convertParsedStatement(const ParsedStatement& statement) {
    ElementType elementType = spTypeToElementTypeTable.at(statement.statement_type);
    std::string procOrVarName = ProgramElement::nullStringValue;
    if (elementType == ElementType::kRead)
        procOrVarName = *statement.var_modified.begin();
    if (elementType == ElementType::kPrint)
        procOrVarName = *statement.var_used.begin();
    if (elementType == ElementType::kCall)
        procOrVarName = statement.procedure_called;
    return ProgramElement::createStatement(elementType, statement.stmt_no, procOrVarName);
}

void PkbSetter::insertStmt(const ParsedStatement& parsedStatement) {
    db->stmtTable[parsedStatement.stmt_no] = parsedStatement;
    db->elementStmtTable.insert({parsedStatement.stmt_no, convertParsedStatement(parsedStatement)});
    // handle entity
    handleVariables(parsedStatement);
    handleConstants(parsedStatement);
    handleProcedure(parsedStatement);
}

void PkbSetter::insertStmts(const std::vector<std::vector<ParsedStatement>>& procedures) {
    // insert legacy stuff, might be deprecated
    for (const auto& procedure : procedures)
        for (const auto& parsedStatement : procedure)
            insertStmt(parsedStatement);

    // extract design abstractions
    designExtractor.extractCalls(db->callsTable);
    DesignExtractor::computeReverse(db->callsTable, db->callsTableR);
    designExtractor.extractCallsT(db->callsTTable);
    DesignExtractor::computeReverse(db->callsTTable, db->callsTTableR);

    // validate design abstractions
    try {
        pkbValidator.validateNoCyclicCall();
        pkbValidator.validateCallsExists();
        PkbValidator::validateNoDuplicateProcedure(procedures);
    } catch (const std::exception& e) {
        std::cout <<
            "SIMPLE source semantic error detected. Details following:\n" <<
            e.what() << '\n';
        exit(1);
    }

    // extract design abstractions (these assume that data is clean)
    designExtractor.extractFollows(db->followsTable);
    DesignExtractor::computeReverse(db->followsTable, db->followsTableR);
    designExtractor.extractFollowsT(db->followsTTable);
    DesignExtractor::computeReverse(db->followsTTable, db->followsTTableR);
    designExtractor.extractParent(db->parentTable);
    DesignExtractor::computeReverse(db->parentTable, db->parentTableR);
    designExtractor.extractParentT(db->parentTTable);
    DesignExtractor::computeReverse(db->parentTTable, db->parentTTableR);
    designExtractor.extractModifiesP(db->modifiesPTable);
    DesignExtractor::computeReverse(db->modifiesPTable, db->modifiesPTableR);
    designExtractor.extractModifiesS(db->modifiesSTable);
    DesignExtractor::computeReverse(db->modifiesSTable, db->modifiesSTableR);
    designExtractor.extractUsesP(db->usesPTable);
    DesignExtractor::computeReverse(db->usesPTable, db->usesPTableR);
    designExtractor.extractUsesS(db->usesSTable);
    DesignExtractor::computeReverse(db->usesSTable, db->usesSTableR);
    designExtractor.extractNext(db->nextTable);
    DesignExtractor::computeReverse(db->nextTable, db->nextTableR);
}
