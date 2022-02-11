#include "PkbSetter.h"
#include "../ParsedStatement.h"
#include "../StatementType.h"
#include "ElementType.h"

# define NULL_STMT_NO -1

const std::map<StatementType, ElementType> PkbSetter::spTypeToElementTypeTable = {
    {StatementType::kassign_stmt, ElementType::Assignment},
    {StatementType::kprint_stmt,  ElementType::Print},
    {StatementType::kcall_stmt,   ElementType::Call},
    {StatementType::kif_stmt,     ElementType::If},
    {StatementType::kwhile_stmt,  ElementType::While},
    {StatementType::kread_stmt,   ElementType::Read}
};

PkbSetter::PkbSetter(DB *db) : db(db) {
}

void PkbSetter::handleVariables(const ParsedStatement& parsedStatement) {
  for (const auto& var: parsedStatement.var_modified)
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

void PkbSetter::handleParent(const ParsedStatement& parsedStatement) {
  int parentStmtNo = ((parsedStatement.while_line_no != NULL_STMT_NO) ? parsedStatement.while_line_no : parsedStatement.if_line_no);
  db->childToParentTable[parsedStatement.stmt_no] = parentStmtNo;
  if (parentStmtNo != NULL_STMT_NO) {
    db->parentToChildTable[parentStmtNo].insert(parsedStatement.stmt_no);
  }
}

void PkbSetter::handleFollows(const ParsedStatement& parsedStatement) {
  if (parsedStatement.stmt_no != NULL_STMT_NO) {
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
  handleProcedure(parsedStatement);
  handleStatementType(parsedStatement);

  // handle relationships
  handleFollows(parsedStatement);
  handleParent(parsedStatement);
  handleUses(parsedStatement);
  handleModifies(parsedStatement);
  handleCalls(parsedStatement);
}
