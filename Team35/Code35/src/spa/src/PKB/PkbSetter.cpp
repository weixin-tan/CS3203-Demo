#include "PkbSetter.h"
#include "../SP/ParsedStatement.h"
#include "../SP/StatementType.h"
#include "ElementType.h"

const std::map<StatementType, ElementType> PkbSetter::spTypeToElementTypeTable = {
        {StatementType::ASSIGNMENT_STMT, ElementType::ASSIGNMENT},
        {StatementType::PRINT_STMT,  ElementType::PRINT},
        {StatementType::CALL_STMT,   ElementType::CALL},
        {StatementType::IF_STMT,     ElementType::IF},
        {StatementType::WHILE_STMT,  ElementType::WHILE},
        {StatementType::READ_STMT,   ElementType::READ}
};

PkbSetter::PkbSetter(DB* db) : db(db), designExtractor(db), pkbValidator(db) {}

void PkbSetter::handleVariables(const ParsedStatement& parsedStatement) {
    for (const auto& var: parsedStatement.varModified)
        db->variables.insert(var);
    for (const auto& var: parsedStatement.varUsed)
        db->variables.insert(var);
}

void PkbSetter::handleProcedure(const ParsedStatement& parsedStatement) {
    db->procedures.insert(parsedStatement.procedureName);
}

void PkbSetter::handleConstants(const ParsedStatement& statement) {
    for (const std::string& c : statement.constant) {
        db->constantToStmtTable[c].insert(statement.stmtNo);
        db->usesStmtToConstantTable[statement.stmtNo].insert(c);
        db->constants.insert(c);
    }
}

void PkbSetter::handleExpression(const ParsedStatement& statement) {
    db->exprTable[statement.stmtNo] = statement.pattern;
}

ProgramElement PkbSetter::convertParsedStatement(const ParsedStatement& statement) {
    ElementType elementType = spTypeToElementTypeTable.at(statement.statementType);
    std::string procOrVarName = ProgramElement::nullStringValue;
    if (elementType == ElementType::READ)
        procOrVarName = *statement.varModified.begin();
    if (elementType == ElementType::PRINT)
        procOrVarName = *statement.varUsed.begin();
    if (elementType == ElementType::CALL)
        procOrVarName = statement.procedureCalled;
    return ProgramElement::createStatement(elementType, statement.stmtNo, procOrVarName);
}

void PkbSetter::insertStmt(const ParsedStatement& parsedStatement) {
    db->stmtTable[parsedStatement.stmtNo] = parsedStatement;
    db->elementStmtTable.insert({parsedStatement.stmtNo, convertParsedStatement(parsedStatement)});
    // handle entity
    handleVariables(parsedStatement);
    handleConstants(parsedStatement);
    handleProcedure(parsedStatement);
    handleExpression(parsedStatement);
}

void PkbSetter::insertStmts(const std::vector<std::vector<ParsedStatement>>& procedures, bool testing) {
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
        if (testing) throw e;  // testing purpose
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
