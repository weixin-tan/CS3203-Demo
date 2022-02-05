#ifndef SPA_DB_H
#define SPA_DB_H

#include <map>
#include <set>
#include <string>
#include "../Type.h"

// just a dataclass to hold all the tables
struct DB {
 public:
  std::map<int, std::set<std::string>> modifyStmtToVarTable;
  std::map<std::string, std::set<std::string>> modifyProcToVarTable;
  std::map<std::string, std::set<int>> varToModifyStmtTable;
  std::map<int, EntityType> stmtTypeTable;
  std::set<std::string> variables;  // might be redundant? all variables must be modified by a statement?

 public:
  DB();
};


#endif //SPA_DB_H
