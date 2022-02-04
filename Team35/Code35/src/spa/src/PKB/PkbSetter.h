#ifndef SPA_PKBSETTER_H
#define SPA_PKBSETTER_H

#include "DB.h"
//#include "parsed_statement_temp.h"  // TODO: delete when ParsedStatement is implemented

class PkbSetter {
private:
  DB* db;

private:
  void handleModifies(const ParsedStatement& parsedStatement);

public:
  PkbSetter(DB* db);
  void insertStmt(const ParsedStatement& parsedStatement);

};


#endif //SPA_PKBSETTER_H
