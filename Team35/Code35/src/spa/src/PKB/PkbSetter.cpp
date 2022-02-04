#include "PkbSetter.h"
//#include "parsed_statement_temp.h"  // TODO: delete when ParsedStatement is implemented

PkbSetter::PkbSetter(DB *db) : db(db) {}

void PkbSetter::handleModifies(const ParsedStatement& parsedStatement) {
  for (const auto& var: parsedStatement.varModified) {
    db->modifiesStmtTable[parsedStatement.stmtNo].insert(var);
  }
}

void PkbSetter::insertStmt(const ParsedStatement& parsedStatement) {
  handleModifies(parsedStatement);
}
