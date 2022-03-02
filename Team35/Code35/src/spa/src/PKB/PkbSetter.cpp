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

PkbSetter::PkbSetter(DB *db) : db(db) {}

void PkbSetter::handleVariables(const ParsedStatement& parsedStatement) {
    for (const auto& var: parsedStatement.var_modified)
        db->variables.insert(var);
    for (const auto& var: parsedStatement.var_used)
        db->variables.insert(var);
}

void PkbSetter::handleProcedure(const ParsedStatement& parsedStatement) {
    db->procedures.insert(parsedStatement.procedure_name);
}

void PkbSetter::handleModifies(const ParsedStatement& parsedStatement) {
    for (const auto& var: parsedStatement.var_modified) {
        db->modifyStmtToVarTable[parsedStatement.stmt_no].insert(var);
        db->varToModifyStmtTable[var].insert(parsedStatement.stmt_no);
        db->modifyProcToVarTable[parsedStatement.procedure_name].insert(var);
        db->varToModifyProcTable[var].insert(parsedStatement.procedure_name);
    }
}

void PkbSetter::handleUses(const ParsedStatement& parsedStatement) {
    for (const auto& var: parsedStatement.var_used) {
        db->usesStmtToVarTable[parsedStatement.stmt_no].insert(var);
        db->varToUsesStmtTable[var].insert(parsedStatement.stmt_no);
        db->usesProcToVarTable[parsedStatement.procedure_name].insert(var);
        db->varToUsesProcTable[var].insert(parsedStatement.procedure_name);
    }
}

void PkbSetter::handleStatementType(const ParsedStatement& parsedStatement) {
    db->stmtTypeTable[parsedStatement.stmt_no] = spTypeToElementTypeTable.at(parsedStatement.statement_type);
}

void PkbSetter::handleConstants(const ParsedStatement& statement) {
    for (const std::string& c : statement.constant) {
        db->constantToStmtTable[c].insert(statement.stmt_no);
        db->usesStmtToConstantTable[statement.stmt_no].insert(c);
        db->constants.insert(c);
    }
}

void PkbSetter::handleParent(const ParsedStatement& parsedStatement) {
    int parentStmtNo = ((parsedStatement.while_stmt_no != ParsedStatement::default_null_stmt_no) ? parsedStatement.while_stmt_no : parsedStatement.if_stmt_no);
    db->childToParentTable[parsedStatement.stmt_no] = parentStmtNo;
    if (parentStmtNo != ParsedStatement::default_null_stmt_no) {
        db->parentToChildTable[parentStmtNo].insert(parsedStatement.stmt_no);
    }
}

void PkbSetter::handleFollows(const ParsedStatement& parsedStatement) {
    if (parsedStatement.stmt_no != ParsedStatement::default_null_stmt_no) {
        db->stmtPreceding[parsedStatement.stmt_no] = parsedStatement.preceding;
        db->stmtFollowing[parsedStatement.preceding] = parsedStatement.stmt_no;
    }
}

void PkbSetter::handleCalls(const ParsedStatement& parsedStatement) {
    db->stmtToProcedureCalled[parsedStatement.stmt_no] = parsedStatement.procedure_called;
}

void PkbSetter::insertStmt(const ParsedStatement& parsedStatement) {
    db->stmtTable[parsedStatement.stmt_no] = parsedStatement;
    // handle entity
    handleVariables(parsedStatement);
    handleConstants(parsedStatement);
    handleProcedure(parsedStatement);
    handleStatementType(parsedStatement);

    // handle relationships
    handleFollows(parsedStatement);
    handleParent(parsedStatement);
    handleUses(parsedStatement);
    handleModifies(parsedStatement);
    handleCalls(parsedStatement);
}

void PkbSetter::insertStmts(const std::vector<std::vector<ParsedStatement>>& procedures) {
    for (const auto& procedure : procedures)
        for (const auto& parsedStatement : procedure)
            insertStmt(parsedStatement);
}
