#include "PkbSetter.h"
#include "../Type.h"
#include "parsed_statement_temp.h"  // TODO: delete when ParsedStatement is implemented

PkbSetter::PkbSetter(DB *db) : db(db) {}

void PkbSetter::handleModifies(const ParsedStatement& parsedStatement) {
  for (const auto& var: parsedStatement.varModified) {
    db->modifyStmtToVarTable[parsedStatement.stmtNo].insert(var);
    db->varToModifyStmtTable[var].insert(parsedStatement.stmtNo);
  }
}

void PkbSetter::handleStatementType(const ParsedStatement& parsedStatement) {
  db->stmtTypeTable[parsedStatement.stmtNo] = SP_TYPE_ENTITY_TYPE_MAP[parsedStatement.statementType];  // pending SP
}

void PkbSetter::handleVariables(const ParsedStatement &parsedStatement) {
  for (const auto& var: parsedStatement.varModified)
    db->variables.insert(var);
}

void PkbSetter::insertStmt(const ParsedStatement& parsedStatement) {
  handleVariables(parsedStatement);
  handleStatementType(parsedStatement);
  handleModifies(parsedStatement);
}
