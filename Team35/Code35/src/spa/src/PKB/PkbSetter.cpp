#include "PkbSetter.h"
#include "../Type.h"
#include "../ParsedStatement.h"

# define NULL_STMT_NO -1

PkbSetter::PkbSetter(DB *db) : db(db) {
  PkbSetter::spTypeToQpsTypeTable = {
      {StatementType::kassign_stmt, EntityType::Assignment},
      {StatementType::kprint_stmt,  EntityType::Print},
      {StatementType::kcall_stmt,   EntityType::Call},
      {StatementType::kif_stmt,     EntityType::If},
      {StatementType::kwhile_stmt,  EntityType::While},
      {StatementType::kread_stmt,   EntityType::Read}
  };
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
  db->stmtTypeTable[parsedStatement.stmt_no] = spTypeToQpsTypeTable[parsedStatement.statement_type];
}

void PkbSetter::handleParent(const ParsedStatement& parsedStatement) {
  if (parsedStatement.if_line_no != NULL_STMT_NO) {
    db->parentToChildTable[parsedStatement.if_line_no].insert(parsedStatement.stmt_no);
    db->childToParentTable[parsedStatement.stmt_no] = parsedStatement.if_line_no;
  }
  if (parsedStatement.while_line_no != NULL_STMT_NO) {
    db->parentToChildTable[parsedStatement.while_line_no].insert(parsedStatement.stmt_no);
    db->childToParentTable[parsedStatement.stmt_no] = parsedStatement.while_line_no;
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
