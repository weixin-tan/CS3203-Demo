#ifndef SPA_PKBSETTER_H
#define SPA_PKBSETTER_H

#include "DB.h"
#include "parsed_statement_temp.h"  // TODO: delete when ParsedStatement is implemented

class PkbSetter {
 private:
  static std::map<StatementType, EntityType> spTypeToQpsTypeTable;
  DB* db;

 private:
  void handleVariables(const ParsedStatement& parsedStatement);
  void handleProcedure(const ParsedStatement& parsedStatement);
  void handleStatementType(const ParsedStatement& parsedStatement);

  void handleFollows(const ParsedStatement& parsedStatement);
  void handleParent(const ParsedStatement& parsedStatement);
  void handleUses(const ParsedStatement& parsedStatement);
  void handleModifies(const ParsedStatement& parsedStatement);
  void handleCalls(const ParsedStatement& parsedStatement);

 public:
  PkbSetter(DB* db);
  void insertStmt(const ParsedStatement& parsedStatement);

};


#endif //SPA_PKBSETTER_H
