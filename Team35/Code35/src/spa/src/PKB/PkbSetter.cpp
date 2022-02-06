#include "PkbSetter.h"
#include "../ParsedStatement.h"
#include "../StatementType.h"

PkbSetter::PkbSetter(DB *db) : db(db) {}

void PkbSetter::handleModifies(const ParsedStatement& parsedStatement) {
  for (const auto& var: parsedStatement.var_modified) {
    db->modifyStmtToVarTable[parsedStatement.stmt_no].insert(var);
    db->varToModifyStmtTable[var].insert(parsedStatement.stmt_no);
  }
}

//TODO: figure out how to change map to static
void PkbSetter::handleStatementType(const ParsedStatement& parsedStatement) {
  std::map<StatementType, EntityType> spTypeToQpsType = {
      {StatementType::kassign_stmt, EntityType::Assignment},
      {StatementType::kprint_stmt,  EntityType::Print},
      {StatementType::kcall_stmt,   EntityType::Call},
      {StatementType::kif_stmt,     EntityType::If},
      {StatementType::kwhile_stmt,  EntityType::While},
      {StatementType::kread_stmt,   EntityType::Read}
  };
  db->stmtTypeTable[parsedStatement.stmt_no] = spTypeToQpsType[parsedStatement.statement_type];
}

void PkbSetter::handleVariables(const ParsedStatement& parsedStatement) {
  for (const auto& var: parsedStatement.var_modified)
    db->variables.insert(var);
}

void PkbSetter::insertStmt(const ParsedStatement& parsedStatement) {
  handleVariables(parsedStatement);
  handleStatementType(parsedStatement);
  handleModifies(parsedStatement);
}
