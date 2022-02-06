#ifndef SPA_PKBSETTER_H
#define SPA_PKBSETTER_H

#include "DB.h"
#include "../ParsedStatement.h" 


class PkbSetter {
 private:
  DB* db;

 private:
  void handleModifies(const ParsedStatement& parsedStatement);
  void handleStatementType(const ParsedStatement& parsedStatement);
  void handleVariables(const ParsedStatement& parsedStatement);

 public:
  PkbSetter(DB* db);
  void insertStmt(const ParsedStatement& parsedStatement);

};


#endif //SPA_PKBSETTER_H
