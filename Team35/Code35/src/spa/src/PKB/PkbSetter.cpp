#include "PkbSetter.h"
#include "../Type.h"
#include "parsed_statement_temp.h"  // TODO: delete when ParsedStatement is implemented

PkbSetter::PkbSetter(DB *db) : db(db) {}

void PkbSetter::handleVariables(const ParsedStatement& parsedStatement) {
  for (const auto& var: parsedStatement.varModified)
    db->variables.insert(var);
}

void PkbSetter::handleProcedure(const ParsedStatement& parsedStatement) {
  db->procedures.insert(parsedStatement.procedureName);
}

void PkbSetter::handleModifies(const ParsedStatement& parsedStatement) {
  for (const auto& var: parsedStatement.varModified) {
    db->modifyStmtToVarTable[parsedStatement.stmtNo].insert(var);
    db->varToModifyStmtTable[var].insert(parsedStatement.stmtNo);
    db->modifyProcToVarTable[parsedStatement.procedureName].insert(var);
    db->varToModifyProcTable[var].insert(parsedStatement.procedureName);
  }
}

void PkbSetter::handleUses(const ParsedStatement& parsedStatement) {
  for (const auto& var: parsedStatement.varUsed) {
    db->usesStmtToVarTable[parsedStatement.stmtNo].insert(var);
    db->varToUsesStmtTable[var].insert(parsedStatement.stmtNo);
    db->usesProcToVarTable[parsedStatement.procedureName].insert(var);
    db->varToUsesProcTable[var].insert(parsedStatement.procedureName);
  }
}

void PkbSetter::handleStatementType(const ParsedStatement& parsedStatement) {
  db->stmtTypeTable[parsedStatement.stmtNo] = SP_TYPE_ENTITY_TYPE_MAP[parsedStatement.statementType];  // pending SP
}

void PkbSetter::handleParent(const ParsedStatement& parsedStatement) {
  db->parentToChildTable[parsedStatement.ifLineNo].insert(parsedStatement.stmtNo);
  db->childToParentTable[parsedStatement.stmtNo] = parsedStatement.ifLineNo;
  db->parentToChildTable[parsedStatement.whileLineNo].insert(parsedStatement.stmtNo);
  db->childToParentTable[parsedStatement.stmtNo] = parsedStatement.whileLineNo;
}

void PkbSetter::handleFollows(const ParsedStatement& parsedStatement) {
  db->stmtPreceding[parsedStatement.stmtNo] = parsedStatement.preceding;
  db->stmtFollowing[parsedStatement.preceding] = parsedStatement.stmtNo;
}

void PkbSetter::handleCalls(const ParsedStatement& parsedStatement) {
  db->stmtToProcedureCalled[parsedStatement.stmtNo] = parsedStatement.procedureCalled;
}

void PkbSetter::insertStmt(const ParsedStatement& parsedStatement) {
  db->stmtTable[parsedStatement.stmtNo] = parsedStatement;
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

