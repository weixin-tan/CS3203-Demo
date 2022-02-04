#ifndef SPA_DB_H
#define SPA_DB_H

#include <map>
#include <set>
#include <string>

// just a dataclass to hold all the tables
struct DB {
public:
  std::map<int, std::set<std::string>> modifiesStmtTable;
  std::map<int, std::set<std::string>> modifiesProcTable;
//  std::map<std::string, int> varToIdTable;
//  std::map<std::string, int> procToIdTable;
  std::map<int, int> followsTable;
  std::map<int, int> parentTable;

public:
  DB();
};


#endif //SPA_DB_H
